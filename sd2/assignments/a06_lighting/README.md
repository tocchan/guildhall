A06: Lightings
======

## Overview
- Blinn-Phong Lighting using Vertex Normals
  - Ambient Light
  - Directional Light
  - Point Lights
- Custom Vertex Formats
- Normal Generation 

This will be shown to be working by having a scene with a Quad, a UV-Sphere, and a Box, all lit by at least one directional light, and at least 4-point lights; 


### Resources
- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting : Decent tutorial of terms
- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model : Wikipedia definition; 

## Rubric

### [90%] Required
- [ ] Scene contains 3 Primitives
    - [ ] Quad
    - [ ] UV-Sphere
    - [ ] Cube
- [ ] Can adjust ambient light (console command or hotkey)
- [ ] Scene has at least one direction light
    - [ ] Directional light color is settable by console command
    - [ ] Directional light direction is settable by console command
- [ ] Scene has up to 4 dynamic point lights moving around
    - [ ] Can adjust number of lights with console command/hotkey. 
    - [ ] One light can be forced to the position of the camera with a keypress.
- [ ] Diffuse Lighting is implemented
- [ ] Specular Lighting is implemented 
- [ ] Can swap between rendering normals as color, and rendering lighting (hotkey or console command)


### [10%] Extras 
- See below...

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Goal(s)

### Usage


## Extras

- [X06.00  5%] Screenshot
- [X06.00  4%] Spot Lights
- [X06.00  5%] Projected Textures
- [X06.00  4%] DebugRenderScreenBasis - render your current camera's orientation to the screen. 
- [X06.00  4%] Unified light structure (Global and Local Directional, Point, and Cone lights)
- [X06.00  2%] Branchless (no if statement) lighting equations for above (more a fun excercise in this case then an optimization)
- 

## Extra Information
