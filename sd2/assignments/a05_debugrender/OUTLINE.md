A05 Outline
======

*Note: This is my recommended order, which you should read as "The order I did it in".  This does not make it the best order, just what worked for me.  Feel free to tackle this assignment in your own way.  The only things that matters is that
you have the ability to easily draw debug shapes in world or screen space.*

## Debug Render System

- 3D First
  - *Note: These objects only show up when calling `DebugRenderToCamera`*
  - Hook up the Debug Render system (in App and Game...
    - Call `Startup` and `Shutdown`
    - Call `DebugRenderToCamera` for your game camera in `Game::Render`
    - Call `DebugRenderToScreen` before `RenderContext::EndFrame` presents.  
    - Setup a camera for the `DebugRenderSystem` to use when rendering to the screen; 
    - Test that they hookup by clearing the screen to a solid colour somewhere in those methods
      - Do one at at time and use different colours; 
  - Call something like `DebugRenderPoint( 5.0f, m_cam_position, rgba::GREEN, rgba::RED )` in your `Game::Update`
    - Implement it - try to get just that call to work (system design is up to you)
    - *Renders a point for 5 seconds, transitioning from Green to Red at the camera's posiition is a good one to get working.  By default it will use the depth buffer;.*
    - *Recommend drawing the point as either 6 intersecting lines (a 3D asterik) or a camera facing quad (which is what I do in my demo);*
  - Now try to implement other 3D draw commands - `DebugRenderQuad` is a good next one after this; 
  - Make it work for different render modes (`ALWAYS` and `XRAY`).  This may change your system slightly
  - Next, `DebugRenderText` - determine how you want to handle the variable argument list (see bottom of document for options)

- 2D 
  - Similar to above, but these objects only show up when calling `DebugRenderToScreen`, and points are specified in whatever screen space you determined for your system; 
  - *Note:  Feel free to use any 2D coordinate system you want for you screen coordinates.  Some people prefer 720p, 1080, -1 to 1 (normalized), or even using the resolution of the window.  Use whatever system will be intuitive to you as the user of said system.*

- Go through the rubric and and make sure you support everything
- Make sure when you demo this you can show all features working; 
  - *During grading I may ask you to draw varius things, so I can see how easy the system is to use*


## Handling Variable Arguments (Printf/Stringf)

```cpp

// Using standard C, this is usually handled using 
// a "..." argument.  The macros to use this are in <stdarg.h>
// You can grab a va_list by passing the last parameter before the argument
// list, and then pass it forward; 
//
// You can use vsprintf and similar to construct your string 
// These methods take a va_list intead of the standard ... you usually see with printf; 
// (you can also enumerate these args yourself if you want to do something special
// but you need a way to determine the type and count (usually the format string)
void Foof( char const *format, ... )
{
   char buffer[1024]; 

	va_list args; 
	va_start( args, format ); 
	vsprintf_s( buffer, 1024, format, args ); 
	va_end( args ); 
}


// Another way (C++ only) is to use variadic templates; 
// These will resolve at compile time and will let you forward the arguments whole sale
// two another function; 
//
// In this case, your Stringf from SD1
template <typename ...ARGS>
void Foof( char const *format, ARGS ...args )
{
   // construct a string using the argumetns, and pass it to a standard function
	Foo( Stringf(format, args...) ); 
}

// ...where Foo t
void Foo( std::string const &str )
{
	// ...
}
```

