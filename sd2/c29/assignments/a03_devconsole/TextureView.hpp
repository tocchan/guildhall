class TextureView
{
   public:
      TextureView(); 
      ~TextureView(); // just be sure to release the handle

   public:
      Texture* m_owner; 

      union {
         ID3D11Resource* m_handle;        // A01

         ID3D11ShaderResourceView* m_srv; // A03 - what is bound to a shader stage for reading
         ID3D11ColorTargetView* m_rtv;    // A01 - alias as an rtv
      }; 


}; 