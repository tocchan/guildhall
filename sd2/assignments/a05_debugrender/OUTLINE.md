A05 Outline
======

## Debug Render System

- Start with DebugRenderQuad in World Space
  - Sprinkle some `DebugRender...` calls in you `Game::Update`
    - `DebugRenderQuad( 5.0f, m_cam_position, rgba::GREEN, rgba::RED )` -> Renders a point for 5 seconds, transitioning from Green to Red at the camera's posiition
- 




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

