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

      ColorTargetView* GetFrameColorTarget();               // A01
      DepthStencilTargetView* GetFrameDepthStencilTarget(); // A04

      // DRAWING
      void BeginCamera( Camera* );     // A01, A02, A03, A04
      void EndCamera();                // A01

      void ClearColorTargets( rgba const &color );                              // A01
      void ClearDepthStencilTarget( float depth = 1.0f, uint8_t stencil = 0U ); // A04

      // State Binding
      void BindShader( Shader *shader );  // A01, A04

      // Input Binding
      void BindVertexStream( VertexBuffer *vbo );  // A02
      void BindIndexStream( IndexBuffer *ibo );    // A04

      // Resource Binding
      // Constants
      void BindUniformBuffer( uint slot, UniformBuffer *ubo );    // A02
      void SetModelMatrix( mat44 const &modelMatrix );           // A04

      // Lighting
      void SetAmbientLight( rgba const &color, float intensity ); // A06
      void EnableLight( uint slot, light_t const &info );         // A06
      void DisableLight( uint slot );                             // A06
      // Add additional helpers as you see fit; 
      
      // Textures & Samplers
      void BindTextureView( uint slot, TextureView *view );       // A03
      void BindSampler( uint slot, Sampler *sampler );            // A03

      void BindTextureViewWithSampler( uint slot, TextureView *view );                          // A03
      void BindTextureViewWithSampler( uint slot, std::string const &name );                    // A03
      void BindTextureViewWithSampler( uint slot, TextureView *view, Sampler *sampler );        // A03
      void BindTextureViewWithSampler( uint slot, TextureView *view, eSampleMode mode );        // A03
      void BindTextureViewWithSampler( uint slot, std::string const &name, eSampleMode mode );  // A03

      void BindMaterial( Material *mat );                                  // A07

      // Drawing
      void Draw( uint vertexCount );                                       // A01
      void DrawIndexed( uint indexCount );                                 // A04

      void DrawVertexArrays( VertexPCU const *vertices, uint count );      // A02
      void DrawMesh( GPUMesh *mesh );                                      // A04 -> A06

      // Any Additional Helper Drawing Methods
      // ... 

   public:
      Shader* GetOrCreateShader( std::string const &filename );                // A01
      TextureView2D* GetOrCreateTextureView2D( std::string const &filename );  // A03
      Material* GetOrCreateMaterial( std::string const &filename );            // A07

   public:
      // D3D11 Handles
      ID3D11Device *m_device;                         // A01
      ID3D11DeviceContext *m_context;                 // A01
      ID3D11SwapChain *m_swapchain;                   // A01

      // Stateful Data
      ColorTargetView *m_frameBackbuffer;                // A01
      DepthStencilTargetView *m_defaultDepthStencilView; // A04
      UniformBuffer *m_modelBuffer;                      // A04
      
      // light buffer   
      UniformBuffer *m_gpuLightBuffer;                   // A06 - constant buffer storing lights; 
      light_buffer_t m_cpuLightBuffer;                   // A06 - CPU copy of light data
      bool m_lightBufferDirty;                           // A06 - need to update the light buffer before a draw

      Camera *m_currentCamera;                           // A01

      // Immediate Drawing Utility
      VertexBuffer *m_immediateVBO;                   // A02 
      GPUMesh *m_immediateMesh;                       // -> A04 (replaces m_immediateVBO)
      Shader *m_defaultShader;                        // Optional (allow some SD1 methods to work - used when nullptr shader is set)

      // cached resources for convenience; 
      Sampler* m_cachedSamplers[SAMPLE_MODE_COUNT];   // A03

      // Databases
      std::map<std::string, Shader*> m_shaderDatabase;             // A01
      std::map<std::string, TextureView2D*> m_textureViewDatabase; // A03
      std::map<std::string, Material*> m_materialDatabase;         // A07
}; 


