/* This file is included into demo.cpp,
   and inclues all old demo code that is no
   longer important to what this project is demonstrating */

void WindowCreate();
void WindowDestroy();
void WindowStep();
bool WindowIsOpen();

bool D3D11Setup(HWND hwnd);   // Creates required D3D Objects
void D3D11Cleanup();          // Cleans up D3D11 Objects

//------------------------------------------------------------------------
// WINDOW STUFF
//------------------------------------------------------------------------
// WINDOWS

//------------------------------------------------------------------------
void WindowCreate()
{
   int const left_position = 100;
   int const top_position = 100;

   gHWND = CreateTheWindow( WINDOW_TITLE, left_position, top_position, WINDOW_RES_X, WINDOW_RES_Y );
}

//------------------------------------------------------------------------
void WindowDestroy()
{
   ::DestroyWindow(gHWND);
   gHWND = NULL;
}

//------------------------------------------------------------------------
bool WindowIsOpen() 
{
   return ::IsWindow(gHWND) == TRUE;
}

//------------------------------------------------------------------------
void WindowStep()
{
   ProcessWindowMessages(gHWND);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool D3D11Setup( HWND hwnd )
{
   // Creation Flags
   // For options, see;
   // https://www.google.com/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#safe=off&q=device_flags+%7C%3D+D3D11_CREATE_DEVICE_DEBUG%3B
   uint device_flags = 0U;
   #if defined(RENDER_DEBUG)
      device_flags |= D3D11_CREATE_DEVICE_DEBUG;

      // This flag fails unless we' do 11.1 (which we're not), and we query that
      // the adapter support its (which we're not).  Just here to let you know it exists.
      // device_flags |= D3D11_CREATE_DEVICE_DEBUGGABLE; 
   #endif

   // Setup our Swap Chain
   // For options, see;
   // https://www.google.com/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#safe=off&q=DXGI_SWAP_CHAIN_DESC
    
   DXGI_SWAP_CHAIN_DESC swap_desc;
   memset( &swap_desc, 0, sizeof(swap_desc) );
   
   // fill the swap chain description struct
   swap_desc.BufferCount = 2;                                    // two buffers (one front, one back?)
  
   swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT       // how swap chain is to be used
      | DXGI_USAGE_BACK_BUFFER;                                  
   swap_desc.OutputWindow = hwnd;                                // the window to be copied to on present
   swap_desc.SampleDesc.Count = 1;                               // how many multisamples (1 means no multi sampling)

   // Default options.
   swap_desc.Windowed = TRUE;                                    // windowed/full-screen mode
   swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
   swap_desc.BufferDesc.Width = WINDOW_RES_X;
   swap_desc.BufferDesc.Height = WINDOW_RES_Y;

   
   // Actually Create
   HRESULT hr = ::D3D11CreateDeviceAndSwapChain( nullptr, // Adapter, if nullptr, will use adapter window is primarily on.
      D3D_DRIVER_TYPE_HARDWARE,  // Driver Type - We want to use the GPU (HARDWARE)
      nullptr,                   // Software Module - DLL that implements software mode (we do not use)
      device_flags,              // device creation options
      nullptr,                   // feature level (use default)
      0U,                        // number of feature levels to attempt
      D3D11_SDK_VERSION,         // SDK Version to use
      &swap_desc,                // Description of our swap chain
      &gD3DSwapChain,            // Swap Chain we're creating
      &gD3DDevice,               // [out] The device created
      nullptr,                   // [out] Feature Level Acquired
      &gD3DContext );            // Context that can issue commands on this pipe.

   // SUCCEEDED & FAILED are macros provided by Windows to checking
   // the results.  Almost every D3D call will return one - be sure to check it.
   return SUCCEEDED(hr);
}

//------------------------------------------------------------------------
void D3D11Cleanup()
{
   DX_SAFE_RELEASE(gD3DSwapChain);
   DX_SAFE_RELEASE(gD3DContext);
   DX_SAFE_RELEASE(gD3DDevice);
}

//------------------------------------------------------------------------
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


//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
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

   CreateInputLayout( vs_bytecode );
   DX_SAFE_RELEASE( vs_bytecode );
   DX_SAFE_RELEASE( fs_bytecode );
}

//------------------------------------------------------------------------
void ClearScreen()
{
   // Clear the buffer.
   float clear_color[4] = { 0.00f, 0.05f, 0.40f, 1.0f };
   gD3DContext->ClearRenderTargetView( gD3DRenderTargetView, clear_color );
}
