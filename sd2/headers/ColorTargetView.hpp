
class ColorTargetView
{
   public: // external use
      ColorTargetView();                                 // A01
      ~ColorTargetView();                                // A01

      float GetWidth() const;                            // A01
      float GetHeight() const;                           // A01

   public: // internal use - could be private with friends;
      void CreateForInternalTexture( ID3D11Texture2D *tex );   // A01

   public: // a renderable view of a texture // private
      ID3D11Resource *m_source         = nullptr;        // ???
      ID3D11RenderTargetView *m_rtv    = nullptr;        // A01
      vec2 m_size                      = vec2::ZERO;     // A01

}; 

