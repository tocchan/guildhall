A06: Lightings
======

## Overview
- Blinn-Phong Lighting using Vertex Normals
  - Ambient Light
  - Directional Light
  - Point Lights
- Custom Vertex Formats
- Normal Generation 
- Gamma and sRGB

This will be shown to be working by having a scene with a Quad, a UV-Sphere, and a Box, all lit by at least one directional light, and at least 4-point lights; 


### Resources
- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting : Decent tutorial of terms
- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model : Wikipedia definition; 
- https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-packing-rules : HLSL Packing rules for constant buffers in detail; 
- http://filmicworlds.com/blog/linear-space-lighting-i-e-gamma : Gamma and sRGB Explanation
  - https://blog.demofox.org/2018/03/10/dont-convert-srgb-u8-to-linear-u8 : Cautionary Tale about pre-converting sRGB to RGB with U8 formats; 
  - No Reading Material - But you can use `DXGI_FORMAT_R8G8B8A8_UNORM_SRGB` formats (and equivalent) to automatically convert linear values to sRGB space on store, and sRGB to linear space on load. 

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

```cpp

// create meshes with normals
void Game::Startup()
{
  RenderContext *rc = App::GetRenderContext(); 
  CPUMesh mesh; 

   // we're using a format with normals; 
  mesh.SetLayout<VertexLit>(); 

  // updated to set normals
  MeshAddUVSphere( &mesh, 
    vec3::ZERO,     // ...centered at zero 
    5.0f,           // ...with radius 5 
    32, 16 );       // ...with 32 wedges, 16 slices 

  m_sphere = GPUMesh::FromCPUMesh( rc, mesh ); 
}

// Using Lights
void Game::Render()
{
  RenderContext *rc = App::GetRenderContext(); 
  rc->BeginCamera( m_camera ); 

  rc->SetAmbienteLight( rgba::WHITE, .01f ); 

  // enable a point light as some position in the world with a normal quadratic falloff; 
  rc->EnablePointLight( m_lightPosition, m_lightColor, m_lightIntensity, vec3(0.0f, 0.0f, 1.0f) );

  rc->BindShader( "shader/lit.shader" ); 
  rc->DrawMesh( m_sphere ); 

  rc->EndCamera(); 
}
```


## Extras

### Free (From Class Notes - Cut for C29)
- [X06.00  4%] Unified light structure (free if following class notes)
- [X06.01  2%] Branchless (no if statements) lighting equations for above (free if following class notes)

### Utility
- [X06.10  5%] Projected Textures
- [X06.11  4%] DebugRenderScreenBasis - render your current camera's orientation to the screen. 
- [X06.12  4%] Remove `VertexMaster` and have `CPUMesh` work soley using a `BufferLayout`
- [X06.13  4%] UniformBuffer can optionally be given a buffer layout.  Will store a CPU copy of data if present and this can be used to set properties by name; 

### D3D Exploration
- [X06.20  6%] Screenshot
- [X06.21  4%] Hook up D3D11InfoQueue to print D3D11 errors to your dev console
- [X06.22  4%] Shader Reloading will now work for included files


## Extra Information
...todo... sorry C28...
