//------------------------------------------------------------------------
// Texture2D.hpp
//------------------------------------------------------------------------

// Textures are just another form of a buffer, so enums used
// to describe buffers also apply to textures; 
#include "Renderer/RenderBuffer.h"

//------------------------------------------------------------------------
class Texture                 // A03
{
   public:
      Texture( RenderContext *ctx );            // A03
      virtual ~Texture();                       // A03 

   public:
      RenderContext *m_owner;                   // A03

      // D3D11 objets; 
      ID3D11Resource *m_handle;                 // A03
      eGPUMemoryUsage m_memoryUsage;            // A03
      eTextureUsageBits m_textureUsage;         // A03

      eImageFormat m_imageFormat;               // A05
};

//------------------------------------------------------------------------
class Texture2D : public Texture       // A03
{
   public:
      Texture2D( RenderContext *ctx );       // A03
      virtual ~Texture2D();                  // A03

      bool LoadFromFile( std::string const &filename );  // A03 
      bool LoadFromImage( Image const &image );          // A03

      // Create a view of this texture usable in the shader; 
      TextureView2D* CreateTextureView2D() const;                    // A03  
      DepthStencilTargetView* CreateDepthStencilTargetView() const;  // A04
      ColorTargetView* CreateColorTargetView() const;                // A10

   public:
      vec2 m_dimensions;                     // A03

   public: // Helper statics
      static Texture2D* CreateColorTarget( RenderContext *ctx, uint width, uint height, eImageFormat format );  // A10
      static Texture2D* CreateMatchingColorTarget( Texture2D *other );  // A10
      
      static Texture2D* CreateDepthStencilTarget( RenderContext *ctx, uint widht, uint height );   // A04
      static Texture2D* CreateDepthStencilTargetFor( Texture2D *colorTarget );   // A04
};
