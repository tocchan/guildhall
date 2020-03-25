C29.SD2.A05 Debug Render System
======

## Overview
Implement the [./DebugRender.hpp](./DebugRender.hpp) file to spec.  Some lines are extras. 

**Turn-In Branch Name: `sd2/turnin/a06`**

**Due Date: Mar 26, 2020**

### Goal 
Some example code for how the system will be used in this assignment;

------

## Checklist [90%]
- [ ] RenderContext
    - [ ] `RasterState` moved off shader, and added to `RenderContext`
    - [ ] `RenderContext::SetCullMode`
    - [ ] `RenderContext::SetFillMode`
    - [ ] `RenderContext::SetFrontFaceWindOrder`
    - [ ] Create a default raster state to set when `BeginCamera` is called.
    - [ ] Have a transient raster state to create/set when above calls are used.

- [ ] World Rendering
    - [ ] Points
    - [ ] Lines
    - [ ] Arrows
    - [ ] Basis
    - [ ] Quad
    - [ ] Wire Box
    - [ ] Wire Sphere
    - [ ] Text
    - [ ] Billboarded Text
    - [ ] All world commands support all rendering modes; 

- [ ] Screen Rendering
    - [ ] Points
    - [ ] Lines
    - [ ] Quads
    - [ ] Textured Quads
    - [ ] Text

- [ ] Output
    - [ ] Implement `DebugRenderWorldTo` to draw the debug objects into the passed camera's space.
    - [ ] Implement `DebugRenderScreenTo` to add the screen-space debug calls to the passed in texture.
    - [ ] Add a `DebugRenderWorldTo` call to your game after you render your scene
    - [ ] Add a `DebugRenderScreenTo` call to your App before your present to render 2D objects

- [ ] Controls
    - [ ] Console command: `debug_render enabled=bool` 
    - [ ] Console command: `debug_add_world_point position=vec3 duration=float`
    - [ ] Console command: `debug_add_world_wire_sphere position=vec3 radius=float duration=float`
    - [ ] Console command: `debug_add_world_wire_bounds min=vec3 max=vec3 duration=float`
    - [ ] Console command: `debug_add_world_billboard_text position=vec3 pivot=vec2 text=string`
    - [ ] Console command: `debug_add_screen_point position=vec2 duration=float`
    - [ ] Console command: `debug_add_screen_quad min=vec2 max=vec2 duration=float`
    - [ ] Console command: `debug_add_screen_text position=vec2 pivot=vec2 text=string`


## Extras
- [ ] *X05.00: 02%*: Screen Basis functions implemented
- [ ] *X05.00: 05%*: Message functions implemented
- [ ] *X05.00: 04%*: Border Around Text (one or two texel black border around text drawn in the world)
- [ ] *X05.00: 03%*: Option to include a background behind rendered text (color with alpha)
- [ ] *X05.00: 05%*: Option to pass bounds to text, and pass an alignment.  Text should wrap and align to within the box.  If the text would be larger than the box, you may handle this in your preferred way (clip overflow, center it, or shrink to fit)
- [ ] *X05.00: 02%*: `DebugAddWorldLineStrip`
- [ ] *X05.00: 02%*: `DebugAddWireMeshToWorld( mat44 mode, GPUMesh* mesh, rgba start_tint, rgba end_tint, float duration, eDebugRenderMode mode );` 
- [ ] *X05.00: 02%*: `DebugRenderAddGrid`, see notes.
    - [ ] *X05.00: 03%*: Grid is clipped to area the camera can potentially see (allowing for an infinitly large grid)
- [ ] *X05.00: 03%*: MeshUtils: `AddCylinderToIndexedVertexArray`
- [ ] *X05.00: 03%*: MeshUtils: `AddConeToIndexedVertexArray`
- [ ] *X05.00: 04%*: `RGBA LerpAsHSL( RGBA const &a, RGBA const &b, float t );` 

------

## Resources

### Getting a Render Context

```cpp
// Option A if possible
void DebugRenderWorldTo( Camera* camera ) 
{
  // if this possible - this is my preferred way...
  RenderContext* rc = camera->GetRenderContext();
  if (gRenderSystem->NeedsToBeFinalized()) {
    gRenderSystem->Finalize(rc);
  } 

  // .. do other stuff
  Camera* debug_camera = gDebugSystem->GetCamera();

  // copy all relevant settings from passed in camera
  debug_camera->SetModelMatrix( camera->GetGetModelMatrix() ); 
  debug_camera->SetColorTarget(...);
  debug_camera->Set... 

  debug_camera->SetClearMode( CLEAR_NONE ); 
  rc->BeginCamera( debug_camera ); 
  // 
  rc->EndCamera(); 
}

// option B - pass the context during startup
void DebugRenderStartup( RenderContext* rc ) 
{
  gRenderSystem->Finalize(rc); 
}
```

