#include "demo.h"

#include "window.h"

// D3D Stuff

//------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------
#define WINDOW_TITLE    "D3D11 SETUP"
#define WINDOW_RES_X    (1280)
#define WINDOW_RES_Y    (720)

#define RENDER_DEBUG

typedef unsigned int uint;

//------------------------------------------------------------------------
// MACROS
//------------------------------------------------------------------------
#define SAFE_DELETE(ptr)   if ((ptr) != nullptr) { delete ptr; ptr = nullptr }



//------------------------------------------------------------------------
// Declare functions this demo uses
//------------------------------------------------------------------------
void WindowCreate();
void WindowDestroy();
void WindowStep();
bool WindowIsOpen();

bool D3D11Setup(HWND hwnd);   // Creates required D3D Objects
void D3D11Cleanup();          // Cleans up D3D11 Objects

void DemoRender();            // Does rendering for this demo
void DemoRun();               // Demo setup and update loop

//------------------------------------------------------------------------
// Code not pertinant to the topic of this demo is moved here
//
// I will be swapping things in and out as I add/remove steps.
//------------------------------------------------------------------------
#include "demo.inl"

//------------------------------------------------------------------------
// WINDOW STUFF
//------------------------------------------------------------------------
// WINDOWS
static HWND gHWND = NULL;

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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// D3D11 STUFF
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// Required Headers
#include <d3d11.h>  
#include <DXGI.h>    

// DEBUG STUFF
#include <dxgidebug.h>
// #pragma comment( lib, "dxguid.lib" )

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )

#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

ID3D11Device *gD3DDevice = nullptr;
ID3D11DeviceContext *gD3DContext = nullptr;
IDXGISwapChain *gD3DSwapChain = nullptr;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void D3D11Cleanup()
{
   DX_SAFE_RELEASE(gD3DSwapChain);
   DX_SAFE_RELEASE(gD3DContext);
   DX_SAFE_RELEASE(gD3DDevice);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void DemoRender()
{
   // Get the back buffer
   ID3D11Texture2D *back_buffer = nullptr;
   gD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

   // Get a render target view of this
   // NOTE:  This could be cached off and stored instead of creating
   // a new one each frame.  It is fairly cheap to do though.
   ID3D11RenderTargetView *rtv = nullptr;
   gD3DDevice->CreateRenderTargetView( back_buffer, nullptr, &rtv );
   DX_SAFE_RELEASE( back_buffer ); // I'm done using this - so release my hold on it (does not destroy it!)

   // Clear the buffer.
   float clear_color[4] = { 0.004f, 0.475f, 0.435f, 1.0f };
   gD3DContext->ClearRenderTargetView( rtv, clear_color );
   DX_SAFE_RELEASE( rtv ); // done with the view - can release it (if you save it frame to frame, skip this step)

   // We're done rendering, so tell the swap chain they can copy the back buffer to the front (desktop/window) buffer
   gD3DSwapChain->Present( 0, // Sync Interval, set to 1 for VSync
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

   // While this window is open, process messages, and do other game processing
   while (WindowIsOpen()) {
      WindowStep();

      // Do other stuff.
      DemoRender();
   }

   // This is overkill - since it was destroyed before it got here
   // but just want it to match the create above (and more match what this looks
   // like when it becomes an class)
  
   D3D11Cleanup();
   WindowDestroy();
}
