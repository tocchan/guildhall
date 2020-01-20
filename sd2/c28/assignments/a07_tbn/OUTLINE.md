A07 Outline
======

Note - materials, tangents, and emissive lights can all be done independently of each other, so you can pick your order. 

## Checklists 

### Fixup for A06
- [ ] Make sure at least one of your objects is being affected by a model matrix (rotation around Z & X) is good.

### Materials
- [ ] Create the `Material` class with appropraite members
- [ ] Write `RenderContext::BindMaterial` to bind all information associated with the `Material`
- [ ] Create a material xml file to load (see samples)
- [ ] Write code to create material from file; 
- [ ] Create a Material Database, ie, `RenderContext::GetOrCreateMaterial` or similar; 


### Tangents & Bitangents
- [ ] Generate Tangents (`vec4`) or Tangents and Bitangents (`vec3` and `vec3`) for you geoemetry.
- [ ] Create shader to display Normals, Tangents, Bitangents, and Surface (texture) Normals.
- [ ] Generate the TBN (Tangent-Bitangent-Normal) Basis (which could also be called a Surface-To-World transform)
- [ ] Transform your surface normal by the TBN to generate your world normal.
- [ ] Calculate lighting as normal; 


### Emissive Lights
- [ ] Be able to specify an emissive colour, and an emissive texture (defaulted to black).
- [ ] In shader, emissive colour is multiplied against the emissive texture colour to get our `emissive_light` value (so emissive colour is just a way to control the power of the emissive texture)
- [ ] The `emissive_light` is added on to the end, similar to specular; 


## Notes
- **Emissive Light** refers to the light the surface itself gives off.  
