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
      Texture( RenderContext *ctx ); 
      virtual ~Texture(); // virtual, as this will release the resource; 

   public:
      RenderContext *m_owner; 

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
      Texture2D( RenderContext *ctx );  // texures always come from a context; 
      virtual ~Texture2D(); 

      bool LoadFromFile( std::string const &filename ) 
      bool LoadFromImage( Image const &image ) 

      // Create a view of this texture usable in the shader; 
      TextureView2D* CreateTextureView2D() const;  

   public:
      vec2 m_dimensions; 
};

//------------------------------------------------------------------------
// Texture2D.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static uint DXBindFromUsage( uint usage ) 
{
   uint binds = 0U; 

   if (usage & TEXTURE_USAGE_TEXTURE_BIT) {
      binds |= D3D11_BIND_SHADER_RESOURCE; 
   }
   if (usage & TEXTURE_USAGE_COLOR_TARGET_BIT) {
      binds |= D3D11_BIND_RENDER_TARGET; 
   }
   if (usage & TEXTURE_USAGE_DEPTH_STENCIL_TARGET_BIT) {
      binds |= D3D11_BIND_DEPTH_STENCIL_TARGET; 
   }

   return binds; 
}

//------------------------------------------------------------------------
bool Texture2D::LoadFromFile( std::string const &filename ) 
{
   // I am assuming you already have an Image class from SD1 (let me know if you do not!)
   Image img;
   if (!img.LoadFromFile(filename)) {
      return false; 
   }

   return LoadFromImage( image ); 
}
      
//------------------------------------------------------------------------
bool Texture2D::LoadTextureFromImage( Image const &image ) 
{
   // cleanup old resources before creating new one just in case; 
   FreeHandles(); 

   ID3D11Device *dd = m_device->m_device; 

   // If created from image, we'll assume it is only needed
   // as a read-only texture resource (if this is not true, change the
   // signature to take in the option)
   m_textureUsage = TEXTURE_USAGE_TEXTURE_BIT;

   // we are not picking static here because
   // we will eventually want to generate mipmaps,
   // which requires a GPU access pattern to generate.
   m_memoryUsage = GPU_MEMORY_USAGE_GPU; 

   // Setup the Texture Description (what the resource will be like on the GPU)
   D3D11_TEXTURE2D_DESC texDesc;
   MemZero( &texDesc );

   texDesc.Width = image.GetWidth();
   texDesc.Height = image.GetHeight();
   texDesc.MipLevels = 1; // setting to 0 means there's a full chain (or can generate a full chain)
   texDesc.ArraySize = 1; // only one texture
   texDesc.Usage = DXUsageFromMemoryUsage(m_memoryUsage);  // loaded from image - probably not changing
   texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // if you support different image types  - this could change!  
   texDesc.BindFlags = DXBindFromUsage(m_textureUsage);   // only allowing rendertarget for mipmap generation
   texDesc.CPUAccessFlags = 0U;                            // Determines how I can access this resource CPU side 
   texDesc.MiscFlags = 0U;  
   
   // If Multisampling - set this up.
   texDesc.SampleDesc.Count = 1;
   texDesc.SampleDesc.Quality = 0;

   // Setup Initial Data
   // pitch is how many bytes is a single row of pixels;  
   uint pitch = image.GetWidth() * image.GetBytesPerPixel(); // 4 bytes for an R8G8B8A8 format;  Just sub in four if your image is always 4 bytes per channel
   D3D11_SUBRESOURCE_DATA data;
   MemZero( &data );
   data.pSysMem = image.GetBuffer();
   data.SysMemPitch = pitch;

   // Actually create it
   ID3D11Texture2D *tex2D = nullptr; 
   HRESULT hr = dd->CreateTexture2D( &texDesc,
      &data, 
      &tex2D );

   if (SUCCEEDED(hr)) {
      // save off the info; 
      m_size = image.GetSize(); 
      m_handle = tex2D;
      // TODO later assigment, generate mips if option is set; 

      return true; 

   } else {
      ASSERT( tex2D == nullptr ); // should be, just like to have the postcondition; 
      return false; 
   }
}

//------------------------------------------------------------------------
TextureView2D* Texture2D::CreateTextureView2D() const
{
   // if we don't have a handle, we can't create a view, so return nullptr
   ASSERT_RETURN_VALUE( m_handle != nullptr, nullptr );

   // 2D - we will want to eventually create specific views of a texture
   // and will want ot fill out a D3D11_SHADER_RESOURCE_VIEW_DESC, but for now
   // we'll just do the default thing (nullptr)

   // get our device - since we're creating a resource
   ID3D11Device *dev = m_context->m_device; 
   ID3D11ShaderResourceView *srv = nullptr; 
   dev->CreateShaderResourceView( m_handle, nullptr, &srv );

   if (srv != nullptr) {
      // Awesome, we have one
      TextureView2D *view = new Texture2DView();

      // give it the handle to the srv (we do not AddRef, 
      // but are instead just handing this off)
      view->m_view = srv; 

      // Also let the view hold onto a handle to this texture
      // (so both the texture AND the view are holding onto it)
      // (hence the AddRef)
      m_handle->AddRef(); 
      view->m_source = m_handle; 

      // copy the size over for convenience
      view->m_size = m_size;

      // done - return!
      return view; 
   
   } else {
      return nullptr; 
   }
}
