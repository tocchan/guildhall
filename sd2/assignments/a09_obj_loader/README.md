A09: Wavefront Obj Loader
======

## Overview
Support loading object files from data, as well as model files specifying import rules; 

## Rubric

### [90%] Required
- [ ] 40%: Object Loader Works and one is visible in game; 
- [ ] 10%: Model Class (houses a `GPUMesh`, `Material`, and *transform* for model (Matrix4x4 is fine))
- [ ] 40%: Model File support 
    - [ ] 15%: Custom Transform
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


## Extras

### Engine Features
- [ ] X09.00: `bool EvaluateExpression( float *result, char const *str )` implemented
- [ ] X09.01: `InputManager` - see below for expected features
- [ ] X09.02: `StopWatch` class - see notes from MP2

### Rendering Extensions
- [ ] X09.10: Multi-Material Meshes
- [ ] X09.11: mikkt tangent generation
- [ ] X09.12: Flat normal generation
    - [ ] X09.13: Smooth weighted normal generation

## Extra Information
Coming soon...
