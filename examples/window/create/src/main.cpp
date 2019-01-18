#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "log.h"
#include "window.h"


//------------------------------------------------------------------------
// Platform Specific Entrance Point
// Like "main", but for Windows Applications
int WINAPI WinMain( HINSTANCE instance, 
   HINSTANCE prev_instance, 
   LPSTR cmd_line, 
   int show_cmd )
{
   int left_position = 100;
   int top_position = 100;

   // must happen before we can create a window
   // you can put this as part of CreateTheWindow if you know you'll
   // only ever create the one.
   RegisterGameWindowClass();

   HWND hwnd = CreateTheWindow( "SAMPLE WINDOW", left_position, top_position, 1280, 720 );

   // This bit benefits the most from making it a class - knowing when a processing messages 
   // results int he window itself being closed so we can stop processing.
   while (::IsWindow(hwnd)) {
      ProcessWindowMessages(hwnd);
      // Do other stuff.
   }

   // This is overkill - since it was destroyed before it got here
   // but just want it to match the create above (and more match what this looks
   // like when it becomes an class)
   ::DestroyWindow(hwnd);
   hwnd = NULL;


   UnregisterGameWindowClass();
}