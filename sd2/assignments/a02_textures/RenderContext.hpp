//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

// common sampler types
// that we'll just have the RenderContext cache off; 
enum eSamplerType
{
   SAMPLER_NEAREST,
   SAMPLER_BILINEAR, 
}; 

// As the engine designer, we're going to make the decision to 
// just reserve certain slot indices for specific constant buffers
// We will gruop uniform buffers by how often they change, and for now, we
// really only use the CAMERA
enum eCoreUniformSlot 
{
   UNIFORM_SLOT_SYSTEM        = 0,
   UNIFORM_SLOT_FRAME         = 1,
   UNIFORM_SLOT_CAMERA        = 2, 
}; 

// What data does the camera provide the shader (slot:2)
// This must follow a 16-byte element alignemnt (for now, this only contains
// ortho information; 
struct camera_buffer_t 
{
   // ...
};

class RenderContext
{
   public:
      //...

      // new methods
      void BindTextureView( uint slot, TextureView *view ); 
      void BindTextureView( uint slot, std::string const &tex_name ); 

      void BindSampler( uint slot, Sampler *sampler ); 
      void BindSampler( uint slot, eSamplerType samplerType ); 
      
      // extra convenience - bind texture and sampler together
      void BindTextureView( uint slot, std::string const &name, eSamplerType type ); 

      // Changed methods
      void BeginCamera( Camera *cam ); 
      void Draw( uint vertex_count, uint byte_offset = 0U ); 

      // Resurrected Functions
      void DrawVertexArrays( VertexPCU const *vertices, uint count ); 


   public: // Resources
      TextureView2D* GetOrCreateTextureView2D( std::string const &name ); 

   public:
      //...

      // *new* members
      VertexBuffer *m_immediateVBO; 

      Sampler *m_defaultPoint; 
      Sampler *m_defaultBilinear; 
}; 

//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------


void RenderContext::DrawVertexArrays( VertexPCU const *vertices, uint count )
{
   m_immediateVBO->CopyCPUToGPU( vertcies, count * sizeof(VertexPCU) );

   // 1. What are we drawing - so input data.  In this case, we're going to 
   // be drawing some vertices stored in a Vertex Buffer Object (VBO)
   uint stride = sizeof(VertexPCU);
   uint offset = 0U;
   m_context->IASetVertexBuffers( 0,    // Start slot index
      1,                            // Number of buffers we're binding
      &m_immediateVBO->GetHandle(), // Array of buffers
      &stride,                // Stride (read: vertex size, or amount we move forward each vertex) for each buffer
      &offset );             // Offset into each buffer (array - we are only passing one. 
  
   // Bind VertexBufferToShader
   // ...
   m_inputLayout->BindLayoutToShader( VertexPCU::LAYOUT, m_currentShader ); 
   m_context->IASetInputLayout( m_inputLayout );

   Draw( count ); 
}
