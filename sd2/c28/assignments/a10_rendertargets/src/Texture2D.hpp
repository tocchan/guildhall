//------------------------------------------------------------------------
// Texture2D.hpp
//------------------------------------------------------------------------

// Textures are just another form of a buffer, so enums used
// to describe buffers also apply to textures; 
#include "Renderer/RenderBuffer.h"

//------------------------------------------------------------------------
class Texture2D : public Texture       // A03
{
   public:
      // ...
      ColorTargetView* CreateColorTargetView() const;    

   public: // Helper statics
      // ...
      static Texture2D* CreateColorTarget( RenderContext *ctx, uint width, uint height, eImageFormat format );  // A10
      static Texture2D* CreateMatchingColorTarget( Texture2D *other );  // A10
};