### Usage Examples
Idea is this system is usable anywhere without have to "worry" about the rendering details.

For example, for physics...
```cpp
// physics2d.cpp
void Physics2D::GetContacts()
{
  foreach (iobj) {
    foreach (jojb != iobj) {
      vec2[] contacts = GetContacts( iobj, jobj ); 

      // easily spot check the contact points by showing them in the world
      foreach (contact : contacts) {
        DebugRenderPoint( contacts[i], rgba::YELLOW, 1.0f ); 
      }
    }
  }
}
```

For this to show up, the app has to be do a couple things;

1. Initialize the system in startup.
2. Call `BeginFrame` and `EndFrame` to do the cleanup
3. Call `DebugRenderWorldTo(camera)` for the camera you want that point to show up in.  

If you use screen debug text (useful for temporary UI), you would also need to call

4. Call `DebugRenderScreenTo(texture)` sometime before you present.



### Registering Commands with Arguments

Assume I have a command that is registered - I just need to get it the `NamedProperties`, or `NamedStrings`.

```cpp
void SomeEventFunc( NamedProperties& properties ) { /*...*/ }
```

So `ConsoleRun(std::string const& cmd)` needs to do something like...

```cpp
void ConsoleRun( std::string const& cmd ) 
{
  // PSUEDO-CODE!!!
  std::string[] cmds = cmd.SplitFirst(' '); // split on the first space, so "cmd arg=value" becomes "cmd" and "arg=value"
  std::string commandName = cmds[0].GetTrimmed(); // trim whitespace

  // create commands from the remaining part of the string, "arg0=value0 arg1=value1 ..."
  NamedProperties props;
  props.SetFromString( &cmds[1] ); // simple version, up to equals is arg name, skip the equal, next word up to white space is value.  Can be more complex in future.

  FireEvent( commandName, props )
}
```

### Points (2D vs 3D)
In 2D, drawing a point should be a disc or a quad, something large you would see in pixel space (so usually a default size of 4 to 8 is good).

In world space, the our scale is usually much smaller, and we can look at objects from the side, so make sure whatever you draw can be seen from any direction.  Some options;

A. Draw a screen facing quad.
B. Draw a sphere/cube
C. Draw a hatch of multiple lines in different directions so it spans space in all axis's.

You can default to something smaller here to (possibly 0.125 or .25).  Having a global debug scale may also be useful if you have very different scales between games. 


### Wireframe
Wireframe is just setting the `Fill Mode` do wire.  This is part of the `ID3D11RasterState`.

See [D3D11_RASTERIZER_DESC](https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc).  

This controls how triangles are filled.  By default this is `SOLID`, meaning all pixels show up, but for we may want to switch it to `WIREFRAME` which only draws pixels for the edges.  Useful for showing bounds or mesh complexity. 


### XRay View Mode
X-Ray is a trick.  We want to show objects always, but still inform the user if they're in-front or behind of what they're looking at. 

To do this, we'll render the object twice.

1. Render the object again with normal depth rules.  Render this as normal.

2. Render the object using depth, and only if it would fail the depth test.  We do not write depth in this case.  Useful to also possibly tint the object or use a special shader to make it look different. 



### Removing from Production Builds
If you were implement this system for real - you would likely want to remove the debug draw calls from a release build.  Preprocessor is great for this.

------
## Extras
### Grid

This is an example of a generic grid function, though it is somewhat clunky to use.  The intent is that you create other functions
that call into it.  

A major axis is usually a multiple of the minor axis, and renders a differnent color line - think the different between millimeters and centimeters on a yard stick - helps
people count distance a little easier. 

```cpp
DebugRenderAddWorldGrid( vec3 origin, 
         vec3 i, float iMin, float iMax, float iMinorSegment, iMajorSegment, rgba iMinorColor, rgba iMajorColor, rgba iOriginColor,
         vec3 j, float jMin, float jMax, float jMinorSegment, jMajorSegment, rgba jMinorcolor, rgba jMajorColor, rgba jOriginColor, 
         rgba axisColor );
```

Feel free to add helpers that call into the above, for example;

```cpp
void DebugRenderAddWorldXZGrid() 
{
   // renders a grid with major segments every 5 units (bright red/blue) and a line every 1 unit (darker red bleu)
   // with the major axis rendered as white (lines that would pass through zero)
   // Renders for 50 units around the origin.
   // (if you support infinite grid, pass -INFINITY to INFINITY on those axis)
   DebugRenderAddWorldGrid( vec3::ZERO, 
      vec3::I, -50.0f, 50.0f, 1.0f, 5.0f, .5f * RED, RED, WHITE
      vec3::K, -50.0f, 50.0f, 1.0f, 5.0f, .5f * BLUE, BLUE, WHITE ); 
}

```
