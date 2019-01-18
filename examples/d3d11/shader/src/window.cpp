
#include "window.h"
#include "log.h"

//------------------------------------------------------------------------
//
// DEFINES
//
//------------------------------------------------------------------------
#define WNDCLASS_GAME_NAME    ("GameWindowClass")

//------------------------------------------------------------------------
//
// INTERNAL FUNCTIONS
//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Get the instance of the running program.  Here so I don't need 
// to pass around an HINSTANCE from my WinMain to all my window creation
// functions.
static HINSTANCE GetCurrentHINSTANCE()
{
    return ::GetModuleHandle(NULL);
}

//------------------------------------------------------------------------
// Message Handler for the windows we create.
// Determines how it responds to specific events.
static LRESULT CALLBACK GameWndProc( HWND hwnd, 
   UINT msg, 
   WPARAM wparam, 
   LPARAM lparam )
{
   switch (msg) {
      //------------------------------------------------------------------------
      // IMPORTANT/COMMON
      //------------------------------------------------------------------------
      case WM_CREATE: {
         // Window creation happened.
      } break;

      case WM_SHOWWINDOW: {
         // Window was shown/hidden
         // bool visible = (wparam == TRUE);
      } break;

      case WM_DESTROY: {
         // Window has been destroyed (actually closed)
         Log( "Window Destroyed" );
      } break;


     
      // Never do the default action for WM_CLOSE
      case WM_CLOSE: {
         // User has requested this window be closed
         // so actually destroy it
         // (note - you can intercept this by not calling DestroyWindow.
         // For example, if you wanted a custom in-game dialogue whent he user ALT-F4s
         ::DestroyWindow(hwnd);
         return 0;
      } 
                     
      case WM_PAINT: {
         // Ignore WM_PAINT messages
         // [c4] Must handle them otherwise they just keep coming
         PAINTSTRUCT ps;
         ::BeginPaint( hwnd, &ps );
         ::EndPaint( hwnd, &ps );
      }   
      return 1;

      //------------------------------------------------------------------------
      // INPUT      
      //------------------------------------------------------------------------

      case WM_KEYUP: {
         // User has released a key
         // wparam - virtual key code
         // lparam - additional information
      } break;

      case WM_KEYDOWN: {
         // User has pressed a key
         // wparam - virtual key code
         // lparam - additional information

         // TEMP - just to show I can close with Escape
         if (wparam == VK_ESCAPE) {
            ::DestroyWindow(hwnd);
         }
      } break;

      case WM_CHAR: {
         // User has typed a character
         // wparam - char code
         // lparam - additional information (like ALT state)
      } break;

      case WM_MOUSEMOVE:
         // mouse has moved.
         // wparam - mouse keys
         // lparam - mouse positions
         
         // Positions are relative to client area.
         // int mouse_x = LOWORD(lparam);
         // int mouse_y = HIWORD(lparam);
         break;

      //------------------------------------------------------------------------
      // POTENTIALLY USEFUL EVENTS - LOOK UP IN DOCUMENTATION 
      //------------------------------------------------------------------------
      case WM_ACTIVATE:   {
         // called when the window is activated/gains focus, or is deactivated/loses focus.
      }
      break;
      
      case WM_MOVE:
      case WM_SIZE: {
         // Called when the window changes size or is moved.
      } break;

      case WM_DWMNCRENDERINGCHANGED: {
         // Rendering policy has changed (full screen deciated in OpenGL will cause this)
      } break;
     

      
      case WM_DPICHANGED: {
         // If the monitor DPI has changed.
      } break;
         
      case WM_ERASEBKGND: {
         // sent when the background needs to be erased (may potentially want to render)
         return 1;
      }

      //------------------------------------------------------------------------
      // NO-OP these ones - prevents windows key
      //------------------------------------------------------------------------
      case WM_INITMENUPOPUP: 
      // case WM_SYSCOMMAND:
      case WM_ENTERIDLE:
      {
         // Consumes the event
         return 0;
      }

      default:
         // Trace( "wnd", "Unhandled Message: 0x%04X [0x%04X, 0x%04X]", msg, wparam, lparam );
         break;

   }

   // Default handling of messages
   return DefWindowProc( hwnd, msg, wparam, lparam );
}

