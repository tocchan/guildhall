class Texture
{
   public:
      // constructor used soley for the swapchain
      Texture( RenderContext* owner, ID3D11Resource* handle ); 
      ~Texture(); 

      TextureView* GetColorTargetView(); 

   public:
      RenderContext* m_owner; 
      ID3D11Resource* m_handle; 

      // temp for now - eventually a texture
      // can have multiple different views of it,
      // but in the first assignment
      TextureView* m_ctv; 
}; 