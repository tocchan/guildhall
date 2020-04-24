C29.SD2.A08 Mesh Loading
======

## Tasks
- [ ] Be able to load `OBJ` Files
    - [ ] Load OBJ as a single vertex array
    - [ ] At end of each group (or at very end), apply post steps..
        - [ ] Invert-V
        - [ ] Calculate Normals
        - [ ] Calculate Tangents
    - [ ] At end of import...
        - [ ] Apply transform
        - [ ] *Clean* (is an extra) 
        - [ ] Invert Winding
- [ ] Post-Import Options supported
    - [ ] Invert-V
        - Some programs us UV with V at the top, and some bottom.  On import
          be able to swap it out to the correct version for your engine.
    - [ ] Invert Winding Order
        - Some programs use clockwise by default.  If you notice your mesh is 
          inside out (you can see it from the inside, but not outside), you will
          want to invert the winding order of faces.
    - [ ] Generate Normals (Flat only required)
        - Some OBJ files do not include normals.  If this import option is set, 
          you will want to generate normals if the file didn't include them.
          For **Flat Normals**, just calculate the normal for each face and assign it to
          each version that defines the face. 
    - [ ] Generate Tangents
        - Use MikkT to generate tangents.  This requires the mesh have normals, so generate
          normals should also be true. 
          Use [./mikktspace.h](./mikktspace.h) and [./mikktspace.c](./mikktspace.c) to
          do this step.  
    - [ ] Post import transform from the authoring engines space to our space.
        - [ ] Transform Positions (full)
        - [ ] Normals (just direction, not scaled or translated)
        - [ ] Tangents & Bitangents (just direction, not scaled or translated)
- [ ] Have a lit model loaded in your scene to show it is working
- [ ] Support a `Shader` or `ShaderState` class that is data driven
    - [ ] Define a data format you like
    - [ ] Define a class you like
    - [ ] Implement `RenderContext::BindShader` or `BindShaderState` that
          binds the program and the additional state for you.
    - [ ] Add `RenderContext::GetOrCreateShader(...)` to support a database of shaders
    - [ ] Implement `RenderContext::BindShaderByName` or `RenderContext::BindShaderStateByName`
- [ ] Support a `Material` class to make managing material data easier.
    - [ ] Should encapsulate a `ShaderState` and relevant data needed for the material
    - [ ] Define a data format you like.
    - [ ] Define a `Material` class you can use that works with our data
        - [ ] Must be able to support owning at least one UBO for material specific data?
        - [ ] Can set material specific textures
        - [ ] Can set material specific samplers
        - [ ] Can set your engine specific constants (spec/tint)
    - [ ] Can make a material from file
    - [ ] Can call `RenderContext::BindMaterial` to bind the shader and all data assocated with the material

## Suggested Interface
```cpp
// If `clean` is not supported
void MeshLoadToVertexArray( std::vector<VertexLit>& vertices, 
   mesh_import_options_t const& options = mesh_import_options_t ); 

// If `clean` is supported
void MeshLoadToIndexedVertexArray( std::vector<VertexLit>& vertices, 
   std::vector<uint>& indices, 
   mesh_import_options_t const& options = mesh_import_options_t() ); 

// MeshUtils.hpp
void MeshInvertV( std::vector<VertexLit>& vertices ); 
void MeshCalculateNormals( std::vector<VertexLit>& vertices ); 
void MeshInvertWindingOrder( std::vector<VertexLit>& vertices ); 
void MeshInvertIndexWindingOrder( std::vector<uint>& indices ); 
void MeshGenerateTangents( std::vector<VertexLit>& vertices ); 

// This is "Clean" for this assignment
// Can assume vertices is a normal vertex array and indices starts empty
// by the end, vertices should only contain unique vertices, and indices
// should an index buffer that preserves the original shape
void MeshRemoveDuplicate( std::vector<VertexLit>& vertices, std::vector<uint>& indices ); 
```


## Import Options
During import, some massaging of data will be required

```cpp
struct mesh_import_options_t
{
   mat44 transform            = mat44::IDENTITY; // what space transform to apply to the object
   
   bool invert_v              = false;    // Required - inverts V coordinate (v = 1.0f - v)
   bool invert_winding_order  = false;    // Required - Change the winding order of all faces
   bool generate_normals      = false;    // Required - Generate normals for the surface if they weren't in the file
   bool generate_tangents     = false;    // Required - Generate tangents for the surface if they werent' in the file
   bool clean                 = false;    // Extra    - Convert a vertex array to an index vertex array by removing duplicates
};
```

### Cleaning the Mesh
Convert to an indexed mesh. 

So for example, after reading an OBJ for a quad, you would end up with 
6 vertices, with two of the corners being duplicates.

After cleaning, you would have 4 vertices, and 6 indices.  

If you do this extra, print out the memory size needed before and after 
clean (and the percent memory saved/lost from the process).  