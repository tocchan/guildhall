class Texture
{
   public:
      // constructor used soley for the swapchain
      Texture( RenderContext* owner, ID3D11Resource* handle ); 
      Texture( RenderContext* owner, std::string const& filename ); // A03 - be able to load a file; 
      ~Texture(); 

      TextureView* GetColorTargetView();     // A01
      TextureView* GetShaderResourceView();  // A03, needed to bind to a shader

   public:
      RenderContext* m_owner; 
      ID3D11Resource* m_handle; 

      // temp for now - eventually a texture
      // can have multiple different views of it,
      // but in the first assignment
      TextureView* m_ctv;                 // A01
      TextureView* m_srv;                 // A03
}; 