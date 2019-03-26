A06 Outline
======

## Lighting

### Ambient Light (Initial Setup)
- [ ] *Recommend: Create a "shader/common/dot3.hlsl" include file so multiple shaders who want to apply lighting can just include this.*
- [ ] Create a constant buffer for lighting constants.
- [ ] Add `float4 AMBIENT` constant
- [ ] `RenderContext` manages a constant buffer for lighting data
    - [ ] Default ambient light to `vec4(1,1,1,1)`, ie, a fully lit white light (which will make things look like they have so far)
- [ ] `RenderContext::SetAmbientLight`
- [ ] `RendreContext::UpdateLightBuffer` called before a draw if lighting information has changed. 
- [ ] In the pixel shader, call `CalculateLight`, and apply the ambient light.
- [ ] Add console commands or hotkeys to control ambient lighting
- [ ] Test and Run

### Normals & Buffer Layouts
- [ ] Introduce `VertexLit` or `VertexPCUN` depending on your naming preference; 
- [ ] Introduce `BufferLayout` and `buffer_attribute_t` objects
- [ ] Fully describe `VertexLit` in a layout
- [ ] Create unique `BufferLayout` objects for given attribute lists or vertex structures; 
- [ ] Update `VertexMaster` to contain a `vec3 normal`
- [ ] Update mesh building functions to also set normal when applicable (or default to `vec3(0,0,-1)` when not (why?))
- [ ] `CPUMesh` now must specify a desired `BufferLayout` before mesh construction is allowed; 
- [ ] Update `GPUMesh` copy and create methods from `CPUMesh`
- [ ] When drawing a `GPUMesh`, make sure `Shader` updates its `ID3D11InputLayout` to match the current input format. 
    - *Note: there is multiple ways to handle this pairing.  Easiest would just be to remake an input-layout when you detect the vertex layout is different from last time this shader (or more specifically, this Vertex Shader Stage) was used;  You can also store a list of all layouts used with the shader and cache them off.  You can also use reflection to determine unique **Input Signatures** used, and create a global dual-key lookup, though DX11 does not make this particularly easy to do.  
- [ ] Write a shader that uses Normals, and renders normals as colour.  Be sure you are still able to draw, and your normals look correct. 


### Directional Light
- [ ] Add a directional light to the light constant buffer
- [ ] `RenderContext::EnableDirectionalLight` implemented
    - [ ] `RenderContext::DisableDirectionalLight` can just call the above with no intensity
- [ ] In the shader, implement Dot3 (Diffuse) Lighting using the directinal light ot get your incoming light vector and information; 
- [ ] Test!
- [ ] In the shader, implement Phong or Blinn-Phong specular calculation
- [ ] Test again!


### Point Lights
- [ ] Give an array of 8 point lights in your constant buffer
    - [ ] Default to having zero intensity so they're all *"disabled"* at the start. 
- [ ] Update shader to walk and accumulate lighting for all lights; 
- [ ] `RenderContext::EnablePointLight( uint idx, ... )` to set point light data.
- [ ] Add appropraite control commands/hotkeys
- [ ] test


### Extras
- [ ] Do some extras time permitting; 


```