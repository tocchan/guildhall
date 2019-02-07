//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

// As the engine designer, we're going to make the decision to 
// just reserve certain slot indices for specific constant buffers
// We will gruop uniform buffers by how often they change, and for now, we
// really only use the CAMERA
enum eCoreUniformSlot 
{
   UNIFORM_SLOT_FRAME         = 1,
   UNIFORM_SLOT_CAMERA        = 2, 
}; 

// A constant buffer can address up 64KB,
// with 15 available slots (docs usually say 14, as one gets used
// up by the $GLOBAL constant buffer; 

//------------------------------------------------------------------------
// FRAME data is information that is constant for an entire
// frame - you can update this UBO during RenderContext::BeginFrame, 
// as well as bind it (as long as you don't accidently bind over it
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// I usually use this to store time, as it is a common
// input for shader effects
// A UBO should usually be a multipole of 16-bytes large, so we'll just add some 
// variations of time for our shaders to use; 
// slot: 1
struct frame_buffer_t
{
   float time; 
   float halfTime; 
   float doubleTime; 
   float quadTime; 
}; 

//------------------------------------------------------------------------
// slot 2; 
// What data does the camera provide the shader (slot:2)
// This must follow a 16-byte element alignemnt (for now, this only contains
// ortho information; 
// camera slot I usually bind all constants that change per view; 
struct camera_buffer_t 
{
   vec2 orthoMin; 
   vec2 orthoMax; 
};

// I start at slot 1 out of habit.  I reserve slot 0 for what I call the "SYTEM" buffer, which
// is usually just debug constants and configurations as I'm working. 
// The uniform buffer system is very free-form, you can bind any data to any slot
// but I like to usually have my engine renderer reserve a few slots
// grouped by update frequency.


//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      //...

      // BINDING  *new methods*
      
   
      // Changed methods
      void BeginCamera( Camera *cam ); 
      void EndCamera(); 

      // Stream Data
      void BindVertexStream( VertexBuffer *vbo ); 

      // Uniform/Constant Data
      void BindUniformBuffer( uint slot, UniformBuffer *ubo ); 


      void Draw( uint vertex_count, uint byte_offset = 0U ); 

      // Resurrected Functions
      void DrawVertexArrays( VertexPCU const *vertices, uint count ); 


   public:
      //...

      // *new* members
      VertexBuffer *m_immediateVBO; 
}; 

//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------


//------------------------------------------------------------------------
void RenderContext::BeginCamera( Camera *cam ) 
{
   // previous camera code is still here
   // ...


   // Next, update the uniform data, and bind it as an input for the shader
   // Camera data
   cam->UpdateUniformBuffer( this ); 
   BindUniformBuffer( UNIFORM_SLOT_CAMERA, cam->m_cameraUBO ); 
}

   //------------------------------------------------------------------------
void RenderContext::BindUniformBuffer( uint slot, UniformBuffer *ubo ) 
{
   // The API allows us to bind multiple constant buffers at once
   // and binds to each stage seperately.  For simplicity, we'll
   // just assume a slot is uniform for the entire pipeline
   ID3D11Buffer *buffer = (ubo != nullptr) ? ubo->GetHandle() : nullptr; 
   m_context->VSSetConstantBuffers( idx, 1U, &buffer ); 
   m_context->PSSetConstantBuffers( idx, 1U, &buffer ); 
}; 

//------------------------------------------------------------------------
void RenderContext::DrawVertexArrays( VertexPCU const *vertices, uint count )
{
   // copy to a vertex buffer
   m_immediateVBO->CopyCPUToGPU( vertcies, count );

   // bind that vertex buffer
   BindVertexStream( m_immediateVBO ); 

   Draw( count ); 
}

//------------------------------------------------------------------------
void RenderContext::BindVertexStream( VertexBuffer *vbo ) 
{
   // Bind the input stream; 
   uint stride = sizeof(VertexPCU);
   uint offset = 0U;
   m_context->IASetVertexBuffers( 0,    // Start slot index
      1,                            // Number of buffers we're binding
      &m_immediateVBO->GetHandle(), // Array of buffers
      &stride,                // Stride (read: vertex size, or amount we move forward each vertex) for each buffer
      &offset );             // Offset into each buffer (array - we are only passing one. 
}


//------------------------------------------------------------------------
void RenderContext::Draw( uint vertCount, uint byteOffset ) 
{
   // **NEW** - before a draw can happen, 
   // We need to describe the input to the shader
   // TODO: only create an input layout if the vertex type changes; 
   // TODO: When different vertex types come on-line, look at the current bound
   //       input streams (VertexBuffer) for the layout
   bool result = m_currentShader->CreateInputLayoutForVertexPCU(); 
   // TODO: m_currentShader->CreateInputLayoutFor( VertexPCU::LAYOUT ); 

   if (result) {
      m_context->IASetInputLayout( m_currentShader->m_inputLayout );
      m_context->Draw( vertCount, byteOffset ); 
   } else {
      // error/warning
   }
}
