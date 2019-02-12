//------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------

// two options for this - either you can name common blend mode configurations
// using an enum, or you can fully expose it using a color and alpha blend operation; 
// Believe the enum is what Squirrel is intended for your engines; 
enum eBlendMode
{
                        // op,  src_factor,  dst_factor
   BLEND_MODE_OPAQUE,   // ADD, ONE,         ZERO              (or blend disabled)
   BLEND_MODE_ALPHA,    // ADD, SRC_ALPHA,   INV_SRC_ALPHA,
   BLEND_MODE_ADDTIVE,  // ADD, ONE,         ONE
}; 

// other options - fully expose the options; 
// enum eBlendOp { ... }; // maps to https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend_op
// enum eBlendFactor { ... }; // maps to https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend
// struct blend_info_t { eBlendOp op; eBlendFactor srcFactor;  eBlendFactor dstFactor; }; 

//------------------------------------------------------------------------
class Shader
{
   public:
      // !! DON'T FORGET -> update deconstructor to free newly added inputLayout

      // old stuff...
      //...

      // **NEW STUFF**
      void SetBlendMode( eBlendMode mode ); 

      bool UpdateBlendStateIfDirty(); 

   public:
      ShaderStage m_vertexStage;
      ShaderStage m_fragmentStage; 

      // blend state - not sure how your SD1 exposes this.  Some Engines expose the blend factors
      // and some engines will expose an enum for common setups (like alpha, additive, etc...)

      eBlendMode m_blendMode                 = BLEND_MODE_ALPHA; 
      // blend_info_t m_colorBlend; 
      // blend_info_t m_alphaBlend; // eBlendOp m_blendOp; 

      bool m_blendStateDirty                 = true; 

      
      ID3D11InputLayout *m_inputLayout       = nullptr; 
      ID3D11BlendState *m_blendState         = nullptr; 


}; 

//------------------------------------------------------------------------
// Shader.cpp
//------------------------------------------------------------------------


//------------------------------------------------------------------------
bool Shader::UpdateBlendStateIfDirty( RenderContext *ctx )
{
   // if we already have a good blend state - keep it; 
   if ((m_blendState != nullptr) && (false == m_blendStateDirty)) {
      return true; 
   }

   // Free old state
   DX_SAFE_RELEASE(m_blendState); 

   // otherwise, let's make one; 
   D3D11_BLEND_DESC desc;
   MemZero( &desc ); 

   desc.AlphaToCoverageEnable = false;  // used in MSAA to treat alpha as coverage (usual example is foliage rendering, we will not be using this)
   desc.IndepdentBlendEnable = false;   // if you have multiple outputs bound, you can set this to true to have different blend state per output

   // Blending is setting put the equation...
   // FinalColor = BlendOp( SrcFactor * outputColor, DestFactor * destColor )
   // where outputColor is what the pixel shader outputs
   // and destColor is the color already in the pixel shader
   
   // the below describes the equation...
   // FinalColor = outputColor.a * outputColor + (1.0f - outputColor.a) * destColor;  

   // since we disabled independent blend, we only have to setup the first blend state
   // and I'm setting it up for "alpha blending"
   desc.RenderTarget[0].BlendEnable = TRUE;  // we want to blend

   if (e_blendMode == BLEND_MODE_ALPHA) {
      desc.RenderTarget[0].SrcBlend    = D3D11_BLEND_SRC_ALPHA;      // output color is multiplied by the output colors alpha and added to...
      desc.RenderTarget[0].DestBlend   = D3D11_BLEND_INV_SRC_ALPHA;  // the current destination multiplied by (1 - output.a); 
      desc.RenderTarget[0].BlendOp     = BD3D11_BLEND_OP_ADD;        // we add the two results together
   
      // you can compute alpha seperately, in this case, we'll just set it to be the max alpha between the src & destination
      desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
      desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; 
      desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_MAX;
   } else { 
      // TODO: Add else_if branches for the other blend modes; 
      ASSERT(false); // Unimplemented blend mode; 
                     // probably need to add the other cases
   };

   desc.RenderTarget[0].RenderTargetWriteMask       = D3D11_COLOR_WRITE_ENABLE_ALL;  // can mask off outputs;  we won't be doing that; 

   // Finally, create the blend state
   ID3D11Device *dev = ctx->m_device; 
   dev->CreateBlendState( desc, &m_blendState );

   m_blendStateDirty = false; 
   return (m_blendState != nullptr); 
}