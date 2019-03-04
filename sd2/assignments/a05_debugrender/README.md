A05: Debug Render System
======



## Rubric

*Note: I'm looking for functionality, not you matching the header signature exactly.  If you prefer using a namespace, a Static class, or a singleton, that is up to you.  What I will be looking for is that it is usable anywhere, and it is as simple as one line to say "Print out the time of impact at the point of impact, for 4 seconds".*

### [40%] 3D
- Command to render a Point in 3D Space (for a given time, start color, and end color)
- Command to render a billboarded quad in 3D space (for a given time, start color, end color, and optional texture)
- Command to render a line in 3D space (for a given time, start color, and end color)
- Command to render a spherical wire volume in 3D space 
- Command to render billboarded text in 3D space (for a given time, start color, end color, and pivot point)
- All the above can be rendered either "Ignoring Depth", "Using Depth"
- All the above can be rendered using "XRay" mode
- Dev Console Command to clear all live commands (mostly matters for long lived commands)

### [20%] 2D
- Command to render a point in Screen Space (for given time, start color, and end color)
- Command to render a line in Screen space (for a given time, start color, and end color)
- Command to render a textured quad in Screen Space (for given time, start color, end color, and optional texture)
- Command to render text in screen space (for given time, start color, end color, and pivot)

### [20%] Message System
- Command to render a message (for a given time, start color, and end color)
- Messages listed in order added

### [20%] Extras 
- See below...


### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Topics
- Nothing new - just use the systems you have to implement the `DebugDraw` system; 

## Additional Links
- None


## Goal(s)

### Usage
Should be able to add debug information to the screen very simply;  For example;

```cpp
void Physics2D::OnCollision(...)
{
   vec2 collision_point; 
   vec2 normal; 

   // print the impact point, an arrow along impact normal, and the 
   // resultant velocity in the world for 5 seconds; 
   DebugRenderPoint( 5.0f, collision_point, rgba::RED ); 
   DebugRenderArrow( 5.0f, collision_point, normal, rgba::WHITE ); 
   DebugRenderTextf( 5.0f, collision_point, 2.0f, rgba::WHITE, "Resultant Velocity: %.2f, %.2f", vel.x, vel.y ); 
}
```


### Hookup
Places we need to call `DebugRender` system calls so that things show up; 

```cpp
void App::Startup()
{
   //...

   // Initialize the system; 
   DebugRenderSystemStartup( g_theRenderContext ); 
}

void App::Shutdown()
{
   // cleanup the system; 
   DebugRenderSystemShutdown(); 

   // ...
}

void App::Render()
{
   // Render Everything

   // Game Render
   // ...

   // Render screen space debug data
   // (usually text or quads)
   // AFTER you render your game, but before you render
   // your dev console; 
   DebugRenderToScreen(); 

   // Dev Console Render
   // ...

   // Present
   g_theRenderer->EndFrame(); 

   // At the end of a frame, we can run a purge (could also be done in BeginFrame, though I prefer
   // right after rendering as there is no room for an object to be added and immediately removed; 
   DebugRenderSystemEndFrame(); 
}

void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext(); 

   ctx->BeginCamera( m_camera ); 

   // Draw some stuff
   // ...

   ctx->EndCamera();    

   // IF!! we want to show debug information to this camera, 
   // we call this (ie, don't do it for every camera - the DevConsole camera for 
   // instance does not need it)
   DebugRenderToCamera( m_camera ); 
}
```

## Extras

### Glyphs
- [4%] DebugRenderGlyph
- [4%] DebugRenderLabel

### Extra Utility (3D)
- [X05.20  2%] DebugRenderSphere (3D)
- [X05.20  2%] DebugRenderBox (3D)
- [X05.22  2%] DrawArrow (3D)
- [X05.23  2%] DrawBasis (3D)
- [X05.24  2%] DrawGrid (3D)

### Extra Utility (2D)
- [X05.30  2%] DebugRenderScreenArrow
- [X05.31  2%] DebugRenderScreenDisc

### New Meshes
- [2%] MeshAddCylinder
- [2%] MeshAddCone

### Text
- [4%] Text Wrap
- [2%] Border Around Text (one or two texel black border around text drawn in the world)
- [2%] Option to include a background behind rendered text (color with alpha)


## Extra Information
**Coming Soon**
