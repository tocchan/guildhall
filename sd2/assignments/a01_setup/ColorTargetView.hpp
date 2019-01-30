#define DX_SAFE_RELEASE(dx_obj)  if (dx_obj != nullptr) { dx_obj->Release(); dx_obj = nullptr; }

//------------------------------------------------------------------------
// *new* ColorTargetView.hpp
//------------------------------------------------------------------------
class ColorTargetView
{
   public: // external use
      ColorTargetView() {}
      ~ColorTargetView(); 

      float GetWidth() const; 
      float GetHeight() const; 

   public: // internal use - could be private with friends;
      void CreateForInternalTexture( ID3D11Texture2D *tex );

   public: // a renderable view of a texture // private
      ID3D11RenderTargetView *m_rtv    = nullptr; 
      vec2 m_size                      = vec2::ZERO; 
}; 

//------------------------------------------------------------------------
// ColorTargetView.cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
ColorTargetView::~ColorTargetView()
{
   // be sure to release this D3D11 resource
   DX_SAFE_RELEASE( m_rtv ); 
}

//------------------------------------------------------------------------
void ColorTargetView::CreateForInternalTexture( ID3D11Texture2D *tex ) 
{
   DX_SAFE_RELEASE( m_rtv ); // release the old one if present 

      // next, create the rtv for it; 
   HREUSLT hr = m_device->CreateRenderTargetView( tex,  // texture to create the view for
      nullptr,   // default options (use texture info)
      &m_rtv );    // out variable to store the view

   GUARANTEE_OR_DIE( SUCCEEDED(hr) ); // make sure we could get it 
   // if this fails, check your output - DX will usually tell you why
   // (likely texture wasn't setup to be an output)

   // cache off size from D3D11 texture; 
   D3D11_TEXTURE2D_DESC desc;
   tex->GetDesc( &desc )
   m_size = vec2( desc.Width, desc.Height ); 
}

