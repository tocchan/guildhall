enum eTextureUsageBit : uint
{
   TEXTURE_SHADER_RESOURCE_BIT       = BIT_FLAG(0),    // A03
   TEXTURE_RENDER_TARGET_BIT         = BIT_FLAG(1),    // A08
   TEXTURE_DEPTH_STENCIL_BIT         = BIT_FLAG(2),    // A04
};
typedef uint eTextureUsage;  


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

      eTextureUsage m_usage; // A03
      eRenderMemoryHint m_memoryHint; // A03

      // temp for now - eventually a texture
      // can have multiple different views of it,
      // but in the first assignment
      TextureView* m_ctv;                 // A01
      TextureView* m_srv;                 // A03
}; 