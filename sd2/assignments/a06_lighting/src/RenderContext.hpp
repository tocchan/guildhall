//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      // ...
      // Lighting - update the local light buffers
      void SetAmbientLight( rgba const &color, float intensity ); // A06
      void EnableLight( uint slot, light_t const &info );         // A06
      void DisableLight( uint slot );                             // A06
      // Add additional helpers as you see fit; 

      // ...
      void BeginCamrea( Camera *cam );   // don't forget to bind the light buffer to the correct slot

      // ...
      void DrawMesh( Mesh *mesh );                                         // A04 -> A06

   public:
      // ...
      
      // light buffer   
      UniformBuffer *m_gpuLightBuffer;                   // A06 - constant buffer storing lights; 
      light_buffer_t m_cpuLightBuffer;                   // A06 - CPU copy of light data
      bool m_lightBufferDirty;                           // A06 - need to update the light buffer before a draw
}; 

//------------------------------------------------------------------------
// .cpp
//------------------------------------------------------------------------

// Two new things happen in draw (both DrawIndexed and Draw)
// You may want to move this to a PreDraw() private method; 
void RenderContext::DrawMesh( Mesh *mesh )
{
   // First, finalize the light buffer
   if (m_lightBufferDirty) {
      m_lightBufferDirty = false; 
      m_gpuLightBuffer.CopyCPUToGPU( m_gpuLightBuffer ); 
   }

   // Second, make sure the correct input layout exists
   m_currentShader->UpdateInputLayout( mesh->GetLayout() ); 

   // bind input layout and draw as normal; 
   // ...

}
