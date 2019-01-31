
//------------------------------------------------------------------------
// Shader.hpp
//------------------------------------------------------------------------
enum eShaderStage
{
   SHADER_STAGE_VERTEX, 
   SHADER_STAGE_FRAGMENT, 
};


//------------------------------------------------------------------------
class ShaderStage
{
   public:
      ShaderStage(); 
      ~ShaderStage(); 

      bool LoadShaderFromSource( RenderContext *ctx, 
         std::string const &filename, 
         void const *source, 
         size_t const sourceByteLen, 
         eShaderStage stage );

      inline bool IsValid() const { return m_handle != nullptr; }

   public:
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
      bool CreateFromFile( RenderContext *ctx, std::string const &filename ); 

   public:
      ShaderStage m_vertex_shader; 
      ShaderStage m_pixel_shader; 
}; 

//------------------------------------------------------------------------
// Shader.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static char const* GetEntryForStage( eShaderstage stage ) 
{
   switch (stage) {
      case SHADER_STAGE_VERTEX: return "VertexFunction"; 
      case SHADER_STAGE_FRAGMENT: return "FragmentFunction";
      default: GUARANTEE_OR_DIE(false, "Unknown shader stage"); 
   }
}

//------------------------------------------------------------------------
static char const* GetShaderModelForStage( eShaderstage stage ) 
{
   switch (stage) {
      case SHADER_STAGE_VERTEX: return "vs_5_0"; 
      case SHADER_STAGE_FRAGMENT: return "ps_5_0";
      default: GUARANTEE_OR_DIE(false, "Unknown shader stage"); 
   }
}

//------------------------------------------------------------------------
static ID3DBlob* CompileHLSLToByteCode( char const *opt_filename,  // optional: used for error messages
   void const *source_code,                                          // buffer containing source code.
   size_t const source_code_size,                                    // size of the above buffer.
   char const *entrypoint,                                           // Name of the Function we treat as the entry point for this stage
   char const* target  )                                             // What stage we're compiling for (Vertex/Fragment/Hull/Compute... etc...)
{
   /* DEFINE MACROS - CONTROLS SHADER
   // You can optionally define macros to control compilation (for instance, DEBUG builds, disabling lighting, etc...)
   D3D_SHADER_MACRO defines[1];
   defines[0].Name = "TEST_MACRO";
   defines[0].Definition = nullptr;
   */

   DWORD compile_flags = 0U;
   #if defined(DEBUG_SHADERS)
      compile_flags |= D3DCOMPILE_DEBUG;
      compile_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
      compile_flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;   // cause, FIX YOUR WARNINGS
   #else 
      // compile_flags |= D3DCOMPILE_SKIP_VALIDATION;       // Only do this if you know for a fact this shader works with this device (so second run through of a game)
      compile_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // Yay, fastness (default is level 1)
   #endif

   ID3DBlob *code = nullptr;
   ID3DBlob *errors = nullptr;

   HRESULT hr = ::D3DCompile( source_code, 
      source_code_size,                   // plain text source code
      opt_filename,                       // optional, used for error messages (If you HLSL has includes - it will not use the includes names, it will use this name)
      nullptr,                            // pre-compiler defines - used more for compiling multiple versions of a single shader (different quality specs, or shaders that are mostly the same outside some constants)
      D3D_COMPILE_STANDARD_FILE_INCLUDE,  // include rules - this allows #includes in the shader to work relative to the src_file path or my current working directly
      entrypoint,                         // Entry Point for this shader
      target,                             // Compile Target (MSDN - "Specifying Compiler Targets")
      compile_flags,                      // Flags that control compilation
      0,                                  // Effect Flags (we will not be doing Effect Files)
      &code,                              // [OUT] ID3DBlob (buffer) that will store the byte code.
      &errors );                          // [OUT] ID3DBlob (buffer) that will store error information

   if (FAILED(hr) || (errors != nullptr)) {
      if (errors != nullptr) {
         char *error_string = (char*) errors->GetBufferPointer();
         Logf( "Failed to compile [%s].  Compiler gave the following output;\n%s", 
            opt_filename, 
            error_string );
         DX_SAFE_RELEASE(errors);
      } else {
         Logf( "Failed with HRESULT: %u", hr ); 
      }
   } 

   // will be nullptr if it failed to compile
   return code;
}

//------------------------------------------------------------------------
// Just initialize handle null; 
ShaderStage::ShaderStage()
   : m_handle(nullptr) 
{
}

//------------------------------------------------------------------------
ShaderStage::~ShaderStage()
{
   // cleanup the D3D Resource
   DX_SAFE_RELEASE(m_handle); 
}

//------------------------------------------------------------------------
bool ShaderStage::LoadShaderFromSource( RenderContext *ctx, 
   std::string const &filename, 
   void *source,
   size_t source_len,
   eShaderStage stage )
{
   // The devie is what is used to 
   // create resources, so lets cache it off
   // for use in this method; 
   ID3DDevice *device = ctx->m_device; 

   // save the stage; 
   m_stage = stage; 

   // entry point is what function to call for this stage
   char const *entrypoint = GetEntryForStage(stage); 

   // target is the shader model to compile against (determines feature set)
   // we'll be using shader model 5.0 in this class; 
   char const *target = GetShaderModelForStage(stage); 

   // take the source and convert it to byte code;
   // note: this byte code can be saved to disk and loaded from
   // instead of having to recompile the shader - this is 
   // usually done during shader-caching (or done at ship for D3D titles)
   ID3DBlob *bytecode = HLSLCompileToByteCode( filename, source, source_len, entrypoint, target ); 
   if (bytecode == nullptr) {
      return false; 
   }

   // based on the stage; create the appropriate D3D object; 
   switch (stage) {
      case SHADER_STAGE_VERTEX:    // Compile the byte code to the final shader (driver/hardware specific program)
         device->CreateVertexShader( bytecode->GetBufferPointer(), 
            bytecode->GetBufferSize(), 
            nullptr, 
            &m_vs );
         break;

      case SHADER_STAGE_FRAGMENT: 
         device->CreatePixelShader( bytecode->GetBufferPointer(), 
            bytecode->GetBufferSize(), 
            nullptr, 
            &m_fs );
         break; 
      }
   }

   // we're done with the byte code.
   // later, when doing vertex buffers, we'll need
   // to keep the bytecode of the vertex shader around
   DX_SAFE_RELEASE(byte_code); 

   // if we have a handle, this succeeded; 
   return IsValid();
}
//------------------------------------------------------------------------
// Takes an *.hlsl file and loads
// a shader.  Eventually this optionally take
// in a *.xml file for additional state 
bool Shader::CreateFromFile( RenderContext *ctx, std::string const &filename ) 
{
   size_t buffer_len = 0U; 
   void *buffer = CreateBufferForFile( &buffer_len, filename ); 

   m_vertexShader.LoadShaderFromSource( ctx, filename, buffer, buffer_len, SHADER_STAGE_VERTEX );
   m_pixelShader.LoadShaderFromSource( ctx, filename, buffer, buffer_len, SHADER_STAGE_FRAGMENT ); 

   delete buffer; 

   return m_vertexShader.IsValid()
      && m_pixelShader.IsValid(); 
}



