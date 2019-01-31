#include "demo.h"
#include "window.h"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// NEW CONTENT MARKERS
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//------------------------------------------------------------------------
static void CommonStartup()
{
   RegisterGameWindowClass();
}

//------------------------------------------------------------------------
static void CommonShutdown()
{
   UnregisterGameWindowClass();
}

//------------------------------------------------------------------------
// Platform Specific Entrance Point
// Like "main", but for Windows Applications
int WINAPI WinMain( HINSTANCE instance, 
   HINSTANCE prev_instance, 
   LPSTR cmd_line, 
   int show_cmd )
{
   CommonStartup();

   // See Demo for this Demonstrations Content
   DemoRun();

   CommonShutdown();
}