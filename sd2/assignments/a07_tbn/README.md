A07: Normal Maps
======

## Overview
Finish out our lighting by having pre-fragment normals, really bumping up the graphical fidelity for just a few extra lines of shader code; 

Also, we'll add a utility container object called the `Material`.  A `Material` is shader with its resources - and is really as a convenience to us to make changing between different objects easier; 

### Resources
- [./builds/a07.zip](./builds/a07.zip) - A07 Demo Build

## Rubric

### [90%] Required
- [ ] 5%: Scene contains 3 Primitives with correct Normals & Tangents
    - [ ] Quad
    - [ ] UV-Sphere
    - [ ] Cube
- [ ] 5%: An Object are rotating (around Y and/or X is fine)
- [ ] 10%: A "flat" texture is created by defualt (like "white"), that is the default normal map to use
- [ ] 10% Be able to display Normals/Tangents/Bitangents
- [ ] 5% Support for emissive color (global and texture)
- [ ] 10% Lighting works correctly with normal maps for at least camera facing faces
- [ ] 20% Lighting works correctly with normal maps on all faces;
- [ ] 10% Materials Implemented
- [ ] 15% Material from Data works

### [10%] Extras 
- See below...

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Goal(s)

### Usage

Materials are meant to just make drawing a little easier.  YOu can define a material either through data or through code, and it keeps track of all the resources needed for that draw call; 

For example, below, I can have two materials both using the "lit" shader;

```xml
<!-- material/couch.mat -->
<material id="material/couch" shader="shader/lit">
   <diffuse src="image/couch_color.png" />
   <normal  src="image/couch_normal.png" />
   <sampler idx="0" type="linear" />
</material>

<!-- material/brick.mat -->
<material id="material/brick" shader="shader/lit">
   <texture idx="0" src="image/brick_color.png" />
   <texture idx="1" src="image/brick_normal.png" />
   <sampler idx="0" type="linear" />
</material>
```

...which can then be used by the game...

```cpp
void Game::Startup()
{
   // Material *m_mat_brick;  Material *m_mat_couch
   RenderContext *ctx = App::GetRenderContext(); 
   m_mat_brick = ctx->GetOrCreateMaterial( "material/brick.mat" ); 
   m_mat_couch = ctx->Material::GetOrCreateMaterial( "material/couch.mat" ); 

   // create resources
}

void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext(); 
   // ...

   ctx->BeginCamera( m_camera ); 

   ctx->SetModelMatrix( mat44::Translation( 5.0f, 0.0f, 0.0f ) ); // where
   ctx->BindMaterial( m_mat_couch ); // how
   ctx->Draw( m_mesh_cube ); // what

   ctx->SetModelMatrix( mat44::Translation( -5.0f, 0.0f, 0.0f ) ); // where
   ctx->BindMaterial( m_mat_brick ); // how
   ctx->Draw( m_mesh_cube ); // what

   ctx->EndCamera(); 
}
```

When shaders start requiring a lot more resources (even now, we an have up to diffuse, normal, spec, emissive, and displacement), this helps clean the code up a lot.  


## Extras

### Lights
- [ ] 04%: Cone Lights

### Shaders
- [ ] 02%: Laser Sword Cut Effect
- [ ] 03%: Dissolve Effect
- [ ] 04%: Tri-Planar Shading 
- [ ] 04%: Parallax Shader
    - [ ] 02%: Self Occluding Parallax Shader, aka Deep Parallax Shader
- [ ] 05%: Interior Mapping (Cube) Effect

### Material 
- [ ] 04%: Material Instancing (ie: `Material::Clone`)
- [ ] 04%: Material can define a color for a texture slot 
- [ ] 02%: Material can define a sampler by name ("point", "linear", "trilinear")
- [ ] 04%: Material can define a sampler definition (min-mag-mip filters, and u-v-w wrap modes)
- [ ] 05%: Shader Reflection - Generate a  BufferLayout for a given Constant Buffer in a Shader (requires shader bytecode)
    - [ ] 02%: Material can set uniforms by name in code; 
    - [ ] 02%: Can set properties using the XML


## Extra Information

### Cone Lights
Be able to define an `inner` and `outer` angle and power for a light.  If a lit fragment is within `inner_angle`, it gets full `inner_power` applied to its light intensity.  If it it outside `outer_angle`, it will get the light intensity scaled by `outer_power`. 

When between the two angles, use the hlsl function `smoothstep` or `lerp` to blend between the two. 

**Note: This should be doable without any trigometric functions in the shader;**


### Cut Effect
Be able to define an arbitrary plane in which to "cut" an object.  Only parts of the object on the positive side of the plane should be be visible, and colour near the cut should be a solid colour, blending quickly to the object's normal colour; 

Will require use of the hlsl `clip` function.

### Dissolve Effect
See Demo.  Will require use of the hlsl `clip` function. 

### Parallax Shader
See Demo; 

**Resources;**
- https://learnopengl.com/Advanced-Lighting/Parallax-Mapping
- https://www.habrador.com/tutorials/shaders/3-parallax-mapping/

### Interior Mapping
See Demo; 

**Resources**; 
- http://interiormapping.oogst3d.net/
- https://www.habrador.com/tutorials/shaders/2-interior-mapping/
