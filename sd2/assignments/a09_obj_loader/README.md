A09: Wavefront Obj Loader
======

## Overview
Support loading object files from data, as well as model files specifying import rules; 

## Rubric

### [90%] Required
- [ ] 30%: Object Loader Works and one is visible in game; 
- [ ] 10%: `.mesh` files to define model loading rules (see examples)
- [ ] 05%: `RenderContext::GetOrCreateMesh` or equivalent method for mesh resource database; 
- [ ] 10%: Model Class (houses a `GPUMesh`, `Material`, and *transform* for model (Matrix4x4 is fine)).  Constructable from a Mesh; 
- [ ] 35%: Model File support 
    - [ ] 10%: Custom Transform
    - [ ] 10%: Custom Scale
    - [ ] 10%: Flip Faces
    - [ ] 05%: Desired material file to use; 

When grading, I will ask you to import a couple different meshes wiht different import rules.  Be able to
- Show the building in game with a (1 / 256) scale, with the transform in the example data file
- Will change scale, import scale, and flip faces options to see the desired result

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console

## Resources
- [./data/](./data): Data folder containing a couple test files
- [Wikipedia Wavefront OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file): Information about the file format; 

### Model Object
The desired use case for the above system is to be able to load meshes, but will be used when rendering, in what we'll call a Model;

So game code would look like this;

```cpp
class Prop::Load()
{
    // Model *m_model; 
    m_model = new Model( App::GetRenderContext(), "model/building/rock.mesh" ); 
}

void Prop::Render()
{
    m_model->SetModelMatrix( m_transform.GetMatrix() ); 

    // This will bind the model matrix, the material, and draw the mesh - mostly as a convenience class; 
    App::GetRenderContext()->DrawModel( m_model ); 
}
```

...where a model being created does the following;

```cpp
Model::Model( RenderContext *ctx, char const *mesh_file )
{
    m_mesh = ctx->GetOrCreateMesh( mesh ); 
    m_material = ctx->GetOrCreateMaterial( m_mesh->GetDefaultMaterialID() ); 
    m_modelMatrix = mat44::IDENTITY; 
}
```

Notice that meshes are just resources - shared potentially by many models (or even directly access by someone, like a map or level). 



## Extras

### Engine Features
- [ ] X09.00 (05%): `bool EvaluateExpression( float *result, char const *str )` implemented
- [ ] X09.01 (04%): `InputManager` - see below for expected features
- [ ] X09.02 (02%): `StopWatch` class - see notes from MP2

### Rendering Extensions
- [ ] X09.10 (04%): Multi-Material Meshes
- [ ] X09.11 (04%): `CPUMesh` simplify
- [ ] X09.12 (02%): mikkt tangent generation
- [ ] X09.13 (02%): Flat normal generation
    - [ ] X09.14 (03%): Smooth weighted normal generation

## Extra Information

### Evaluate Expression 
A useful tool when parsing data files or user input.  

Be able to evaluate simple value only equations, such as...

```cpp
2 + 3                     // 5
2 + 7 * 3                 // 23
(2 + 7) * 3               // 27
2^8 - 1                   // 255
-1 - (3 * (8 + 2)^2) - 4  // -305
```

If an expression can't be evaluated - leave `out` unchanged and return `false`.


### InputManager
Support a stateful input manager for keyboard and mouse input; 

Support at least the following methods (though you do not need to follow my naming conventions).  

```cpp
enum eControlKeyBit : uint { CONTROL_KEY_CONTROL, CONTROL_KEY_SHIFT, CONTROL_KEY_ALT }; 
typedef uint eControlKeys; 

class InputManager
{
  public:
    // keyboard
    bool KeyIsDown( uint key ) const; 
    bool KeyIsUp( uint key ) const;
    bool KeyWasPressed( uint key ) const;   // was pressed this frame
    bool KeyWasReleased( uint key ) const;  // was released this frame; 

    eControlKeys GetControLKeys() const; 

    // mouse
    float GetMouseScroll() const; 
    bool MouseButtonIsUp( uint btn ) const; 
    bool MouseButtonIsDown( uint btn ) const; 
    bool MouseButtonWasPressed( uint btn ) const; 
    bool MouseButtonWasReleased( uint btn ) const; 

  public: 
    // whatever members or mtehods you need to accompolish the above; 
};
```

### StopWatch 
--notes from mp2 - will post in the morning--


### Multi-Material Meshes
*Note: The MikuSnow model in the data folder is a good use case as it contains multiple materials using seperate textures - the opaque model, the transparent base, and the transparent detail objects*

If a mesh file has multiple materials - you can think of this as being multiple meshes (or `Submesh`es) making up the overall model.  

Two suggested ways to go about this; 

1. Create new classes; `GPUMeshGroup` and `CPUMeshGroup`, which are made up of multiple child meshes, 
2. Update `CPUMesh` and `GPUMesh` to contain `CPUSubmesh` and `GPUSubmesh` children.  You old code would then be a mesh with a single submesh, but your interfaces mostly stay the same. 

When drawing, each mesh can be assigned a material to use.  If you have fewer materials defined then meshes available, the last assigned material applies to the remaining submeshes (so if you have a single material, all submeshes are drawn using that).  

I would recommend adding a `m_materialID` to your submesh or mesh objects.  When creating a model, keep an array of materials and populate it using the meshes suggested material. 


## CPUMesh Simplify
OBJ files will load in as trianglelists - which is incredibly inefficient in terms of space.  So after import, simplify the mesh by creating a vertex array that only contains unique vertices, and an index buffer maintaining the original topology of the mesh. 

Print a before/after vertex count for the mesh to show that is working to your dev console. 


### MikkT Tangent Space Calculation
By default, OBJ files (and some FBX files) will not export tangents/bitangents.  But as long as you have normals and UVs, you can calulate tangents/bitangents to use (with some napkin math, you can come up with these equations remembering you need to make tangents move along U and bitangents parallel to V). 

There are quite a few edge cases, and the de-facto standard for doing this is the MikkT algorithm (so defacto that some exporters save space on export assuming the importer will use this algorithm).  

See the included C headers, [./data/src/mikkt/](./data/src/mikkt/) which are provided by the original author of the algorithm.  Use the header to figure out how to feed your `CPUMesh` to their header to calculate tangents; 


### Normal Generation
*Note: The MikuSnow model in the data folder is a good use case for this as it does not have normals in the OBJ file*

If a model comes in without normals, you can calculate normals for the faces given just the face positions. 

For flat, recommend doing this calculation before simplifying, as each face should be calculated seperately and will usually result in more unique vertices.

For smooth normals, a technique similar to mesh simplification should be used to find shared faces for a specfic vertex.  The normal for that vertex is then the average of all the shared faces, weighted by the face area. 