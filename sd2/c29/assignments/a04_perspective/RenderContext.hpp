// RenderContext encompasses
// the device and immediate context of D3D11
// controlling drawing
class RenderContext
{
   public:
      RenderContext();                          // A01
      ~RenderContext();                         // A01

      void Startup( Window* defaultWindow );    // A01
      void Shutdown();                          // A01

      void BeginFrame();                        // A01
      void EndFrame();                          // A01

      Texture* GetFrameColorTarget();           // A01

      void BeginCamera( Camera* camera );       // A01, A02, A03
      void EndCamera();                         // A01

      // Binding Inputs
      void BindVertexBuffer( VertexBuffer* vbo );  // A02

      // Binding State
      void BindShader( Shader* shader );        // A02
      void BindShader( char const* filename );  // A02

      void BindTexture( uint slot, Texture* tex );     // A03
      void BindSampler( uint slot, Sampler* sampler ); // A03

      void BindConstantBuffer( uint slot, RenderBuffer* cbo ); // A03

      // Resource Creation
      Shader* GetOrCreateShader( char const* filename ); // A02

      // Drawing (Raw)
      void Draw( int vertexCount, int vertexOffset = 0 );   // A02
      void DrawIndexed( int indexCount, int indexOffset = 0, int vertexOffset = 0 ); // A04

      // Drawing (Helpers)
      void DrawVertexArray( uint vcount, VertexPCT const* vertices ); // A02, A03
      void DrawMesh( GPUMesh* mesh );     // A04  

      // Database Functions
      Texture* GetOrCreateTexture( std::string const& filename ); // A03
      Shader* GetOrCreateShader( std::string const& filename );   // A03

   private:



};