//------------------------------------------------------------------------
// Gets the window size needed for the desired client size.
static RECT WindowGetRect( int width, int height, DWORD style )
{
   RECT window_rect;
   window_rect.left = 0;
   window_rect.top = 0;
   window_rect.right = width;
   window_rect.bottom = height;

   // Adjust the size so the window is big enough with this style to allow the client
   // to be the size of window_rect
   ::AdjustWindowRect( &window_rect, style, FALSE );

   return window_rect;
}

//------------------------------------------------------------------------
//
// EXTERNAL FUNCTIONS
//
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// A window class defines a "type" of window our program will use
// For games, we usually just have the one.
//------------------------------------------------------------------------
bool RegisterGameWindowClass()
{
   Log( "Registering Window Class" );

   WNDCLASSEX wc;
   memset( &wc, 0, sizeof(wc) );

   // Setup the definition for this window class
   wc.cbSize = sizeof(WNDCLASSEX);

   // This sets that it will redraw for vertical or horizontal changes
   // and it also owns its own device context handle (more effecient if we're
   // drawing to this window a lot - which we will be.
   wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

   // The Windows Messaeg Pump - handles messages for this window type.
   wc.lpfnWndProc = GameWndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = GetCurrentHINSTANCE();

   // Default icon for this window (can be set later)
   wc.hIcon = NULL;
   wc.hIconSm = NULL;

   // Cursor to use (can be set later if needed)
   wc.hCursor = ::LoadCursorA( NULL, IDC_ARROW );
   wc.hbrBackground = (HBRUSH) 0;
   wc.lpszMenuName = NULL;

   // Name to use when creating windows so it knows to use this class
   wc.lpszClassName = WNDCLASS_GAME_NAME;

   ATOM result = RegisterClassEx( &wc );
   if (NULL == result) {
      Logf( "Failed to register window class [ErrNo: %u]", GetLastError() );
      return false;
   }

   return true;
}

//------------------------------------------------------------------------
// Not needed since we're exiting the program anyway - but I like to 
// be thorough
void UnregisterGameWindowClass()
{
   UnregisterClass( WNDCLASS_GAME_NAME, GetCurrentHINSTANCE() );
}

//------------------------------------------------------------------------
// Create the window at (x,y) with a client size of (res_x, res_y)
HWND CreateTheWindow( char const *title, int x, int y, int res_x, int res_y )
{
   DWORD extended_style = 0;
   DWORD style = 0U;

   // If you want a "window" - this gives it the caption bar, a sysmenu, and a border.
   style |= (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER);

   // this style will instead make it borderless - good for fullscreen windows.
   // MUTUALLY EXCLUSIVE WITH ABOVE
   // style |= WS_POPUP;

   // Window starts invisible by default - this will allow it to be visible
   style |= WS_VISIBLE;

   RECT win_rect = WindowGetRect( res_x, res_y, style ); 

   x += win_rect.left;
   y += win_rect.top;
   int w = win_rect.right - win_rect.left;
   int h = win_rect.bottom - win_rect.top;
   
   HWND hwnd = CreateWindowExA( extended_style, // Extended style (not used)
      WNDCLASS_GAME_NAME,                       // Windows Class to use
      title,                                    // title      
      style,                                    // Style to start this with
      x, y,                                     // x, y of actual window (not client rect)
      w, h,                                     // width and height of actual window
      NULL,                                     // my parent (not used in this example)
      NULL,                                     // my menu (not used in this example)
      GetCurrentHINSTANCE(),                    // hinstance this window belongs too
      nullptr );                                // user data associated with this window (good place to store a pointer to a window object if you class'ify this)

   if (NULL == hwnd)   {
      DWORD error = GetLastError();
      Logf( "Failed to create window:  Error[%u]", error );
   }

   return hwnd;
}

//------------------------------------------------------------------------
int ProcessWindowMessages( HWND hwnd ) 
{
   int msgs_processed = 0;
   if (NULL != hwnd) {
      // See if this window has a message to process - if so, remove it.
      MSG msg;
      while (::PeekMessage( &msg, hwnd, 0, 0, PM_REMOVE)) {
         ++msgs_processed;

         // Processes virtual key messages into character key messages (and posts them to the queue)
         ::TranslateMessage( &msg );

         // Processes this message (sends it to teh windows procedue)
         ::DispatchMessage( &msg );
      }
   }

   // Not needed - but I like the metric.
   return msgs_processed;
}