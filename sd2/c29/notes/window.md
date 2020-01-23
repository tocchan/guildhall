## Preliminary Notes
- My project is setup to use UTF8 as the default encoding, where yours may be wide character.  If you run into bugs along the lines of "can't convert `const char*` to `const wchar_t*`", that is why.  Look over your SD1 project for where you do the conversion.  

- If a function name starts with `::` such as `::CreateWindowExA`, I'm just using that to denote that the function is a Win32 API function (`::` just means use the global namespace, which is usually just implied).  Can be useful if your method names conflict with Win32 API names.


## Creating

### Register the Class
The very first step in creating a window is registering its **Window Class**.  A **Window Class** defines the window to the OS;  what cursor to use, icon to use, what callback gets the messages, etc...

With one window class, you can create multiple windows of that type.  If you are planning on only ever supporting one window, you can register this in the constructor, and unregister in the deconstructor.  

If you ever want to support multiple windows that share a window class, I recommend using a global counter, ie;

```cpp
static uint gWindowCount = 0; 

//------------------------------------------------------------------------
Window::Window()
{
    if (gWindowCount == 0) {
        RegisterGameWindowClass(); 
    }
    ++gWindowCount; 

    // normal setup...
    // ... 
}

//------------------------------------------------------------------------
Window::~Window()
{
    // normal cleanup...
    // ...

    --gWindowCount; 
    if (gWindowCount == 0) {
        UnregisterGameWindowClass(); 
    }
}
```

Registering and Unregistering look like the following; 

*Note: `LogTaggedPrintf` is my output system, so you may need to replace that method with an equivalent in yours.*

```cpp
// these functions are static as I will usually hide them
// within the `Window.cpp` file

// name is how this class will be referenced, used with in 
// registering and creating of the window
static char const*const WNDCLASS_GAME_NAME = "EngineGameWindow";

//------------------------------------------------------------------------
static bool RegisterGameWindowClass()
{
   WNDCLASSEX wc;
   memset( &wc, 0, sizeof(wc) );

   // Setup the definition for this window class
   wc.cbSize = sizeof(WNDCLASSEX);

   // This sets that it will redraw for vertical or horizontal changes
   // and it also owns its own device context handle (more effecient if we're
   // drawing to this window a lot).
   wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

   // The Windows Messaeg Pump - handles messages for this window type.
   wc.lpfnWndProc = GameWndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = ::GetModuleHandle(NULL);

   // Default icon for this window (can be set later)
   wc.hIcon = NULL;
   wc.hIconSm = NULL;

   // Cursor to use (can be set later if needed)
   wc.hCursor = ::LoadCursorA( NULL, IDC_ARROW );
   wc.hbrBackground = (HBRUSH) 0;
   wc.lpszMenuName = NULL;

   // Name to use when creating windows so it knows to use this class
   wc.lpszClassName = WNDCLASS_GAME_NAME;

   ATOM result = ::RegisterClassEx( &wc );
   if (NULL == result) {
      LogTaggedPrintf( "window", "Failed to register window class [ErrNo: %u]", GetLastError() );
      return false;
   }

   return true;
}

//------------------------------------------------------------------------
static void UnregisterWindowClass( char const *class_name )
{
   ::UnregisterClass( class_name, ::GetModuleHandle(NULL) );
}
```

### The Windows Procedure
So Windows communicates with us through the **Windows Procedure**, which is a callback we register with the window class.  This would be the `GameWindowProc` function above.  

This should already be defined in your `App.cpp` or `WinMain.cpp`.  We'll be moving it over to 
`Window.cpp` instead.

It should have a signature similar to this; 

```cpp
static LRESULT CALLBACK GameWndProc( HWND hwnd, 
   UINT msg, 
   WPARAM wparam, 
   LPARAM lparam )
{
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint( hwnd, &ps );
            EndPaint( hwnd, &ps );
        } return 1;  // handled it

        // handle other messages our app cares about
        // ...
    }

    // anything we don't handle, we do the default work for; 
    return DefWindowProc( hwnd, msg, wparam, lparam );
}
```

---

### Creating the Window

Once we have a window class registered, we can use it to create a window using `CreateWindowEx`;

```cpp
// picking a style that can be resized on purpose.
DWORD style =  WS_VISIBLE   // starts visible; 
    | WS_OVERLAPPED         // title bar and border
    | WS_SYSMENU;           // system menu


DWORD extended_style = 0;   // not using any of the extended styles

// client_width and client_height are how
// large you want the client area; 
int res_x = client_width;
int res_y = client_height;

// from our client size (drawable area),
// get the full size of the window (depends on style)
RECT win_rect; 
win_rect.left = 0;
win_rect.top = 0;
win_rect.right = width;
win_rect.bottom = height;
::AdjustWindowRect( &win_rect,  // in-out variable that is being adjusted
    style,                      // style window is using
    FALSE );                    // does the window have a menu to take into account?


int width = win_rect.right - win_rect.left;
int height = win_rect.bottom - win_rect.top;
x = CW_USEDEFAULT; // where the window will appear - this will let windows position it
y = CW_USEDEFAULT; 

HWND hwnd = ::CreateWindowExA( extended_style,
    WNDCLASS_GAME_NAME,
    m_title.c_str(), // title
    style,
    x, y,
    width, height,
    NULL,       // parent window (none)
    NULL,       // menu object (none)
    ::GetModuleHandle(NULL),    // process we're creating it as part of
    NULL );     // parameter to pass forward with WM_CREATE

if (NULL == hwnd) {
    DWORD error = ::GetLastError();
    LogTaggedPrintf( "window", "Failed to create window:  Error[%u]", error );
    return false;
}
```

### Closing a Window
This part is a lot easier...

```cpp
::DestroyWindow( hwnd )
```

### Handling Messages
And finally, we need to give our program a time to process messages coming from windows; 

```cpp
void Window::BeginFrame()
{
    // Process window messages; 
    MSG msg;
    while (::PeekMessage( &msg, m_hwnd, 0, 0, PM_REMOVE)) {
        ::TranslateMessage( &msg ); // may generate new messages from a message (WM_KEYDOWN -> WM_CHAR)

        ::DispatchMessage( &msg ); // call the window proc

        if (msg.message == WM_DESTROY) {
            // what to do when the window was destroy?  May want to mark 
            // the window as closed so you can respond to it; 
            // ...

            break; 
        } 
    }
}
```

