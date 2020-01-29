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

      void BeginCamera( Camera* camera );       // A01, A02
      void EndCamera();                         // A01

      // Binding Inputs
      void BindVertexBuffer( VertexBuffer* vbo );  // A02

      // Binding State
      void BindShader( Shader* shader );        // A02
      void BindShader( char const* filename );  // A02

      // Resource Creation
      Shader* GetOrCreateShader( char const* filename ); 

   private:


};