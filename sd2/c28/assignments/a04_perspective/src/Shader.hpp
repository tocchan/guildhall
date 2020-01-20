//------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------

//------------------------------------------------------------------------
enum eShaderStage
{
   SHADER_STAGE_VERTEX, 
   SHADER_STAGE_FRAGMENT, 
};


//------------------------------------------------------------------------
class Shader         // A01, A02, A03
{
   public:
      // Be sure to clear the state; 
      Shader(); 
      ~Shader();                                                  // A01, A02, A03, A04

      // This now needs to also support the ".shader" or ".xml" extension (depending on which you want to use)
      bool CreateFromFile( RenderContext *ctx, std::string const &filename );    // A01, A04

      //...

      // Can now set our depth compare mode; 
      void SetDepth( eCompareOp op, bool write );                 // A04

   public: // Internal, so could be private as friend; 
      //...
      // Depth stencil state now also needs to be generated; 
      bool UpdateStatesIfDirty();                                 // A04

   public:
      // ...
         
      // Depth/Stencil Mode
      eCompareOp m_depthCompareOp            = COMPARE_LEQUAL;    // A04
      bool m_writeDepth                      = false;             // A04

      //...
      ID3D11DepthStencilState *m_depthStencilState = nullptr;     // A04

      bool m_blendStateDirty                 = true;              // A03
      bool m_depthStateDirty                 = true;              // A04
}; 

//------------------------------------------------------------------------
// Shader.cpp
//------------------------------------------------------------------------


//------------------------------------------------------------------------
bool Shader::UpdateStatesIfDirty() 
{
   // blend state
   // ...

   // depth stencil
   if (m_depthStateDirty || (m_depthStencilState == nullptr)) {
      D3D11_DEPTH_STENICL_DESC ds_desc = {};

      ds_desc.DepthEnable = TRUE;  // for simplicity, just set to true (could set to false if write is false and comprae is always)
      ds_desc.DepthWriteMask = m_writeDepth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO; 
      ds_desc.DepthFunc = DXGetCompareFunc( m_depthCompareOp ); //  

      // Stencil - just use defaults for now; 
      ds_desc.StencilEnable = false; 
      ds_desc.StencilReadMask = 0xFF; 
      ds_desc.StencilWriteMask = 0xFF; 

      D3D11_DEPTH_STENCILOP_DESC default_stencil_op = {}; 
      default_stencil_op.StencilFailOp = D3D11_STENCIL_OP_KEEP;      // what to do if stencil fails
      default_stencil_op.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // What to do if stencil succeeds but depth fails
      default_stencil_op.StencilPassOp = D3D11_STENCIL_OP_KEEP;      // what to do if the stencil succeeds
      default_stencil_op.StencilFunc = D3D11_COMPARISON_ALWAYS;      // function to test against

      // can have different rules setup for front and backface
      ds_desc.FrontFace = default_stencil_op; 
      ds_desc.BackFace = default_stencil_op; 

      DX_SAFE_RELEASE(m_depthStencilState); 
      dx->CreateDepthStencilState( &ds_desc, &m_depthStencilState ); 
      m_depthStateDirty = false; 
   }
}
