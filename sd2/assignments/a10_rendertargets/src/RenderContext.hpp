//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
     
      // ...
      ColorTargetView* GetFrameColorTarget();               // A01, A10
      
      void CopyTexture( Texture2D *dst, Texture2D *src );   // A10
      void ApplyEffect( Texture2D *dst, Texture2D *src, Material *mat ); // A10 

}; 


