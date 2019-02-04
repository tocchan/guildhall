SD2-A02:  Orthographic Camera using Uniform Buffers
======

## Goal
Get orthographic camera and vertex buffers back into our Engine.
Finish out by adding TIME data as information our shaders can use; 

## Grading
- (80%) The below code works and I see that quad;  
- (20%) The quad is animated in the shader using time (make it move back and forth, or in a circle)

## New Classes

- `RenderBuffer` - Effectively an allocation of memory on the GPU.  All data accessed in a shader comes from GPU backed buffers.
  - `VertexBuffer` - A `RenderBuffer` meant for stream input into a shader. 
  - `UniformBuffer` - A `RenderBuffer` meant to for uniform input into a shader.

## Changes to Old Classes/Methods

- `ShaderStage`, when compiling for a VertexStage, do not discard the byte code blob, as it is needed to create an InputLayout
- `RenderContext`
  - `RenderContext::BindShader` variant that takes a shader name instead, and uses the shader in the context's database


## Notes 

By the end of this, the following game code should fully work; 

```cpp

void Game::Startup()
{
   m_camera = new Camera();
   m_camera->SetColorTarget(nullptr); // when binding, if nullptr, use the backbuffer

   // set projection; 
   float aspect = WINDOW_WIDTH / WINDOW_HEIGHT; 
   m_camera->SetOrthographicProjection( vec2(0.0f), vec2(aspect * 100.0f, 100.0f) ); 
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
   AddQuadToVertexArray( someQuad, aabb::FromPoints( vec2(0.0f), vec2(25.0f, 50.0f) ) ); 
   ctx->DrawVertexArray( someQuad ); 

   ctx->EndCamera(); 
}
```

The `TIME` constant buffer should look like the following; 

```hlsl 
cbuffer struct 
```

### Tips

There are quite a few new pieces being added - I suggest working top-down (write the above sample code in your Engine's style, and then implement each function, implementing the pieces those functions need as you need them). 

I suggest implementing in this order; 

1. UniformBuffer - [Use This Shader](./shader/ubo.hlsl) This will give back the ability to use your ortho matrix.  
   - [Camera.hpp](./Camera.hpp)
   - [RenderBuffer.hpp](./RenderBuffer.hpp)
   - [UniformBuffer.hpp](./UniformBuffer.hpp)
2. VertexBuffer - [Use This Shader](./shader/vbo.hlsl) Reintroduces DrawVertexArrrays
   - [VertexBuffer.hpp](./VertexBuffer.hpp)
   - [Shader.hpp](./Shader.hpp)
