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
      // ... new/changed
      bool UpdateInputLayout( BufferLayout const* );              // A06

   public:
      // ...
      BufferLayout const *m_currentLayout          = nullptr;     // A06
      ID3D11InputLayout *m_inputLayout             = nullptr;     // A02

      // ...
}; 

//------------------------------------------------------------------------
// .cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// This is just one such option.
// Could also do something like Shader::GetInputLayoutForBufferLayout(...)
// that would cache all used input layouts with the shader
//
// In practice though, most shaders just end up getting used with the same
// vertex type, so as long as you only create on change, you'll probably get
// fairly similar performance
bool Shader::UpdateInputLayout( Bufferlayout const *layout ) 
{
   if (layout == m_currentLayout) {
      // don't do anything - already valid;
      return true; 
   }

   // free old layout
   DX_SAFE_RELEASE( m_inputLayout ); 

   // Fill a new input layout description using the buffer layout
   // ... EXCERCISE TO THE READER ...

   // Create the new input layout...
   // ...

   // cache this off so we can prevent recreating it next time; 
   m_currentLayout = layout; 
   return true; 
}