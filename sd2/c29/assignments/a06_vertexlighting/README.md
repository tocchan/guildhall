C29.SD2.A06 Blinn-Phong Lighting
======

## Overview
Implement Per-Pixel Lighting using a hsader; 

**Turn-In Branch Name: `sd2/turnin/a06`**

**Due Date: Apr 09, 2020**

### Goal [100/100]
- [ ] Have a Quad, Sphere, and Cube rendering in world.
- [ ] Be able to switch to cycle active shader to show the following...  Use keys `<` and `>` for this
    - [ ] Current shader and hotkeys are shown on screen using debug screen text.
    - [ ] Diffuse color only (no lighting) - this is your `default` shader
    - [ ] Vertex Normals (transformed by model)
    - [ ] Vertex Tangents (transformed by model)
    - [ ] Vertex Bitangents (transformed by model)
    - [ ] Surface Normals (uses TBN)
- [ ] Each object should be rotating around the `Y` and `X` axis at different speeds.  This is to correct a correct application to model matrices to TBN space.
- [ ] Be able to adjust global ambient light. 
- [ ] There should be one point light in the scene that you can adjust.
    - [ ] Point light color and position is represented in world using a `DebugDrawWorld...` call.  Point or Sphere work well. 
        - [ ] Don't do this if the light is following the camera it is will just get in the way. 
    - [ ] Keyboard `-` and `+` should change its intensity
    - [ ] A console command `light_set_color color=rgb` to set the light color.  
    - [ ] Be able to switch the light's positioning mode... Keys are just suggestions, feel free to use whatever as long as it is in your readme. 
        - [ ] `F5` - Origin (light is positioned at (0,0,0))
        - [ ] `F6` - Move to Camera (light is moved to where the camera currently is, and stays there)
        - [ ] `F7` - Follow Camera (light is moved to camera each frame while in this mode)
        - [ ] `F8` - Animated.  Light follows a fixed path through the enfironment (cirlce your objects, or figure 8 through the objects)

### Extras
No extras for this assignment.  Initial goal is worth the full 100 points. 


## Resources

### Links
- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting : Decent tutorial of terms
- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model : Wikipedia definition; 
- https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-packing-rules : HLSL Packing rules for constant buffers in detail; 
- http://filmicworlds.com/blog/linear-space-lighting-i-e-gamma : Gamma and sRGB Explanation
  - https://blog.demofox.org/2018/03/10/dont-convert-srgb-u8-to-linear-u8 : Cautionary Tale about pre-converting sRGB to RGB with U8 formats; 
  - No Reading Material - But you can use `DXGI_FORMAT_R8G8B8A8_UNORM_SRGB` formats (and equivalent) to automatically convert linear values to sRGB space on store, and sRGB to linear space on load. 