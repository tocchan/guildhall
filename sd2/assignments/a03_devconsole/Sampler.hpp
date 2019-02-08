//------------------------------------------------------------------------
// Sampler.hpp
//------------------------------------------------------------------------

class Sampler
{
   public:
      Sampler();
      ~Sampler();  // be sure to clean up; 

   public:
      // called by the device when bound to lazily 
      // create the state object the first time, or if
      // it has changed (which is rare)
      void CreateStateIfDirty( RenderContext *ctx ); 

      inline void SetFilterModes( eFilterMode min, eFilterMode mag )    { m_minFilter = min; m_magFilter = mag; m_isDirty = true; }

      ID3D11SamplerState* GetHandle() const                             { return m_handle; }

   public:
      ID3D11SamplerState *m_handle  = nullptr; 

      // state we care about
      eFilterMode m_minFilter       = FILTER_MODE_POINT; // what to do when "shrinking" an image, or a pixel on screen represents multiple texels.
      eFilterMode m_magFilter       = FILTER_MODE_POINT; // what to do when "stretching" an image, or when multiple pixels on screen fit in the same texel; 
      // eFilterMode m_mipFilter    = FILTER_MODE_POINT; // TODO when we implement mipmaps later; 

      // there are more options, we'll introduce them as we need them
      // ...wrap
      // ...mip clamps & bias
      // ...compare samplers
       
      bool m_isDirty                = true; // marked when state has changed so we know to recreate; 
}; 

//------------------------------------------------------------------------
// Sampler.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static D3D11_FILTER DXGetFilter( eFilterMode min, eFilterMode mag ) 
{
   if (min == FILTER_MODE_POINT) {
      swtich (mag) {
         case FILTER_MODE_POINT:  return D3D11_FILTER_MIN_MAG_MIP_POINT; 
         case FILTER_MODE_LINEAR: return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT; 
      }; 
   } else {
      swtich (mag) {
         case FILTER_MODE_POINT:  return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT; 
         case FILTER_MODE_LINEAR: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT; 
      };
   }

   ASSERT(false); // this combination was not handled?  
   return D3D11_FILER_MIN_MAG_MIP_POINT; 
}

//------------------------------------------------------------------------
void Sampler::CreateStateIfDirty( RenderContext *ctx )
{
   // no changes needed
   if (false == m_isDirty) {
      return; 
   }

   // Release old state; 
   DX_SAFE_RELEASE( m_handle ); 

   // Create new state
   D3D11_SAMPLER_DESC desc;
   MemZero( &desc ); 

   desc.Filter = DXGetFilter( m_minFilter, m_magFilter );
   desc.MaxAnisotropy = 1U; // anistropic filtering (we're not using this... yet)

   // set texture to wrap on UV, but clamp on W (mip).  
   // (if you're smapling 3D wrapping noise, you may want wrap in all dimensions)
   // (if you're doing single sprites, you may want clamp all so you don't get edge bleeding)
   desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;  
   desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; 
   desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

   // set tha tcan can sample from all mip levels; 
   desc.MinLOD = -FLT_MAX; 
   desc.MaxLOD = FLT_MAX;   
   desc.MipLODBias = 0.0f;  // will shift the mip level for a sample by this

   desc.CompareisonFunc = D3D11_COMPARISON_NEVER; // will come into play doing shadow maps; 

   // leaving border color black (only useful if our wrap mode is BORDER

   // Create!
   ctx->Create( &desc, &m_handle ); 

   m_isDirty = false; 
   ASSERT( m_handle != nullptr );   // this should honestly never fail.  If it does - check your output!
}; 