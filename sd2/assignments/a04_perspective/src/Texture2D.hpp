//------------------------------------------------------------------------
// Texture2D.hpp
//------------------------------------------------------------------------

// Textures are just another form of a buffer, so enums used
// to describe buffers also apply to textures; 
#include "Renderer/RenderBuffer.h"

//------------------------------------------------------------------------
class Texture                
{
   public:
      // ...

   public:
      // ...

      eImageFormat m_imageFormat;               // A04
};

//------------------------------------------------------------------------
class Texture2D : public Texture       // A03
{
   public:
      DepthStencilTargetView* CreateDepthStencilTargetView() const;  // A04

   public: // Helper statics
      static Texture2D* CreateDepthStencilTargetFor( Texture2D *colorTarget );   // A04
};

//------------------------------------------------------------------------
// Texture2D.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
