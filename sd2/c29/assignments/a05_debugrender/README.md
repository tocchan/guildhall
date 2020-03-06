C29.SD2.A05 Debug Render System
======

## Overview
Implement the [./DebugRender.hpp](./DebugRender.hpp) file to spec.  Some lines are extras. 

**Turn-In Branch Name: `sd2/turnin/a06`**

**Due Date: Mar 19, 2020**

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
    - [ ] Console command: `debug_add_world_wire_disc position=vec3 radius=float duration=float`
    - [ ] Console command: `debug_add_world_wire_bounds min=vec3 max=vec3 duration=float`
    - [ ] Console command: `debug_add_world_billboard_text position=vec3 pivot=vec2 text=string`
    - [ ] Console command: `debug_add_screen_point position=vec2 duration=float`
    - [ ] Console command: `debug_add_screen_quad min=vec2 max=vec2 duration=float`
    - [ ] Console command: `debug_add_screen_text position=vec2 pivot=vec2 text=string`


## Extras
- [ ] Screen Basis functions implemented
- [ ] Message functions implemented
- [ ] `DebugAddWorldLineStrip`
- [ ] `DebugAddWireMeshToWorld( mat44 mode, GPUMesh* mesh, rgba start_tint, rgba end_tint, float duration, eDebugRenderMode mode );` 
- [ ] `DebugRenderAddGrid`, see notes.
    - [ ] Grid is clipped to area the camera can potentially see (allowing for an infinitly large grid)

------

## Resources

### Usage Examples

### Registering Commands with Arguments

### Points (2D vs 3D)

### Wireframe

### XRay View Mode

### Removing from Production Builds
If you were implement this system for real - you would

------
## Extras
### Billboarding

### World Basis

### Messages

### Wire Mesh

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
