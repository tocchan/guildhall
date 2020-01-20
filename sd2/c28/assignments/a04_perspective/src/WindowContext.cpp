#include "Engine/Core/WindowContext.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
// #include "Core/Internal/WindowsCommon.hpp"

static constexpr const TCHAR* GAME_WINDOW_CLASS_NAME = TEXT("GameWindowClass");

//-----------------------------------------------------------------------------------------------
static void LockMouseToWindow( HWND hwnd )
{
   RECT clientRect; 
   GetClientRect( hwnd, &clientRect ); 

   POINT clientOrigin = { 0, 0 }; 
   ClientToScreen( hwnd, &clientOrigin ); 

   clientRect.left += clientOrigin.x; 
   clientRect.right += clientOrigin.x; 
   clientRect.top += clientOrigin.y; 
   clientRect.bottom += clientOrigin.y; 

   ::ClipCursor( &clientRect ); 
}

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
      case WM_ACTIVATE: {
         WORD activeWord = LOWORD(wParam); 
         bool isActive = (activeWord != WA_INACTIVE);
         if (isActive && windowContext->IsMouseLocked()) {
            LockMouseToWindow( windowHandle ); 
         }
      } break; 

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
   m_hwnd = nullptr; 

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
void WindowContext::Create( std::string const &title, float clientAspect, float maxClientFractionOfDesktop, windows_proc_cb app_proc )
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

   m_hwnd = (void*)hwnd;
   m_gameCB = app_proc; 
   SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)this );

	ShowWindow( hwnd, SW_SHOW );
	SetForegroundWindow( hwnd );
	SetFocus( hwnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

   // Start the game with 0 mouse movement; 
   m_lastFrameMousePosition = GetClientMousePosition(); 
   m_currentMousePosition = GetClientMousePosition(); 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::Close()
{
   if (!IsOpen()) {
      return; 
   }

   CloseWindow( (HWND)m_hwnd ); 
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

   // always save off old one;
   m_lastFrameMousePosition = m_currentMousePosition; 
   m_currentMousePosition = GetClientMousePosition(); 

   if ((m_mouseMode == MOUSE_MODE_RELATIVE) && HasFocus()) {
      IntVec2 center = GetClientCenter();
      SetClientMousePosition( center ); 
      m_lastFrameMousePosition = GetClientMousePosition(); 
   }
}

//-----------------------------------------------------------------------------------------------
IntVec2 WindowContext::GetClientCenter() const
{
   RECT clientRect; 
   GetClientRect( (HWND)m_hwnd, &clientRect ); 

   IntVec2 center; 
   center.x = (clientRect.left + clientRect.right) / 2; 
   center.y = (clientRect.top + clientRect.bottom) / 2; 

   return center; 
}

//-----------------------------------------------------------------------------------------------
IntVec2 WindowContext::GetClientMousePosition()
{
   POINT desktopPosition; 
   if (!::GetCursorPos( &desktopPosition )) {
      desktopPosition.x = 0; 
      desktopPosition.y = 0; 
   }

   ::ScreenToClient( (HWND)m_hwnd, &desktopPosition ); 
   return IntVec2( desktopPosition.x, desktopPosition.y ); 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::SetClientMousePosition( IntVec2 &clientPos )
{
   POINT screen = { clientPos.x, clientPos.y }; 
   ::ClientToScreen( (HWND)m_hwnd, &screen ); 

   ::SetCursorPos( screen.x, screen.y ); 
}

//-----------------------------------------------------------------------------------------------
IntVec2 WindowContext::GetClientMouseRelativeMovement()
{
   return m_currentMousePosition - m_lastFrameMousePosition; 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::LockMouse()
{
   ++m_lockCount; 
   if (m_lockCount > 0) {
      LockMouseToWindow( (HWND)m_hwnd ); 
   }

   // 0 out for a frame; 
   m_lastFrameMousePosition = GetClientMousePosition(); 
   m_currentMousePosition = GetClientMousePosition(); 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::UnlockMouse() 
{
   --m_lockCount; 
   if (m_lockCount <= 0) {
      ::ClipCursor(nullptr); 
   }
}

//-----------------------------------------------------------------------------------------------
void WindowContext::ShowMouse()
{
   ::ShowCursor(TRUE); 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::HideMouse()
{
   ::ShowCursor(FALSE); 
}

//-----------------------------------------------------------------------------------------------
bool WindowContext::HasFocus() const
{
   return (::GetActiveWindow() == (HWND)m_hwnd); 
}

//-----------------------------------------------------------------------------------------------
void WindowContext::SetMouseMode(eMouseMode mode)
{
   m_mouseMode = mode; 
   if (mode == MOUSE_MODE_RELATIVE) {
      IntVec2 center = GetClientCenter(); 
      m_currentMousePosition = center;
      m_lastFrameMousePosition = center; 
      SetClientMousePosition( center ); 
   }
}

