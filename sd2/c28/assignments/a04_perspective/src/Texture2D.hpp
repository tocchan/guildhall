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
      bool CreateDepthStencilTarget( uint widht, uint height ); 
      DepthStencilTargetView* CreateDepthStencilTargetView();  // A04

   public: // Helper statics
      static Texture2D* CreateDepthStencilTarget( RenderContext *ctx, uint widht, uint height );   // A04
      static Texture2D* CreateDepthStencilTargetFor( Texture2D *colorTarget );   // A04
};

//------------------------------------------------------------------------
// Texture2D.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
bool Texture2D::CreateDepthStencilTarget( uint widht, uint height )
{
   // cleanup old resources before creating new one just in case; 
   FreeHandles(); 

   ID3D11Device *dd = m_device->m_device; 

   // We want this to be bindable as a depth texture
   // AND a shader resource (for effects later);
   m_textureUsage = TEXTURE_USAGE_TEXTURE_BIT | TEXTURE_USAGE_DEPTH_STENCIL_TARGET_BIT; 

   // we are not picking static here because
   // we will eventually want to generate mipmaps,
   // which requires a GPU access pattern to generate.
   m_memoryUsage = GPU_MEMORY_USAGE_GPU; 

   D3D11_TEXTURE2D_DESC texDesc;
   MemZero( &texDesc );

   texDesc.Width = image.GetWidth();
   texDesc.Height = image.GetHeight();
   texDesc.MipLevels = 1; // setting to 0 means there's a full chain (or can generate a full chain)
   texDesc.ArraySize = 1; // only one texture
   texDesc.Usage = DXUsageFromMemoryUsage(m_memoryUsage);  // loaded from image - probably not changing
   texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;            // if you support different image types  - this could change!  
   texDesc.BindFlags = DXBindFromUsage(m_textureUsage);    // only allowing rendertarget for mipmap generation
   texDesc.CPUAccessFlags = 0U;                            // Determines how I can access this resource CPU side 
   texDesc.MiscFlags = 0U;  
   
   // If Multisampling - set this up.
   texDesc.SampleDesc.Count = 1;
   texDesc.SampleDesc.Quality = 0;

   // Actually create it
   ID3D11Texture2D *tex2D = nullptr; 
   HRESULT hr = dd->CreateTexture2D( &texDesc,
      nullptr, 
      &tex2D );

   if (SUCCEEDED(hr)) {
      // save off the info; 
      m_size = image.GetSize(); 
      m_handle = tex2D;

      return true; 

   } else {
      ASSERT( tex2D == nullptr ); // should be, just like to have the postcondition; 
      return false; 
   }
}
}


//------------------------------------------------------------------------
STATIC Texture2D* Texture2D::CreateDepthStencilTargetFor( Texture2D *colorTarget )
{
   return CreateDepthStencilTarget( colorTarget->GetContext(), colorTarget->GetWidth(), colorTarget->GetHeight() ); 
   // depth target has to match the dimensions; 
}


//------------------------------------------------------------------------
DepthStencilTargetView* Texture2D::CreateDepthStencilTargetView() const
{
   // if we don't have a handle, we can't create a view, so return nullptr
   ASSERT_RETURN_VALUE( m_handle != nullptr, nullptr );

   // get our device - since we're creating a resource
   ID3D11Device *dev = m_context->m_device; 
   ID3D11DepthStencilView *dsv = nullptr; 

   D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
   MemZero( &dsv_desc );
   dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

   dev->CreateDepthStencilView( m_handle, &dsv_desc, &dsv );

   if (dsv != nullptr) {
      // Awesome, we have one
      DepthStencilTargetView *view = new DepthStencilTargetView();

      // give it the handle to the srv (we do not AddRef, 
      // but are instead just handing this off)
      view->m_view = dsv; 

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
