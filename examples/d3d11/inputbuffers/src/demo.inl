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
