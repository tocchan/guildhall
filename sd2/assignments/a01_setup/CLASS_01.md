Notes: 2019-01-28
======

## Goal
The below is where we want to eventually get to from the Game's point of view.  
We'll be implemetning these final steps; 

```cpp
//------------------------------------------------------------------------
// In App.cpp
//------------------------------------------------------------------------
void App::RenderFrame()
{
   RenderContext *ctx = App::GetRenderContext(); 

   ctx->BeginFrame();   // Get the next backbuffer to render to; 

   m_game->Render();    // Game Rendering

   ctx->EndFrame();     // Present/SwapBuffer - gets out image to the window; 
}

//------------------------------------------------------------------------
// In Game.cpp
//------------------------------------------------------------------------
void Game::Startup()
{
   // a shader can be though of as a configuration of the GPU.
   // It is the *how* of drawing.  
   RenderContext *ctx = App::GetRenderContext(); 
   m_shader = ctx->GetOrCreateShader( "path/to/shader/a01.hlsl" ); 
}

void Game::Render()
{
   // First, what are we rendering to?
   //   (we'll treat nullptr as being default backbuffer)
   m_camera->SetColorTarget( nullptr );

   // Next, how do we want to view the space
   //   - This would likely be done in startup, but does not hurt to do 
   //   every frame.
   //   - This means treat the output as 40 units high.  width is determined by
   //   our color target's aspect ratio.  Depth range defaults to -1 to 1. 
   m_camera->SetOrthographicProjection( 40.0f ); 

   // Second, start rendering
   RenderContext *ctx = App::GetRenderContext(); 
   ctx->BeginCamera( m_camera ); 

   // clear the screen.  Squirrel will sometimes setup classes to clear 
   // in BeginFrame, I prefer it be a concious choice by the user, so as 
   // to know it is a step (in the case you have other output targets)
   // or in case of effects that want to use the last frames color; 
   ctx->ClearColorTargets( rgba::BLACK ); 

   // Third, HOW do we render this?  Bind the shader
   // TODO: Eventually setting nullptr will revert to a default SD1 style shader
   ctx->BindShader( m_shader ); 

   // Missing No.
   // The "what" are we drawing usually
   // needs to be set, but we'll get to that in a later assignment
   // ctx->BindVertexBuffer( m_vbo ); 

   // Fourth, tell the GPU to draw something
   // In this case we're saying draw 3 vertices, starting
   // at byte offset 0
   ctx->Draw( 3, 0 );


   // Finally, end this camera; 
   ctx->EndCamera(); 
}
```


## Classes
Believe you will want to put the following new classes `Engine/Renderer/...`

```cpp
//------------------------------------------------------------------------
// RenderResource.hpp/cpp
//------------------------------------------------------------------------
class RenderResource
{
   public:
      virtual ~RenderResource() {}

   public:
      RenderContext *m_owner; 
};
```


## Pipeline Stages
A GPU graphics pipeline (what we use to draw) is made up of multiple configurable stages;  See the below documentation from
MSDN to see them; 

- [D3D11 Pipline Stages](https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-graphics-pipeline)

Every rectangular stage is a *programmable stage*, something we can write code to control how it behaves.  Every stage with 
rounded corners is a *configurable stage*, meaning we have limted control, usually in the form of a state object for that.

I will refer to the set of programmable stages as the **Shader Program** or **Program** for short.  We will use the term **Shader** to refer to
the *Shader Program*, plus an additional subset of configurate states, which we'll go into more detail later in these notes; 

The stages required, and that we'll be working with in this class are listed below;  (C) for configurable, (P) for programmable. 

- **Input-Assembler** (C):  This stage prepares input data (such as vertcies and indices) for the *Vertex Shader* stage.  It is the translation between what the user is passing to the program and what the program is expecting.  
- **Vertex Shader** (P):  Is given one vertex, and must output one modified vertex.  
- **Rasterizer Stage** (C):  Given a set of vertices, will determine which pixels on the screen they affect, and invoke a *Pixel/Fragment Shader* for it.  
- **Pixel Shader** (P):  Given a single set of input for a pixel, determine the output color(s).
- **Output-Merger Stage** (C):  Given the output color(s) from the previous stage, how do we combine them the output texture.  

