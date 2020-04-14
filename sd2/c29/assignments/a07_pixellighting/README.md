C29.SD2.A07 Lights, Fog, and Effects
======

## Overview
Some more adanced shading techniques and more supported lights. 

**Turn-In Branch Name: `sd2/turnin/a07`**

**Due Date: Apr 16, 2020**

### Goal [80/80]
- [ ] Be able to support up to at least 8 concurrent lights
- [ ] Be able to support different light types...
    - [ ] Point Light...
        - [ ] Local 
        - [ ] Infinite
    - [ ] Infinite Directional Light...
    - [ ] Spot Light
       - [ ] Local
       - [ ] Infinite
    - *Have all these visible in the scene concurrently*
    - *Note: Local and Infinite are just attenuation values...*
- [ ] Shader should be branchless using a unified lighting structure 
- [ ] Support linear fog...
      - [ ] `RenderContext::EnableFog( float nearFog, float farFog, rgba nearFogColor, rgba farFogColor );` 
      - [ ] `RenderContext::DisableFog()` (shader is just 'always' going to do fog, so what do you set this to to make it not have an effect?)
- [ ] Dissolve Shader Effect
    - [ ] Support a `Material Block` uniform buffer with your `RenderContext`
        - Suggest a `RenderContext::SetMaterialBuffer( RenderBuffer* buf )`  
    - [ ] When binding a dissolve pattern to use, be sure to use a free slot.   
        - Suggest having a slot titled `USER_SLOT_START`, defined to likely 8.  That way when binding a texture 
          a user can just say `USER_SLOT_START + idx` for the slot they want.  
          You can also move your common textures (diffuse, normal, et.al.) to a later slot if you like users starting at 0. 
    - [ ] Have your dissolve shader expose the following controls...
        - [ ] A dissolve "depth" or value set to 0 to 1 to control how dissolved something is.
        - [ ] A dissolve "range" to give a *burned edge* to the dissolve effect.  This is the range near the edge at which is interpolates between burn_start_color and burn_end_color.  
        - [ ] A burn start color.
        - [ ] A burn end color
        - *Note, the full range you will be moving through with this effect is not 0 to 1, but `(1 + 2 * dissolve_range)` (why?).  You can think of it kinda like a scroll bar.  Meaning - how does the `dissolve_value` or depth that is 0 to 1 actually affect our dissolve range we're using in the shader?*

### Extras
- [ ] *X07.10: 05%*: Triplanar Shader
- [ ] *X07.11: 05%*: Projected Texture Effect
- [ ] *X07.12: 05%*: Interior Mapping Shader
- [ ] *X07.13: 03%*: Fresnel or Highlight Shader
- [ ] *X07.20: 05%*: Parallax Mapping Shader
    - [ ] *X07.21 05%*: Self Occluding Parallax Shader, aka Deep/Steep Parallax Shader
- [ ] *X07.30: 05%*: Support a Cube Map
    - [ ] *X07.31: 05%*: Support skybox clearing mode for a camera
    - [ ] *X07.32: 03%*: Support reflections into a skybox (can use specular factor to determine how shiny something is)

## Resources
- [CubeMap in D3D11 Notes](./cubemap_notes.md)    

### Links
- https://catlikecoding.com/unity/tutorials/advanced-rendering/triplanar-mapping/ : Triplanar Mapping Tutorial
- http://web.cse.ohio-state.edu/~shen.94/781/Site/Slides_files/projTex.pdf : Projected Texture Mapping example (just to show how it looks, and theory behind it), **but has no useful implementation details**.  You'll be implementing it with a shader.  
- https://learnopengl.com/Advanced-Lighting/Parallax-Mapping/ : Parallax Shading Tutorial #1.  Includes section on deep or steep parallax mapping.
- https://www.habrador.com/tutorials/shaders/3-parallax-mapping/ : Parallax Shading Tutorial #2
- http://interiormapping.oogst3d.net/ : Interior Mapping original paper. 
- https://www.habrador.com/tutorials/shaders/2-interior-mapping/ : Simpler to follow interior mapping shader for Unity.
- https://learnopengl.com/Advanced-OpenGL/Cubemaps: OpenGL CubeMap tutorial to get an idea how they work.
