//------------------------------------------------------------------------
// Texture2D.hpp
//------------------------------------------------------------------------

// Textures are just another form of a buffer, so enums used
// to describe buffers also apply to textures; 
#include "Renderer/RenderBuffer.h"

// Eventually, when we add new Texture types, these common 
// enums & classes should be moved to Texture.hpp
enum eTextureUsageBit : uint
{
   TEXTURE_USAGE_TEXTURE_BIT              = BIT_FLAG(0),    // Can be used to create a TextureView
   TEXTURE_USAGE_COLOR_TARGET_BIT         = BIT_FLAG(1),    // Can be used to create a ColorTargetView
   TEXTURE_USAGE_DEPTH_STENCIL_TARGET_BIT = BIT_FLAG(2),    // Can be used to create a DepthStencilTargetView
}; 
typedef uint eTextureUsageBits; 

//------------------------------------------------------------------------
class Texture
{
   public:
      Texture(); 
      virtual ~Texture(); // virtual, as this will release the resource; 

   public:
      // D3D11 objets; 
      ID3D11Resource *m_handle; 
      eGPUMemoryUsage m_memoryUsage; 
      eTextureUsageBits m_textureUsage; 

      // If you have the concept of an image format (RGBA8, R8, D24S8, etc..)
      // It would go here.  If you only have support for RGBA8, we'll just assume it; 
      // eImageFormat m_format; 

};

//------------------------------------------------------------------------
class Texture2D : public Texture
{
   public:
      Texture2D(); 
      virtual ~Texture2D(); 

   public:
      vec2 m_dimensions; 
};

//------------------------------------------------------------------------
// Texture2D.cpp
//------------------------------------------------------------------------
