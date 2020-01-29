C29.SD2.A02
======

## Goal
Be able to render a triangle to the screen, fulling filling out the rendering pipelines.  That is, the below code should work; 

```cpp

void Game::Startup()
{
   m_camera = new Camera();
   m_camera->SetColorTarget(nullptr); // we use this
}


void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext();

   ctx->BeginCamera( m_camera ); 
   ctx->ClearColorTargets( rgba::BLACK ); 

   ctx->BindShader( "defualt_unlit.hlsl" ); 

   // this quad, if ortho works, should be bottom left corner, 
   // be as tall as half the screen.  
   std::vector<VertexPCU> someQuad;
   AddQuadToVertexArray( someQuad, aabb::FromPoints( vec2(-.25f), vec2(.25f) ) ); 
   ctx->DrawVertexArray( someQuad ); 

   ctx->EndCamera(); 
}
```

------

## Task List

- [ ] When creating a texture from a handle (`ID3D11Texture2D*`), use `ID3D11Texture2D::GetDesc` to cache off the texure dimensions.
- [ ] `Camera::SetColorTarget( Texture* tex )` implemented. 
    - [ ] If color target is `nullptr` when `BeginCamera` is called, default to the swapchain's backbuffer.
- [ ] Be able to create a `Shader` by compiling *Vertex* and *Pixel* shader stages.
    - [ ] Can compile a shader stage
    - [ ] Properly print out compilation errors.
- [ ] Implement `RenderContext::BindShader( Shader* shader )` to set the active shader.  
    - [ ] Assert that you are currently drawing (between a `BeginCamera` and `EndCamera` call)
    - [ ] Binds the vertex and pixel shader stages to the correct place in the pipeline.
- [ ] Implement `RenderContext::GetOrCreateShader( char const* filename )`
- [ ] Implement `RenderContext::BindShader( char const* filename )` helper method so we can set by string.
- [ ] Implement `RenderContext::Draw( int vertexCount, int startIndex = 0 )` to RenderContext
- [ ] Implement `RenderBuffer` class for allocating GPU memory
- [ ] Implement `RenderContext::BindVertexBuffer( VertexBuffer* vbo )`
    - [ ] Keep track of currently bound vertex buffer and don't rebind if no change is made. 
- [ ] Implement `RenderContext::DrawVertexArray`
    - [ ] `RenderContext` should own a vertex buffer (`RenderBuffer* m_immediateVBO`) to use for `DrawVertexArray`
    - [ ] Copy vertices to immediate vbo.
    - [ ] Bind the vertex buffer to input slot 0 (i)
    - [ ] `GetOrCreate` an input layout from current shader/vertex buffer.  Bind if different.
    - [ ] Call `Draw`


------

## Extras
- [ ] *X02.00 : 04pts*:  **Requires X01.00 -or- X01.01** - Allow user to change resolution at runtime and properly recreate the swapchains.
- [ ] *X02.01 : 05pts*:  Hook into DirectX output feed and forward it through your own `DebuggerPrintf` or `ConsolePrintf`

------

## Notes
- [ ] [Sample RenderContext.hpp](./RenderContext.hpp)
- [ ] [Sample Cammera.hpp](./Camera.hpp)
- [ ] [Sample Shader.hpp](./Shader.hpp)
- [ ] [No Input Shader](./triangle.hlsl)
- [ ] [Sample RenderBuffer.hpp](./RenderBuffer.hpp)
- [ ] [Sampler VertexBuffer.hpp](./VertexBuffer.hpp)
- [ ] [Shader Compilation](./shadercompile.md)
- [ ] [Render Buffer Creation & Updating](./renderbuffer.md)
- [ ] [Creating an Input Layout](./inputlayout.md)
- [ ] [Default Shader v0.1](./default.hlsl)

------

## Extra Notes