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
class ShaderStage                         // A01, A02
{
   public:
      ShaderStage();                      // A01
      ~ShaderStage();                     // A01

      bool LoadShaderFromSource( RenderContext *ctx, 
         std::string const &filename, 
         void const *source, 
         size_t const sourceByteLen,      
         eShaderStage stage );            // A01, A02

      bool IsValid() const;               // A01

   public:
      eShaderStage m_stage;               // A01
      union {
         ID3D11Resource *m_handle;        // A01
         ID3D11VertexShader *m_vs;        // A01
         ID3D11PixelShader *m_fs;         // A01
      };

      ID3DBlob *m_byteCode;               // A02 
}


// other options - fully expose the options; 
// enum eBlendOp { ... }; // maps to https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend_op
// enum eBlendFactor { ... }; // maps to https://docs.microsoft.com/en-us/windows/desktop/api/d3d11/ne-d3d11-d3d11_blend
// struct blend_info_t { eBlendOp op; eBlendFactor srcFactor;  eBlendFactor dstFactor; }; 

//------------------------------------------------------------------------
class Shader         // A01, A02, A03
{
   public:
      Shader();                                                   // A01
      ~Shader();                                                  // A01, A02, A03, A05

      bool CreateFromFile( RenderContext *ctx, std::string const &filename );    // A01, A04
      bool IsValid() const;                                       // A01

      void SetBlendMode( eBlendMode mode );                       // A03
      void SetDepth( eCompareOp op, bool write );                 // A04

   public: // Internal, so could be private as friend; 
      bool CreateInputLayoutForVertexPCU();                       // A02 
      bool CreateInputLayoutForVertex( BufferLayout const* );     // A05 
      bool UpdateStatesIfDirty();                                 // A03, A04

   public:
      ShaderStage m_vertexStage;                                  // A01
      ShaderStage m_pixelStage;                                   // A01

      // BlendMode
      eBlendMode m_blendMode                 = BLEND_MODE_ALPHA;  // A03
      // blend_info_t m_colorBlend; 
      // blend_info_t m_alphaBlend; // eBlendOp m_blendOp; 

      // Depth/Stencil Mode
      eCompareOp m_compreOp                  = COMPARE_OP_LEQUAL; // A04
      bool m_writeDepth                      = false;             // A04

      ID3D11InputLayout *m_inputLayout             = nullptr;     // A02
      ID3D11BlendState *m_blendState               = nullptr;     // A03
      ID3D11DepthStencilState *m_depthStencilState = nullptr;     // A04

      bool m_blendStateDirty                 = true;              // A03
}; 

