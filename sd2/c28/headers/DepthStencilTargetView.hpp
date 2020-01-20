
class DepthStencilTargetView                             // A04
{
   public: // external use
      DepthStencilTargetView();                          // A04
      ~DepthStencilTargetView();                         // A04

      float GetWidth() const;                            // A04
      float GetHeight() const;                           // A04

   public: // a renderable view of a texture // private
      ID3D11Resource *m_source         = nullptr;        // A04
      ID3D11DepthStencilView *m_rtv    = nullptr;        // A04
      vec2 m_size                      = vec2::ZERO;     // A04

}; 

