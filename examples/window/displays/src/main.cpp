#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "log.h"
#include "aabb2i.h"

//------------------------------------------------------------------------
static BOOL CALLBACK EnumToFindDesktopResolution( HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM arg )
{
   aabb2i *data = (aabb2i*) arg;

   MONITORINFO info;
   memset( &info, 0, sizeof(info) );
   info.cbSize = sizeof(MONITORINFO);
   if (::GetMonitorInfoA( monitor, &info )) {
      if (info.dwFlags & MONITORINFOF_PRIMARY) {
         data->top = rect->top;
         data->bottom = rect->bottom;
         data->left = rect->left;
         data->right = rect->right;

         // Stop Enumerating
         return FALSE;
      }
   }

   // Go to the next element
   return TRUE;
}

//------------------------------------------------------------------------
bool GetPrimaryMonitorResolution( aabb2i *out_resolution )
{
   aabb2i resolution;
   ::EnumDisplayMonitors( NULL, NULL, EnumToFindDesktopResolution, (LPARAM) &resolution ); 

   if (resolution.get_width() > 0U) {
      *out_resolution = resolution;
      return true;
   } else {
      return false;
   }
}


//------------------------------------------------------------------------
// Platform Specific Entrance Point
// Like "main", but for Windows Applications
int WINAPI WinMain( HINSTANCE instance, 
   HINSTANCE prev_instance, 
   LPSTR cmd_line, 
   int show_cmd )
{
   UNREFERENCED_PARAMETER( instance );
   UNREFERENCED_PARAMETER( prev_instance );
   UNREFERENCED_PARAMETER( cmd_line );
   UNREFERENCED_PARAMETER( show_cmd );
   
   aabb2i res;
   if (GetPrimaryMonitorResolution( &res )) {
      Logf( "Monitor Resolution is (%i, %i).", res.get_width(), res.get_height() );
   } else {
      Log( "Failed to find resolution." );
   }
}