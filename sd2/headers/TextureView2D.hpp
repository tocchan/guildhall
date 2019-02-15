//------------------------------------------------------------------------
// TextureView2D.hpp
//------------------------------------------------------------------------

class TextureView // A03
{
   public:
      TextureView();       // A03
      ~TextureView();      // A03

      void SetSampler( Sampler *samp );   // A03

   public:
      // things this holds onto
      ID3D11ShaderResourceView *m_view = nullptr;  // A03
      ID3D11Resource *m_source         = nullptr;  // A03 
      Sampler *m_sampler               = nullptr;  // A03
}; 


//------------------------------------------------------------------------
class TextureView2D : public TextureView  // A03
{
   public:
      vec2 m_size;                        // A03
}; 


