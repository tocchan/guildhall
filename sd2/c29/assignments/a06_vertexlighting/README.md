C29.SD2.A06 Blinn-Phong Lighting
======

## Overview
Implement Per-Pixel Lighting using a hsader; 

**Turn-In Branch Name: `sd2/turnin/a06`**

**Due Date: Apr 09, 2020**

### Goal [100/100]
- [ ] Have a Quad, Sphere, and Cube rendering in world with normals and tangents;
    - [ ] Make a new `Vertex_PCUTBN` or `VertexLit`
    - [ ] Update or create new methods for generating these shapes, with normals/tangents/bitangents computed.
    - [ ] Create meshes using this new vertex format.
    - [ ] Be sure to update places where D3D11 needs to know format...
        - Setting vertex buffer needs to know correct stride
        - Creating an input layout needs the correct layout to tie it to the shader
    - [ ] Be sure your vertex buffer stores the correct format for the vertices stored in it
- [ ] Be able to switch to cycle active shader to show the following...  Use keys `<` and `>` for this
    - [ ] Current shader and hotkeys are shown on screen using debug screen text.
    - [ ] Normal lighting shader - `lit.hlsl`
    - [ ] Diffuse color only (no lighting) - this is your `default` shader
    - [ ] Vertex Normals (transformed by model) - `normals.hlsl`
    - [ ] Vertex Tangents (transformed by model) - `tangents.hlsl`
    - [ ] Vertex Bitangents (transformed by model) - `bitangents.hlsl`
    - [ ] Surface Normals (uses TBN) - `surface_normals.hlsl`
- [ ] Each object should be rotating around the `Y` and `X` axis at different speeds.  This is to correct a correct application to model matrices to TBN space.
- [ ] `9,0` - Be able to adjust global ambient light
    - [ ] Console command `light_set_ambient_color color=rgb` to set ambient color to whatever you want
- [ ] There should be one point light in the scene that you can adjust.
    - [ ] Be able to toggle attentuation using `T`
        - [ ] Default to `linear attenuation`, or (0, 1, 0)
        - [ ] Cycle from linear -> quadratic -> constant, ie (0,1,0) -> (0,0,1) -> (1,0,0)
        - *Note: Suggest adding a `light_set_attenuation` command that allows you to set it arbitrarily so you can see how it affects the light behaviour.*
    - [ ] Point light color and position is represented in world using a `DebugDrawWorld...` call.  Point or Sphere work well. 
        - [ ] Don't do this if the light is following the camera it is will just get in the way. 
    - [ ] Keyboard `-` and `+` should change its intensity
    - [ ] A console command `light_set_color color=rgb` to set the light color.  
    - [ ] Be able to switch the light's positioning mode... Keys are just suggestions, feel free to use whatever as long as it is in your readme. 
        - [ ] `F5` - Origin (light is positioned at (0,0,0))
        - [ ] `F6` - Move to Camera (light is moved to where the camera currently is, and stays there)
        - [ ] `F7` - Follow Camera (light is moved to camera each frame while in this mode)
        - [ ] `F8` - Animated.  Light follows a fixed path through the enfironment (cirlce your objects, or figure 8 through the objects)
- [ ] Be able to adjust object material properties...
    - [ ] `[,]` keys should adjust specular factor between 0 and 1 (how much does specular light show up
    - [ ] `',"` keys should adjust specular power (min of 1, but allow it to go as high as you want.

### Extras
No extras for this assignment.  Initial goal is worth the full 100 points. 


## Resources

### End Goal `Game::Render`
This interface is just a suggestion - if you want to handle your light information
differently but can still support everything I'm asking for above, that is fine. 

```cpp
m_renderer->BeginCamera( m_game_camera ); 

// setup for rendering opaque boecjts
m_renderer->SetCullMode( CULL_BACK, WIND_COUNTER_CLOCKWISE ); 
m_renderer->SetDepthMode( COMPARE_GREATER_EQUAL, true ); 

// setup lights for the scene
m_renderer->DisableAllLights(); 
m_renderer->SetAmbientLight( m_ambient_light, m_ambient_light.a ); 
m_renderer->EnableLight( 0, m_point_light ); 

// set to use our lit shader
m_renderer->BindShaderByName( "shaders/lit" ); 

// bind our textures for the object
m_renderer->BindSamplerByMode( 0, SAMPLE_LINEAR ); 
m_renderer->BindDiffuseTextureByName( m_diffuse_map.c_str() ); 

// bind the model matrix & draw
m_renderer->SetModelMatrix( m_plane_model ); 
m_renderer->DrawMesh( m_mesh_plane ); 

m_renderer->EndCamera(); 
```


### Links
- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting : Decent tutorial of terms
- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model : Wikipedia definition; 
- https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-packing-rules : HLSL Packing rules for constant buffers in detail; 
- http://filmicworlds.com/blog/linear-space-lighting-i-e-gamma : Gamma and sRGB Explanation
  - https://blog.demofox.org/2018/03/10/dont-convert-srgb-u8-to-linear-u8 : Cautionary Tale about pre-converting sRGB to RGB with U8 formats; 
  - No Reading Material - But you can use `DXGI_FORMAT_R8G8B8A8_UNORM_SRGB` formats (and equivalent) to automatically convert linear values to sRGB space on store, and sRGB to linear space on load. 