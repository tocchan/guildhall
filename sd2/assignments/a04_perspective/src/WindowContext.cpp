#include "Engine/Core/WindowContext.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
// #include "Core/Internal/WindowsCommon.hpp"

static constexpr const TCHAR* GAME_WINDOW_CLASS_NAME = TEXT("GameWindowClass");

//-----------------------------------------------------------------------------------------------
// Handles Windows (Win32) messages/events; i.e. the OS is trying to tell us something happened.
// This function is called by Windows whenever we ask it for notifications
//
// #SD1ToDo: We will move this function to a more appropriate place later on...
//
static LRESULT CALLBACK GameCommonWindowProc( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
   WindowContext *windowContext = (WindowContext*) GetWindowLongPtr( windowHandle, GWLP_USERDATA );
	
   // do Engine level message handling
   switch (wmMessageCode) {
      default:
         break; 
   }; 
   
   // Next, call the callback associated with the window (AppCallback)
   
   bool handled = false; 
   if (windowContext != nullptr) {
      handled = windowContext->m_gameCB( windowHandle, wmMessageCode, wParam, lParam ); 
   } 
   
   // if they don't handle it,
	if (!handled) {
      return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
   } else {
      return 0; 
   }
}


static int gRegisterCount = 0; 

//-----------------------------------------------------------------------------------------------
static void RegisterGameWindowClass()
{
	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( GameCommonWindowProc ); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = GAME_WINDOW_CLASS_NAME; 
	RegisterClassEx( &windowClassDescription );
}


//-----------------------------------------------------------------------------------------------
WindowContext::WindowContext()
{
   if (gRegisterCount == 0) {
      RegisterGameWindowClass();
   }
   ++gRegisterCount; 
}

//-----------------------------------------------------------------------------------------------
WindowContext::~WindowContext()
{
   Close(); 

   --gRegisterCount;
   if (gRegisterCount == 0) {
      UnregisterClass( GAME_WINDOW_CLASS_NAME, GetModuleHandle(NULL) ); 
   }
}


//-----------------------------------------------------------------------------------------------
void WindowContext::Create( std::string const &title, float clientAspect, float maxClientFractionOfDesktop, window_proc_cb app_proc )
{
   ASSERT_OR_DIE( !IsOpen(), "Window already open." ); 

	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)
	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );
	float desktopWidth = (float)( desktopRect.right - desktopRect.left );
	float desktopHeight = (float)( desktopRect.bottom - desktopRect.top );
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if( clientAspect > desktopAspect )
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int) clientMarginX;
	clientRect.right = clientRect.left + (int) clientWidth;
	clientRect.top = (int) clientMarginY;
	clientRect.bottom = clientRect.top + (int) clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, title.c_str(), -1, windowTitle, sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	HWND hwnd = CreateWindowEx(
		windowStyleExFlags,
		GAME_WINDOW_CLASS_NAME,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		::GetModuleHandle(NULL),
		NULL );

   ASSERT_OR_DIE( hwnd != NULL, "Failed to create window" ); 
   SetWindowLongPtr( hwnd, GWLP_USERDATA, this );

	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

   m_hwnd = (void*)hwnd;
   m_gameCB = app_proc; 

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );
}

//-----------------------------------------------------------------------------------------------
void WindowContext::Close()
{
   if (!IsOpen()) {
      return; 
   }

   CloseWindow( m_hwnd ); 
   m_hwnd = nullptr; 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::BeginFrame()
{
   MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage ); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}

