//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      RenderContext();                 // A01
      ~RenderContext();                // A01

      // APP CONTROL
      void Startup();                  // A01, A02, A03
      void Shutdown();                 // A02, A02, A03

      void BeginFrame();               // A01
      void EndFrame();                 // A01

      // DRAWING
      void BeginCamera( Camera* );     // A01, A02, A03, A04, A05
      void EndCamera();                // A01

      void ClearColorTargets( rgba const &color ); // A01
      void ClearDepthStencilTarget( float depth = 1.0f, uint8_t stencil = 0U ); 

      // State Binding
      void BindShader( Shader *shader );  // A01

      // Input Binding
      void BindVertexBuffer( 

      // Resource Binding


      void Draw( uint vertexCount, uint byteOffset = 0U );        // A01
      void DrawIndexed( uint indexCount, uint byteOffset = 0U );  // A04

      // New Methods
      void BindTextureView( uint slot, TextureView *view ); 
      void BindSampler( uint slot, Sampler *sampler ); 

      // Some convenience
      void BindTextureView( uint slot, std::string const &name ); 
      void BindSampler( eSampleMode mode );

      // more convience - I store samplers WITH a texture view for convinience (what
      // is the desired way to sample this texture).  
      // (Personal Note: I like storing the Sampler on the shader, as the sampling usually has to do with the 
      // effect I want to achieve, but that seems to be a fairly uncommon practice)
      
      // sampler I'm storing with the view in this design - but still giving
      // the context the option of binding a view with a different sampler if we so choose; 
      void BindTextureViewWithSampler( uint slot, TextureView *view ); 
      void BindTextureViewWithSampler( uint slot, std::string const &name ); 
      void BindTextureViewWithSampler( uint slot, TextureView *view, Sampler *sampler ); 
      void BindTextureViewWithSampler( uint slot, TextureView *view, eSampleMode mode ); 
      void BindTextureViewWithSampler( uint slot, std::string const &name, eSampleMode mode ); 


   public:
      Shader* GetOrCreateShader( std::string const &filename );                // A01
      TextureView2D* GetOrCreateTextureView2D( std::string const &filename );  // A03

   public:
      // D3D11 Handles
      ID3D11Device *m_device;             // A01
      ID3D11DeviceContext *m_context;     // A01
      ID3D11SwapChain *m_swapchain;       // A01

      // Stateful Data
      ColorTargetView *m_frameBackbuffer;    // A01
      Camera *m_currentCamera;               // A01

      // Immediate Drawing Utility
      VertexBuffer *m_immediateVBO;       // A02 

      // cached resources for convenience; 
      Sampler* m_cachedSamplers[SAMPLE_MODE_COUNT];   // A03

      // also change your texture database; 
      // ...


}; 

//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------


// raw character string
static char const* DEFAULT_HLSL_SHADER = R"(
   
// shader code; 

   )"; 

//------------------------------------------------------------------------
void RenderContext::Startup()
{
   // ...

   // **NEW**
   Sampler *point = new Sampler(); 
   point->SetFilterModes( FILTER_MODE_LINEAR, FILTER_MODE_POINT );  // still min linear - to get blending as it gets smaller; 
   m_cachedSamplers[SAMPLE_MODE_POINT] = point; 

   Sampler *linear = new Sampler(); 
   linear->SetFilterModes( FILTER_MODE_LINEAR, FILTER_MODE_LINEAR ); 
   m_cachedSamplers[SAMPLE_MODE_LINEAR] = linear; 

   // premake defaults 
   Image image;
   image.Create( 1, 1 ); 
   image.SetPixel( 0, 0, RGBA::WHITE ); 
   TextureView2D *white = TextureView2D::ForImage( image ); 
   RegisterTextureView2D( "white", white ); 
}

//------------------------------------------------------------------------
void RendreContext::Shutdown()
{
   // ...

   // **NEW** 
   // Free all Samplers
   IMPLEMENT_ME!
}

//------------------------------------------------------------------------
void RenderContext::BindShader( Shdaer *shader )
{
   // ...
   
   // NEW
   shader->UpdateBlendStateIfDirty(); 
   float black[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
   m_context->OMSetBlendState( shader->m_d3d11BlendState, // the d3d11 blend state object; 
      black,         // blend factor (some blend options use this) 
      0xffffffff );  // mask (what channels will get blended, this means ALL)  
}

//------------------------------------------------------------------------
// Note:  We have 128 texture slots, and 16 sampler slots
// But for simplicity, the system is designed to have the sampler stored
// with the texture view (so users just have to say "BindTexture").  
// Meaning, this design limits us to 16 textures.  
void RenderContext::BindTextureView( uint slot, TextureView *view )
{
   ID3D11ShaderResourceView *srv = nullptr; 
   if (view != nullptr) {
      srv = view->GetHandle(); 
   } else {
      srv = GetOrCreateTextureView2D("white"); 
      // TODO - if view is nullptr, default to something
      // that makes since for the slot
      // For now - bind a solid "WHITE" texture if slot == 0; 
   }

   // You can bind textures to the Vertex stage, but not samplers
   // We're *not* for now since no effect we do at Guildhall requires it, but
   // be aware it is an option; 
   // m_context->VSSetShaderResource( slot, 1U, &srv ); 

   m_context->PSSetShaderResource( slot, 1U, &srv );
}

//------------------------------------------------------------------------
void RenderContext::BindSampler( uint slot, Sampler *sampler ) 
{
   if (sampler == nullptr) {
      sampler = m_cachedSamplers[SAMPLE_MODE_DEFAULT]; // bind the default if nothing is set
   }

   // create the dx handle; 
   sampler->CreateStateIfDirty(this);

   ID3D11SamplerState *handle = sampler->GetHandle(); 
   m_context->PSSetSampler( slot, 1U, &handle ); 
}

//------------------------------------------------------------------------
void RenderContext::BindTextureViewWithSampler( uint slot, TextureView *view )
{
   BindTextureView( view ); 

   if (view != nullptr) {
      BindSampler( view->GetSampler() );
   } else {
      BindSampler( nullptr ); 
   }
}


//------------------------------------------------------------------------
// (NOTE: This design is fairly different from my Engine, 
// so while I'm fairly sure this should work, if it doesn't, please let me know)
TextureView2D* RenderContext::GetOrCreateTextureView2D( std::string const &filename )
{
   TextureView2D *view = nullptr; 

   // normal stuff - if it exists, return it; 
   auto item = m_cachedTextureViews.find(filename); 
   if (item != m_cachedTextureViews.end()) {
      return item->second; 
   } 

   Texture2D *tex = new Texture2D(this); 
   if (!tex->LoadFromFile( filename ))) {
      delete tex;

      // optional -> store a null at this spot so we don't 
      // bother trying to reload it later; 
      m_cachedTextureViews[filename] = nullptr;

      return nullptr;

   } else {
      // create the view
      TextureView2D *view = tex->CreateTextureView2D(); 

      // THIS SHOULD BE FINE - the view should hold onto the D3D11 resource;
      // (I'm not 100% on this though, so if not, please Slack me)
      delete tex;  

      m_cachedTextureViews[filename] = view; 

      return view; 
   }
}

