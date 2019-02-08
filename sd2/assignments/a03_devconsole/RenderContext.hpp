//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

// As the engine designer, we're going to make the decision to 
// just reserve certain slot indices for specific constant buffers
// We will gruop uniform buffers by how often they change, and for now, we
// really only use the CAMERA
enum eCoreUniformSlot 
{
   UNIFORM_SLOT_FRAME         = 1,
   UNIFORM_SLOT_CAMERA        = 2, 
}; 

// **NEW**
// A enum to pick a samplerstate to use
enum eSampleMode
{
   SAMPLE_MODE_POINT       = 0, 
   SAMPLE_MODE_LINEEAR, 
   // SAMPLE_MODE_BILINEAR

   SAMPLE_MODE_COUNT,
   SAMPLE_MODE_DEFAULT     = SAMPLE_MODE_LINEAR; // different games may want to change this.  My projects will use Linear, SD4 will want point; 
}; 

// A constant buffer can address up 64KB,
// with 15 available slots (docs usually say 14, as one gets used
// up by the $GLOBAL constant buffer; 

//------------------------------------------------------------------------
// FRAME data is information that is constant for an entire
// frame - you can update this UBO during RenderContext::BeginFrame, 
// as well as bind it (as long as you don't accidently bind over it
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// I usually use this to store time, as it is a common
// input for shader effects
// A UBO should usually be a multipole of 16-bytes large, so we'll just add some 
// variations of time for our shaders to use; 
// slot: 1
struct frame_buffer_t
{
   float time; 
   float halfTime; 
   float doubleTime; 
   float quadTie; 
}; 

//------------------------------------------------------------------------
// slot 2; 
// What data does the camera provide the shader (slot:2)
// This must follow a 16-byte element alignemnt (for now, this only contains
// ortho information; 
// camera slot I usually bind all constants that change per view; 
struct camera_buffer_t 
{
   mat44 projection; 
};

// I start at slot 1 out of habit.  I reserve slot 0 for what I call the "SYTEM" buffer, which
// is usually just debug constants and configurations as I'm working. 
// The uniform buffer system is very free-form, you can bind any data to any slot
// but I like to usually have my engine renderer reserve a few slots
// grouped by update frequency.


//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      //...

      // Updated methods
      void Startup();
      void Shutdown(); 
      void BindShader( Shader *shader );

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
      void BindTextureViewWithSampler( uint slot, TextureView *view, Sampler *sampler ); 
      void BindTextureViewWithSampler( uint slot, TextureView *view, eSampleMode mode ); 
      void BindTextureViewWithSampler( uint slot, std::string const &name, eSampleMode mode ); 


   public:
      // changed methods

      // IMPORTANT: This REPLACES GetOrCreateTexture
      // Since we usually want just views of a texture
      TextureView2D* GetOrCreateTextureView2D( std::string const &filename ); 

   public:
      //...

      // *new* members
      VertexBuffer *m_immediateVBO; 

      // There are a small set of sampler state that
      // we can get by with just reusing everywhere, 
      // so instead of creating a sampler per texture, 
      // we'll pull from this store; 
      Sampler* m_cachedSamplers[SAMPLE_MODE_COUNT]; 

      // also change your texture database; 
      // ...


}; 

//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------


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
}

//------------------------------------------------------------------------
void RendreContext::Shutdown()
{
   // ...

   // **NEW** 
   // Free all Samplers
   IMPLEMENT_ME!
}

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

