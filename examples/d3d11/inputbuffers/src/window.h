#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//------------------------------------------------------------------------
// A window class defines a "type" of window our program will use
// For games, we usually just have the one.
//------------------------------------------------------------------------
bool RegisterGameWindowClass();

//------------------------------------------------------------------------
// Not needed since we're exiting the program anyway - but I like to 
// be thorough
void UnregisterGameWindowClass();

//------------------------------------------------------------------------
// Create the window at (x,y) with a client size of (res_x, res_y)
HWND CreateTheWindow( char const *title, int x, int y, int res_x, int res_y );

//------------------------------------------------------------------------
int ProcessWindowMessages( HWND hwnd );
