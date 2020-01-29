- hlsl 
- getting source
- compiling
  - entry function
  - shader model
  - byte code
    - reflection
  - linking/creation

## The GPU

So a GPU is like another computer within our computer.   Our `ID3D11DeviceContext` is the current setup of the machine, and we can execute programs on that machine by calling a function like `Draw`.

A context will have all the state it needs for a `Draw` to work, so for example, an extremely simplified context; 

```c

struct DeviceContext
{
   void* input_data; 

   VertexFunc vertex_func; 
   RasterState raster_state; 
   FragmentFunc fragment_func;

   Texture* output;  
};

void Draw( DeviceContext* context, uint vertex_count )
{
   // let us assume we're drawing triangles
   int offset = 0; 
   for (3 vertices in context->input_data up to vertex_count) {
      // output of the vertex stage
      vertex_output vs_out[3]; 

      // get the next three vertices and transform them
      vertex_input* vs_in = context->input_data + offset;
      vs_out[0] = context->vertex_func( vs_in[offset + 0] ); 
      vs_out[1] = context->vertex_func( vs_in[offset + 1] ); 
      vs_out[2] = context->vertex_func( vs_in[offset + 2] );
      offset += 3; 

      // for each pixel in that triangle, we call a pixel shader to get a color      
      foreach (pixel in triangle defined by vs_out) {
         // pixel shader doesn't know aabout the output, and returns a color
         rgba color = context->fragment_func( pixel );

         // color is then put into the output (blended in, coming later)
         output->set_pixel( pixel, color );  
      } 
   }
}

```

Our job is to fill out that context, so `Draw` does work.  

Let us first deal with the functions, and then we'll move onto getting input to them; 


## The Shader
D3D11 uses **High Level Shading Language**, or **HLSL** as its shading language.  See the [Triangle](./triangle.hlsl) or [Default](./default.hlsl) shaders.

This is what we'll be using to write programs for our GPU.  This type of shader is called a **Graphics Shader**, as it is meant for the graphics pipeline.

A `Shader` is made up of multiple stages, like the `vertex` and `fragment` functions above.  We'll call these `ShaderStage`s in code. 

For this class, all graphics shaders will need to support a **Vertex** and **Fragment** shader stage.

In our case, we'll use the term `Shader` to encompass all shader stages, as well as some configurable state having to do with **how** the object is drawn.  

---

## Compiling Shaders

The first step to using shaders is to be able to compile a shader stage.  Stage is code, similar to C++.  So we also have to go through 
the same steps to make it runnable - ie, **Build** and **Link**.  


```cpp

// header needed for compilation
#include <d3dcompiler.h>

//------------------------------------------------------------------------
// What function is the entry point to this stage
static char const* GetEntryForStage( eShaderstage stage ) 
{
   switch (stage) {
      case SHADER_STAGE_VERTEX: return "VertexFunction"; 
      case SHADER_STAGE_FRAGMENT: return "FragmentFunction";
      default: GUARANTEE_OR_DIE(false, "Unknown shader stage"); 
   }
}

//------------------------------------------------------------------------
// what version of the shader language do we want to use - similar to say, C++11 vs C++14
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
```

```cpp
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
```

## Raster State

```cpp

//------------------------------------------------------------------------
void Shader::CreateAndSetDefaultRasterState()
{
	D3D11_RASTERIZER_DESC desc; 

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = TRUE; // the only reason we're doing this; 
	desc.DepthBias = 0U; 
	desc.DepthBiasClamp = 0.0f; 
	desc.SlopeScaledDepthBias = 0.0f; 
	desc.DepthClipEnable = TRUE; 
	desc.ScissorEnable = FALSE; 
	desc.MultisampleEnable = FALSE; 
	desc.AntialiasedLineEnable = FALSE; 

}

void RenderContext::BindShader( Shader* shader ) 
{

	// ID3D11RasterizerState *m_defaultRasterState; 
	m_device->CreateRasterizerState( &desc, &m_defaultRasterState );
	m_context->RSSetState( shader->GetRasterState() ); 
}
```
