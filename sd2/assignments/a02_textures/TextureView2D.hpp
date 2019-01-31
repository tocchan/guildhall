//------------------------------------------------------------------------
// TextureView2D.hpp
//------------------------------------------------------------------------

class TextureView
{
   public:
      RenderContext *m_owner           = nullptr;  // who created me (which device do I belong to?)
      Sampler *m_sampler               = nullptr;  // optional sampler to use; 

      ID3D11ShaderResourceView *m_view = nullptr;  // D3D11 specific info
}; 

class TextureView2D : public TextureView 
{
}; 


//------------------------------------------------------------------------
// TextureView2D.cpp
//------------------------------------------------------------------------
