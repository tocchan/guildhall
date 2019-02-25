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
      eCompareOp m_compreOp                  = COMPARE_LEQUAL;    // A04
      bool m_writeDepth                      = false;             // A04

      //...
      ID3D11DepthStencilState *m_depthStencilState = nullptr;     // A04

      bool m_blendStateDirty                 = true;              // A03
      bool m_depthStateDirty                 = true;              // A04
}; 

