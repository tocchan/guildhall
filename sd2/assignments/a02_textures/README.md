SD2-A02:  Textured Orthographic Quad
======

## Goal
To get a textured, transformed quad to appear on screen.  This will extend our engine to near feature parity outside of blend state. 

## New Classes

- `RenderBuffer` - Effectively an allocation of memory on the GPU.  All data accessed in a shader comes from GPU backed buffers.
  - `VertexBuffer` - A `RenderBuffer` meant for stream input into a shader. 
  - `UniformBuffer` - A `RenderBuffer` meant to for uniform input into a shader.
- `Texture` - An arbitrary texture.  Mostly placeholder for now;
  - `Texture2D` - Replaces your old `Texture` class.  We will eventually have `Texture1D`, `Texture3D`, and `TextureCube`.  
- `TextureView` - Called a *ShaderResourceView* or *SRV* in documentation.  This a view of a texture that a shader can read from;  
  - `TextureView2D` - Specialization for 2D views of data.  
- `Sampler` - Rules for how, given a *UV*-coordindate, how do we determine the color of a texture; 

## Changes to Old Classes/Methods

- `ShaderStage`, when compiling for a VertexStage, do not discard the byte code blob, as it is needed to create an InputLayout
- `RenderContext`
  - `RenderContext::GetOrCreateTexture` becomes `RenderContext::GetOrCreateTextureView2D`
  - `RenderContext::BindShader` variant that takes a shader name instead, and uses the shader in the context's database
- `Camera`
  - `Camera::SetProjectionFromHeightAndPivot`: A more generic orthographic method; 


## Notes 

By the end of this, the following game code should fully work; 

```cpp

void Game::Startup()
{
   m_camera = new Camera();
   m_camera->SetColorTarget(nullptr); // when binding, if nullptr, use the backbuffer

   // So we don't want to have to worry about our screen resolution
   // we'll instead define the our ortho in terms of bottom to top height
   // and a pivot point to (where is 0.0)
   // so this would put (0,0) at the bottom left, (0,100) at top left.
   // a pivot of (.5,.5) would put 0 at the center...
   m_camera->SetOrthographicProjectionFromHeightAndPivot( 100.0f, vec2(0.0f) ); 
}


void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext();

   ctx->BeginCamera( m_camera ); 
   ctx->ClearColorTargets( rgba::BLACK ); 

   ctx->BindShader( "defualt_unlit.hlsl" ); 
   ctx->BindTextureView( 0, "some/texture.png", SAMPLER_BILINEAR ); 

   // this quad, if ortho works, should be bottom left corner, 
   // be as tall as half the screen.  
   std::vector<VertexPCU> someQuad;
   AddQuadToVertexArray( someQuad, aabb::FromPoints( vec2(0.0f), vec2(25.0f, 50.0f) ) ); 
   ctx->DrawVertexArray( someQuad ); 

   ctx->EndCamera(); 
}
```

