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

   // Shader *m_shader; in Game.hpp
   m_shader = ctx->GetOrCreateShader( "path/to/shader/a01.hlsl" ); 
}

void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext(); 
   ColorTargetView *rtv = ctx->GetFrameColorTarget(); 

   // First, what are we rendering to?
   // TODO: nullptr is a valid color target for index 0, will default
   //       to the default backbuffer in BeginCamera
   m_camera->SetColorTarget( rtv );

   // missing no. 1:  how do we want to view the space
   //   - This would likely be done in startup, but does not hurt to do 
   //   every frame.
   //   - This means treat the output as 40 units high.  width is determined by
   //   our color target's aspect ratio.  Depth range defaults to -1 to 1. 
   // m_camera->SetOrthographicProjection( 40.0f ); 

   // Second, start rendering
   ctx->BeginCamera( m_camera ); 

   // clear the screen.  Squirrel will sometimes setup classes to clear 
   // in BeginFrame, I prefer it be a concious choice by the user, so as 
   // to know it is a step (in the case you have other output targets)
   // or in case of effects that want to use the last frames color; 
   ctx->ClearColorTargets( rgba::BLACK ); 

   // Third, HOW do we render this?  Bind the shader
   // TODO: Eventually setting nullptr will revert to a default SD1 style shader
   ctx->BindShader( m_shader ); 

   // Missing No. 2: The "what" are we drawing usually
   // needs to be set, but we'll get to that in a later assignment
   // ctx->BindVertexBuffer( m_vbo ); 

   // Fourth, tell the GPU to draw something
   // In this case we're saying draw 3 vertices, starting
   // at byte offset 0
   ctx->Draw( 3, 0 );


   // Finally, end this camera; 
   ctx->EndCamera(); 
}