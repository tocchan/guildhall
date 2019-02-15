//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      RenderContext();                 // A01
      ~RenderContext();                // A01

      // APP CONTROL
      void Startup();                  // A01, A02, A03
      void Shutdown();                 // A02, A02, A03

      void BeginFrame();               // A01
      void EndFrame();                 // A01

      // DRAWING
      void BeginCamera( Camera* );     // A01, A02, A03, A04, A05
      void EndCamera();                // A01

      void ClearColorTargets( rgba const &color );                              // A01
      void ClearDepthStencilTarget( float depth = 1.0f, uint8_t stencil = 0U ); // A05

      // State Binding
      void BindShader( Shader *shader );  // A01

      // Input Binding
      void BindVertexStream( VertexBuffer *vbo );  // A02
      void BindIndexStream( IndexBuffer *ibo );    // A04

      // Resource Binding
      // Constants
      void BindUniformBuffer( uint slot, UniformBuffer *ubo );    // A02
      void BindModelMatrix( mat44 const &modelMatrix );           // A04
      
      // Textures & Samplers
      void BindTextureView( uint slot, TextureView *view );       // A03
      void BindSampler( uint slot, Sampler *sampler );            // A03

      void BindTextureViewWithSampler( uint slot, TextureView *view );                          // A03
      void BindTextureViewWithSampler( uint slot, std::string const &name );                    // A03
      void BindTextureViewWithSampler( uint slot, TextureView *view, Sampler *sampler );        // A03
      void BindTextureViewWithSampler( uint slot, TextureView *view, eSampleMode mode );        // A03
      void BindTextureViewWithSampler( uint slot, std::string const &name, eSampleMode mode );  // A03

      // Drawing
      void Draw( uint vertexCount, uint byteOffset = 0U );                 // A01
      void DrawIndexed( uint indexCount, uint byteOffset = 0U );           // A04

      void DrawVertexArrays( VertexPCU const *vertices, uint count );      // A02
      void DrawMesh( Mesh *mesh );                                         // A04

      // Any Additional Helper Drawing Methods
      // ... 

   public:
      Shader* GetOrCreateShader( std::string const &filename );                // A01
      TextureView2D* GetOrCreateTextureView2D( std::string const &filename );  // A03

   public:
      // D3D11 Handles
      ID3D11Device *m_device;                         // A01
      ID3D11DeviceContext *m_context;                 // A01
      ID3D11SwapChain *m_swapchain;                   // A01

      // Stateful Data
      ColorTargetView *m_frameBackbuffer;             // A01
      Camera *m_currentCamera;                        // A01

      // Immediate Drawing Utility
      VertexBuffer *m_immediateVBO;                   // A02 
      Shader *m_defualtShader;                        // Optional (allow some SD1 methods to work - used when nullptr shader is set)

      // cached resources for convenience; 
      Sampler* m_cachedSamplers[SAMPLE_MODE_COUNT];   // A03

      // Databases
      std::map<std::string, Shader*> m_shaderDatabase;             // A01
      std::map<std::string, TextureView2D*> m_textureViewDatabase; // A03
}; 


