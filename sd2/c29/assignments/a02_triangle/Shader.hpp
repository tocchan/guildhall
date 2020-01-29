
//------------------------------------------------------------------------
// Shader.hpp
//------------------------------------------------------------------------
enum eShaderStage
{
   SHADER_STAGE_VERTEX, 
   SHADER_STAGE_FRAGMENT, 
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
class ShaderStage
{
   public:
      ShaderStage(); 
      ~ShaderStage(); 

      // A02
      bool LoadShaderFromSource( RenderContext *ctx, 
         std::string const &filename, 
         void const *source, 
         size_t const sourceByteLen, 
         eShaderStage stage );

      inline bool IsValid() const { return m_handle != nullptr; }

   public:
      // a stage could be any one of these types,
      // so we use a enum to identify it, and a union to store it; 
      eShaderStage m_stage; 
      union {
         ID3D11Resource *m_handle; 
         ID3D11VertexShader *m_vs; 
         ID3D11PixelShader *m_fs; 
      };
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Shader 
{
   public:
      bool CreateFromFile( RenderContext *ctx, std::string const &filename );    // A02

      // for hooking IA (input assembler) to the VS (vertex shader), 
      // needs to vertex shader and vertex format to make the binding
      ID3D11InputLayout* GetOrCreateInputLayout( VertexBuffer* vbo );            // A02

      ID3D11RasterizerState* GetRasterState();                                   // A02 (only creating a default one for now)


   public:
      ShaderStage m_vertex_shader; 
      ShaderStage m_pixel_shader; 

      // A02 temp
      ID3D11InputLayout* m_inputLayout = nullptr; // for now, we'll have 1, but in the future you could have one for each different vertex type you use with this; 
      ID3D11RasterizerState* m_defaultRasterState = nullptr; 
}; 