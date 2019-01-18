//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// New Content will be surrounded by this
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "demo.h"
#include "log.h"

#include "window.h"

// D3D Stuff
// Required Headers
#include <d3d11.h>  
#include <DXGI.h>    

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )

// DEBUG STUFF
#include <dxgidebug.h>
// #pragma comment( lib, "dxguid.lib" )

// NEEDED FOR COMPILING
// Note:  This is not allowed for Windows Store Apps.
// Shaders must be compiled to byte-code offline. 
// but for development - great if the program knows how to compile it.
#include <d3dcompiler.h>
#pragma comment( lib, "d3dcompiler.lib" )

//------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------
#define WINDOW_TITLE    "D3D11 SHADERS"
#define WINDOW_RES_X    (1280)
#define WINDOW_RES_Y    (720)

#define RENDER_DEBUG

typedef unsigned int uint;

//------------------------------------------------------------------------
// MACROS
//------------------------------------------------------------------------
#define SAFE_DELETE(ptr)   if ((ptr) != nullptr) { delete ptr; ptr = nullptr }
#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

//------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------
struct vec3
{
   float x; 
   float y;
   float z;

   vec3() 
      : x(0.0f)
      , y(0.0f)
      , z(0.0f)
   {}
   vec3( float _x, float _y, float _z ) 
      : x(_x)
      , y(_y)
      , z(_z) 
   {}
};

struct vertex_t 
{
   vec3 position;

   vertex_t() {}
   vertex_t( vec3 const &p ) 
      : position(p) 
   {}
};

//------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------
static HWND gHWND = NULL;

ID3D11Device *gD3DDevice = nullptr;
ID3D11DeviceContext *gD3DContext = nullptr;
IDXGISwapChain *gD3DSwapChain = nullptr;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Multiple functions now use this - so need to store it for the frame
// [also, need to clean it up at the end of the app] 
ID3D11RenderTargetView *gD3DRenderTargetView = nullptr;
ID3D11VertexShader *gD3DVertexShader = nullptr;
ID3D11PixelShader *gD3DFragmentShader = nullptr;
ID3D11InputLayout *gD3DInputLayout = nullptr;
ID3D11Buffer *gD3DVertexBuffer = nullptr;


//------------------------------------------------------------------------
// Declare functions this demo uses
//------------------------------------------------------------------------

void* FileReadToBuffer( char const *filename, size_t *out_size );
ID3DBlob* CompileHLSLToShaderBlob( char const *opt_filename,         // optional: used for error messages
   void const *source_code,                                          // buffer containing source code.
   size_t const source_code_size,                                    // size of the above buffer.
   char const *entrypoint,                                           // Name of the Function we treat as the entry point for this stage
   char const* target );                                             // What stage we're compiling for (Vertex/Fragment/Hull/Compute... etc...)

void DemoRender();            // Does rendering for this demo
void DemoRun();               // Demo setup and update loop

//------------------------------------------------------------------------
// Code not pertinant to the topic of this demo is moved here
//
// I will be swapping things in and out as I add/remove steps.
//------------------------------------------------------------------------
#include "demo.inl"


