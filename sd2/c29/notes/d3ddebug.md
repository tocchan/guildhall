## Debug Module

If we're making a debug context, we also should make a debug module.  This gives us better
information baout leaks, as well as lets us hook into the debug output stream in case we want to forward it to our own console or log system.

## Links
- [Direct3D 11 Debug API Tricks](https://seanmiddleditch.com/direct3d-11-debug-api-tricks/)

## Setup

The debug module should be created before you make any other D3D11 object, and destroyed after all other objects have been destroyed;

```cpp
class RenderContext
{
    public:
        // ...
        HMODULE m_debugModule        = nullptr; 
        IDXGIDebug *m_debug           = nullptr; 

}

```cpp
void RenderContext::Startup( Window* wnd )
{
    #if defined(RENDER_DEBUG)
        CreateDebugModule(); 
    #endif

    // other setup
    // ...
}

void RenderContext::Shutdown()
{
    // other shutdown
    // ...

    ReportLiveObjects();    // do our leak reporting just before shutdown to give us a better detailed list; 
    DestroyDebugModule();
}
```

So, creating the module first loads the DLL to find the creation function, then uses that to create.

```cpp
// create the debug module for us to use (for now, only for reporting)
void RenderContext::CreateDebugModule()
{
    // load the dll
    m_debugModule = ::LoadLibraryA( "Dxgidebug.dll" );
    if (m_debugModule == nullptr) {
        LogTaggedPrintf( "gfx", "Failed to find dxgidebug.dll.  No debug features enabled." ); 
    } else {
        // find a function in the loaded dll
        typedef HRESULT (WINAPI *GetDebugModuleCB)( REFIID, void** );
        GetDebugModuleCB cb = (GetDebugModuleCB) ::GetProcAddress( m_debugModule, "DXGIGetDebugInterface" );

        // create our debug object
        HRESULT hr = cb( __uuidof(IDXGIDebug), (void**)&m_debug );
        ASSERT( SUCCEEDED(hr) );
    }
}

// cleanup after ourselves
void RenderContext::DestroyDebugModule()
{
    if (nullptr != m_debug) {
        DX_SAFE_RELEASE(m_debug);   // release our debug object
        FreeLibrary(m_debugModule); // unload the dll

        m_debug = nullptr;
        m_debugModule = nullptr; 
    }
}

// This method will list all current live D3D objects, types, and reference counts
void RenderContext::ReportLiveObjects()
{
    if (nullptr != m_debug) {
        m_debug->ReportLiveObjects( DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL ); 
    }
}
```