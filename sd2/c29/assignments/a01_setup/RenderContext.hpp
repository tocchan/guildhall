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

      void BeginCamera( Camera* camera );       // A01
      void EndCamera();                         // A01

      // In A01 - everything else you have is going to be broken
      // so insert an UNIMPLEMENTED macro

   private:


};