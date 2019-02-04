Setting up Uniform Buffers
======

1. Create desired top level code
2. Stub out methods and classes you see.
3. Start with getting Orthographic Projection Working
   1. Switch to [./shader/ubo.hlsl](./shader/ubo.hlsl), which uses a UniformBuffer
   2. Update method on Camera to set ortho to save off the information
   3. Have RenderContext::BeginCamera now tell the Camera to update its UBO...
   4. ...and Bind it (stub needed methods/members as you go)
      - Implement `RenderContext::BindUniformBuffer`, add needed members and methods
   5. Implement `UniformBuffer`
   6. Implment `RenderBuffer` - which is the bulk of the D3D11 work
   7. *Note: I am giving you a lot of this code - take the time to understand it!*
4. Get VertexBuffers working
   1. Switch to [./shader/vbo.hlsl](./shader/vbo.hlsl), which uses a UniformBuffer
   2. Stub out `RenderContext::DrawVertexArrays`, which will stub out `VertexBuffer`
   3. Implement `RenderContext::BindVertexStream`, add members/methods to `VertexBuffer` as needed
   4. Implement `VertexBuffer`
   5. Test
5. Implement the `FRAME CONSTANTS` uniform buffer so your shaders can use time (excercise for reader)


## Uniform Buffers
- **Uniform Buffers** (aka: **Constant Buffers**) are ways to feed information to the shader that is _constant__ for the entire draw call.  

  - Can map up to 64KB of data
  - Up to 15 buffers can be bounds in D3D11
    - We usually end up using about 4 or 5 total for projects at Guildhall.

## Vertex Buffers
- Normal creation
- Stream Slots
- Instanced Rendering
- Vertex Layout
- `buffer_attribute_t` implementation;  (if time)


## Orthographic Projection as a Matrix (if time) 