//------------------------------------------------------------------------
// D3D11 STUFF
//------------------------------------------------------------------------

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Moving this to its own function for cleanliness
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CreateBackBufferView()
{
    // Get the back buffer
   ID3D11Texture2D *back_buffer = nullptr;
   gD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

   // Get a render target view of this
   // NOTE:  This could be cached off and stored instead of creating
   // a new one each frame.  It is fairly cheap to do though.
   gD3DDevice->CreateRenderTargetView( back_buffer, nullptr, &gD3DRenderTargetView );
   DX_SAFE_RELEASE(back_buffer);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Helper - File IO Example - Read an entire file to a buffer
// NOTE:  Be sure for free it when you're done.
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void* FileReadToBuffer( char const *filename, size_t *out_size )
{
   *out_size = 0U;

   // I'm going to use Windows file functions for no particular reason
   // outside of familiarity.  Feel free to use whatever. 
   DWORD file_access = GENERIC_READ;
   DWORD share = FILE_SHARE_READ;
   DWORD options = OPEN_EXISTING;

   HANDLE file_handle = CreateFileA( filename, file_access, share, NULL, options, FILE_ATTRIBUTE_NORMAL, NULL );
   if (file_handle == INVALID_HANDLE_VALUE) {
      Logf( "File [%s] could not be opened.", filename );
      return nullptr;
   }
   
   // Get File Size
   DWORD high_word;
   DWORD lo_word = ::GetFileSize( file_handle, &high_word );
   
   // 64-bit way - use a 64-bit type, but we will never have a 4GB+ file, so ignore it and just using lo-word to prevent warnings
   // size_t size = ((size_t)high_word << 32) | (size_t)lo_word;
   size_t size = (size_t)lo_word;

   // Make a buffer that big
   void *buffer = malloc(size + 1U); // I allocae one extra for a null terminator - but do not include it in the size
                                     // just in the cases I load strings it null terminates.
   if (nullptr != buffer) {
      DWORD read = 0U;
      ReadFile( file_handle, buffer, (DWORD)size, &read, nullptr );
      *out_size = read;

      ((char*)buffer)[size] = NULL;
   }

   CloseHandle( (HANDLE)file_handle );
   return buffer;
}

#if defined(_DEBUG)
   #define DEBUG_SHADERS
#endif

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// "vs_5_0";
// "ps_5_0";
static ID3DBlob* CompileHLSLToShaderBlob( char const *opt_filename,  // optional: used for error messages
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
      }
   } 

   // will be nullptr if it failed to compile
   return code;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void CreateShaders( char const *filename ) 
{
   size_t size = 0U;
   void *source_code = FileReadToBuffer( filename, &size );
   if (source_code == nullptr) {
      return;
   }

   // Compile hlsl to byte code (generic compiled version)
   ID3DBlob *vs_bytecode = CompileHLSLToShaderBlob( filename, source_code, size, "VertexFunction", "vs_5_0" );
   ID3DBlob *fs_bytecode = CompileHLSLToShaderBlob( filename, source_code, size, "FragmentFunction", "ps_5_0" );

   // Compile the byte code to the final shader (driver/hardware specific program)
   gD3DDevice->CreateVertexShader( vs_bytecode->GetBufferPointer(), 
      vs_bytecode->GetBufferSize(), 
      nullptr, 
      &gD3DVertexShader );

   gD3DDevice->CreatePixelShader( fs_bytecode->GetBufferPointer(), 
      fs_bytecode->GetBufferSize(), 
      nullptr, 
      &gD3DFragmentShader );

   DX_SAFE_RELEASE( vs_bytecode );
   DX_SAFE_RELEASE( fs_bytecode );
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void ClearScreen()
{
   // Clear the buffer.
   float clear_color[4] = { 0.80f, 0.25f, 0.0f, 1.0f };
   gD3DContext->ClearRenderTargetView( gD3DRenderTargetView, clear_color );
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void DemoRender()
{
   ClearScreen();

   // Now, we don't actually render to anything by default - so before we can draw anything
   // we must tell D3D11 where we are rendering to.  In this case - the back buffer.
   gD3DContext->OMSetRenderTargets( 1, &gD3DRenderTargetView, nullptr );

   // Also, set which region of the screen we're rendering to, in this case, all of it 
   D3D11_VIEWPORT viewport;
   memset( &viewport, 0, sizeof(viewport) );
   viewport.TopLeftX = 0U;
   viewport.TopLeftY = 0U;
   viewport.Width = WINDOW_RES_X;
   viewport.Height = WINDOW_RES_Y;
   viewport.MinDepth = 0.0f;        // must be between 0 and 1 (defualt is 0);
   viewport.MaxDepth = 1.0f;        // must be between 0 and 1 (default is 1)
   gD3DContext->RSSetViewports( 1, &viewport );

   // 1. How are we drawing - the Shader Programs we compiled earliar.  For this class
   //    we only worry about the Vertex and Pixel Shader - but all stages in the Graphics
   //    pipeline our program uses should be set.
   //    <note: I use OpenGL naming, and refer to Pixel Shaders are Fragment Shaders>
   gD3DContext->VSSetShader( gD3DVertexShader, nullptr, 0U );
   gD3DContext->PSSetShader( gD3DFragmentShader, nullptr, 0U );

   // 2.   Any textures or constant buffers we need - for this program, we have none of these - so, NOTHING

   // 3.   Topology Type - the data in the Vertex Buffer/Index Buffer describes what kind of shapes.  
   //      In thish class - I only deal with POINTS, LINES, and TRIANGLES.
   gD3DContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    
   // Now that everything is bound, we can draw!
   gD3DContext->Draw( 3, 0 );                      // I'm drawing one triangle, so 3 vertices total.  Starting at index 0.


   // We're done rendering, so tell the swap chain they can copy the back buffer to the front (desktop/window) buffer
   gD3DSwapChain->Present( 0, // Sync Interval, set to 1 for VSync (note, the OS may override this anyway)
      0 );                    // Present flags, see;
                              // https://msdn.microsoft.com/en-us/library/windows/desktop/bb509554(v=vs.85).aspx
}

//------------------------------------------------------------------------
// DEMO ENTRY POINT
//------------------------------------------------------------------------
void DemoRun()
{
   // Need a Window so I have something to render to.
   WindowCreate();

   // Setup D3D11 - will crash if this fails (demo contains no error checking)
   D3D11Setup(gHWND);
   
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   CreateBackBufferView(); // can be part of setup
   CreateShaders( "hlsl/nop_color.hlsl" );

   // While this window is open, process messages, and do other game processing
   while (WindowIsOpen()) {
      WindowStep();

      // Do other stuff.
      DemoRender();
   }

   
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Clenaup our shader
   DX_SAFE_RELEASE(gD3DVertexShader);
   DX_SAFE_RELEASE(gD3DFragmentShader);
   DX_SAFE_RELEASE(gD3DInputLayout);
   // Cleanup our buffer
   DX_SAFE_RELEASE(gD3DVertexBuffer);
   // Clean up our view.
   DX_SAFE_RELEASE(gD3DRenderTargetView);

  
   D3D11Cleanup();
   WindowDestroy();
}
