A06: Normal Maps
======

## Overview

### Build
- [./builds/a07.zip](./builds/a07.zip)

### Resources

## Rubric

### [90%] Required
- [ ] 5%: Scene contains 3 Primitives with correct Normals & Tangents
    - [ ] Quad
    - [ ] UV-Sphere
    - [ ] Cube
- [ ] 5%: Object are rotating (around Y and/or X is fine)
- [ ] 10%: A "flat" texture is created by defualt (like "white"), that is the default normal map to use
- [ ] 10% Be able to display Nroamls/Tangents/Bitangents
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


## Extras

### Shaders
- [ ] 02%: Laser Sword Cut Effect
- [ ] 03%: Dissolve Effect
- [ ] 04%: Tri-Planar Shading 
- [ ] 04%: Parallax Shader
    - [ ] 02%: Self Occluding Parallax Shader, aka Deep Parallax Shader
- [ ] 05%: Interior Mapping (Cube) Effect

### Material 
- [ ] 04%: Material can define a color for a texture slot 
- [ ] 02%: Material can define a sampler by name ("point", "linear", "trilinear")
- [ ] 04%: Material can define a sampler definition (support at least mip levels, min-mag-mip filters, and u-v-w wrap modes)
- [ ] 05%: Shader Reflection - Generate a  BufferLayout for a given Constant Buffer in a Shader (requires shader bytecode)
    - [ ] 02%: Material can set uniforms by name in code; 
    - [ ] 02%: Can set properties using the XML



## Extra Information

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
