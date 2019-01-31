#include "demo.h"

#include "window.h"

// IMPORTANT - needed to define
// some objects for DXGIDebug reporting; 
#define INITGUID

// Required Headers
#include <d3d11.h>  
#include <DXGI.h>    

// DEBUG STUFF
#include <dxgidebug.h>
// #pragma comment( lib, "dxguid.lib" )

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )


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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// D3D11 STUFF
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// IMPORTANT - Must define this before including D3D11 libraries
// so for certain constants to be defined 
static HMODULE gD3DDebugModule = NULL; 
static IDXGIDebug *gD3D11DebugObject = nullptr; 

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void D3D11SetupDebug()
{
   #if defined(RENDER_DEBUG)

      // First, get access to the DLL
      gD3DDebugModule = ::LoadLibraryA( "Dxgidebug.dll" );
      if (gD3DDebugModule == nullptr) {
         // Failed - can't use this feature; 
         return; 
      } else {
         // Find the function to create the debug object from the DLL
         typedef HRESULT (WINAPI *GetDebugModuleCB)( REFIID, void** );
         GetDebugModuleCB cb = (GetDebugModuleCB) ::GetProcAddress( gD3DDebugModule, "DXGIGetDebugInterface" );
         HRESULT hr = cb( __uuidof(IDXGIDebug), (void**)&gD3D11DebugObject );
         
         /*
         // If we got one - tell D3D11 to keep track of everything in detail; 
         if (SUCCEEDED(hr)) {
            gD3D11DebugObject->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
         }
         */
      }
   #endif
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void D3D11CleanupDebug()
{
   // free up the resources; 
   DX_SAFE_RELEASE( gD3D11DebugObject );
   if (gD3DDebugModule != NULL) {
      FreeLibrary( gD3DDebugModule ); 
      gD3DDebugModule = NULL; 
   }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This will list out current live objects - so after you free up your device
// call this to check if any objects rae still alive; 
void D3D11ReportLiveObjects()
{
   if (gD3D11DebugObject != nullptr) {
      gD3D11DebugObject->ReportLiveObjects( DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL ); 
   }
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
   
   // **NEW** - Setup and Enable D3D debug tracking 
   D3D11SetupDebug(); 

   // While this window is open, process messages, and do other game processing
   while (WindowIsOpen()) {
      WindowStep();

      // Do other stuff.
      DemoRender();
   }

   // **NEW** Print out leaked objects with names; 
   // List the current live objects (will list, since we haven't cleaned up)
   D3D11ReportLiveObjects(); 

   // Normal Cleanup
   D3D11Cleanup();
   
   // **NEW** Print out leaked objects with names; 
   // Will list nothing because everything should be cleaned up; 
   D3D11ReportLiveObjects(); 

   // **NEW** Cleanup Debug Resources
   D3D11CleanupDebug(); 

   WindowDestroy();
}
