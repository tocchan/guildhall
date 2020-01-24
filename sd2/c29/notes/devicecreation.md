## Initial Setup

Be sure to include the libs and headers.  

```cpp
#if !defined(WIN32_LEAN_AND_MEAN) 
    #define WIN32_LEAN_AND_MEAN
#endif

#define INITGUID
#include <d3d11.h>  // d3d11 specific objects
#include <dxgi.h>   // shared library used across multiple dx graphical interfaces
#include <dxgidebug.h>  // debug utility (mostly used for reporting and analytics)

#pragma comment( lib, "d3d11.lib" )         // needed a01
#pragma comment( lib, "dxgi.lib" )          // needed a01
#pragma comment( lib, "d3dcompiler.lib" )   // needed when we get to shaders
```

## The `Instance`
Most modern rendering APIs have an `Instance` or a singleton global for creating everything else.  In D3D11, this is the `IDXGIFactory`.  You *can* use this this to enumerate all physical devices on your machine, and pick the best one.

Though, D3D11 is a little nicer to us, and has some helper functions for skipping this step if you already have a window, so that is where we'll be starting; 

## The `Device` and `Swapchain`
This is where we'll be starting, getting the following objects;

```cpp
ID3D11Device* m_device;         // in charge of creating resources
ID3D11DeviceContext* m_context; // issues commands to the device and keeps track of current state
IDXGISwapChain* m_swapchain;    // gives us textures that we can draw that the user can see
```

Most creation in modern APIs are going to adhere to the following pattern;

1. Fill out a description structure for the object
2. Call the approprirate creation function
3. Check for errors

So, first example of this will be the swapchain, so let us describe it...

```cpp
DXGI_SWAP_CHAIN_DESC swapchain_desc; 
memset( &swapchain_desc, 0, sizeof(swapchain_desc) ); 

// how many back buffers in our chain - we'll double buffer (one we show, one we draw to)
swapchain_desc.BufferCount = 2;      
swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // on swap, the old buffer is discarded
swapchain_desc.Flags = 0; // additional flags - see docs.  Used in special cases like for video buffers

// how swap chain is to be used
swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
swapchain_desc.OutputWindow = window->get_handle(); // HWND for the window to be used
swapchain_desc.SampleDesc.Count = 1; // how many samples per pixel (1 so no MSAA)
                                     // note, if we're doing MSAA, we'll do it on a secondary target

// describe the buffer
swapchain_desc.Windowed = TRUE;                                    // windowed/full-screen mode
swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color RGBA8 color
swapchain_desc.BufferDesc.Width = window->get_client_width(); 
swapchain_desc.BufferDesc.Height = window->get_client_height();
```

Now that we have this, we'll create all the D3D objects we need for this assignment; 

```cpp
// ID3D11Device* m_device;
// ID3D11DeviceContext* m_context; 
// SwapChain* m_swapchain; // a class we'll be making soon
IDXGISwapChain* swapchain = nullptr;

// options for our device
uint device_flags = 0U;
device_flags |= D3D11_CREATE_DEVICE_SINGLETHREADED; // we will handle our own threading

// optionally, make a debug context (you will want to do this most of the time,
// it gives much better error reporting, and reports leaks)
#if defined(RENDER_DEBUG)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG; //  debug mode
#endif

::D3D11CreateDeviceAndSwapChain( nullptr, // Adapter this device is for - nullptr will use primary
    D3D_DRIVER_TYPE_HARDWARE,             // type of adapter we want, and we want hardware accelerated
    nullptr,                              // software renderer implmentation - not using it
    device_flags,                         // options for the device, setup above
    nullptr,                              // feature level, nullptr means use default
    0,                                    // how many features we're attempting
    D3D11_SDK_VERSION,                    // SDK version to use
    &swapchain_desc,                      // description of the swapchain we want to make
    &swapchain,                           // [out] swapchain
    &device,                              // [out] device
    nullptr,                              // [out] feature level acquired 
    &context );                           // [out] immediate context


if (swapchain != nullptr) {
    m_swapchain = new SwapChain( this, swapchain ); 
}
```

And now we have a device.

So I'm not going to keep the `ID3D11SwapChain` on the `RenderContext`, but instead make a new class for it.  This is mainly to set you up for multiple displays if you ever want to do that.

## Cleanup
When shutting down, we need to release these resources, which looks like this;

```cpp
if (resource != nullptr) {
    resource->Release();
    resource = nullptr; 
}
```

I'm going to encompass this in a macro (though feel free to use a template if you have religious concerns).

```cpp
#define DX_SAFE_RELEASE(obj)  if (nullptr != (obj)) { (obj)->Release(); (obj) = nullptr; }
```

So our shutdown for our context will look like this;

```cpp
void RenderContext::Shutdown()
{
    // swapchain is one of our engine objects
    delete m_swapchain; 
    m_swapchain = nullptr; 

    // release
    DX_SAFE_RELEASE( m_context ); 
    DX_SAFE_RELEASE( m_device ); 
}
```