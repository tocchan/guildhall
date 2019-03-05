A05 Outline
======

*Note: This is my recommended order, which you should read as "The order I did it in".  This does not make it the best order, just what worked for me.  Feel free to tackle this assignment in your own way.  The only things that matters is that
you have the ability to easily draw debug shapes in world or screen space.*

## Debug Render System

- Start with DebugRenderQuad in World Space
  - Sprinkle some `DebugRender...` calls in you `Game::Update`
    - `DebugRenderPoint( 5.0f, m_cam_position, rgba::GREEN, rgba::RED )` -> Renders a point for 5 seconds, transitioning from Green to Red at the camera's posiition is a good one to get working.  By default it will use the depth buffer;
    - *Note: Recommend drawing the point as either 6 intersecting lines (a 3D asterik) or a camera facing quad (my preferred one)
  - Make the generic `DebugRenderQuad` the the above will call into; 
  - Design/Implement a system that would support this (sorry this seems fairly loose - the whole goal of the assignment is to be open on the implementation)
  - Make it work for different render modes (`ALWAYS` and `XRAY`)
  - Try a few different shapes...
    - `DebugRenderQuad` is a good easy followup to a point; 
    - `DebugRenderSphere` after that...
    - `DebugRenderWireSphere` after that.  Easier if you can support `Raster State`, and can just render a sphere with fill mode set to wire.  Otherwise, you can draw lines to visualize the area of the sphere.
  - Next, `DebugRenderTextv` 




## Variablic Templates

```cpp

void Foof( char const *format, ... )
{
	va_list args; 
	va_start( args, format ); 
	Foov( format, args ); 
	va_end( args ); 
}


template <typename ...ARGS>
void Foof( char const *format, ARGS ...args )
{
	Foo( Stringf(format, args...) ); 
}

// ...where
void Foo( std::string const &str )
{
	// ...
}
```

