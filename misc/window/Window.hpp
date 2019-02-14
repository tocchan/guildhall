
//---------------------------------------------------------------
// Engine/Window.hpp
//---------------------------------------------------------------

typedef bool (*windows_proc_cb)( HWND, UINT, WPARAM, LPARAM ); 

class Window
{
   public:
      void Create( int widht, int height, windows_proc_cb cb ); 
      void SetTitle( std::string const &title ); 

   public:
      HWND m_hwnd; // void* if you're avoiding Windows.h

      windows_proc_cb m_gameCB;  
};


//---------------------------------------------------------------
// Window.cpp
//---------------------------------------------------------------

//---------------------------------------------------------------
static LRESULT CALLBACK GameWndProc( HWND hwnd, 
   UINT msg, 
   WPARAM wparam, 
   LPARAM lparam )
{
   // calling app code... I can't do that anymore; 
   Window *wnd = (Window*) GetWindowLongPtr( hwnd, GWLP_USERDATA );

   switch (msg) {
      case WM_...:
         break; 

      // ...
   }; 

   // Let app do whatever it needs to do with this message; 
   if (wnd != nullptr) {
      if (!wnd->m_gameCB( hwnd, msg, wparam, lparam )) {
         return DefWndProc( hwnd, msg, wparam, lparam ); 
      }
   }

   return 0; 
}


//---------------------------------------------------------------
void Window::Create( int width, int height ) 
{
   // Register window class IF this is the first window to be created; 
   RegisterWindowClass(); 

   // WindowsProc registered - that moves to Engine/Window.cpp
   wc.lpfzWndProc = EngineWindowsHandlingProcedure; 

   // most code from app goes here; 
   // create the window
   // ...


   // Register the pointer WITH the hwnd
   SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG_PTR)this );
  
}



// Game/App.hpp

//---------------------------------------------------------------
// WinMain32.hpp -> ProcessWindowsMessagePump
//---------------------------------------------------------------


//---------------------------------------------------------------
class App 
{
   public:
      void Startup()
      {
         // Register Window Class

         // Create the Window
         m_window = new Window(); 
         m_window->RegisterWindowsProc( WindowsMessageHandlingProcedure ); 
         m_window->Create(width, height); 



         // Give it to RenderContext
         m_window->GetHandle(); 
      }

   public:
      Window *m_window; 
};
