A05: Debug Render System
======



## Rubric
- [ ] **30%** World Space Debug Rendering (`USE_DEPTH` mode by default) works
- [ ] **20%** Screen Space Debug Rendedring (`ALWAYS` mode by default) works
- [ ] **20%** Debug commands can be given a duration
- [ ] **10%** World Space Render Modes supported (`ALWAYS`, `USE_DEPTH`, and `XRAY`)
- [ ] **20%** Extras

I'm looking for functionality, not you matching the header signature exactly.  If you prefer using a namespace, a Static class, or a singleton, that is up to you.  What I will be looking for is that it is usable anywhere, and it is as simple as one line to say "Print out the time of impact at the point of impact, for 4 seconds".  

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
- Support Glyph/Camera mode

### Extra Utility
- DrawWireBox
- DrawWireSphere
- DrawArrow (2D && 3D)
- DrawBasis (3D)
- DrawGrid

### New Meshes
- MeshAddCylinder
- MeshAddCone

## Extra Information
**Coming Soon**
