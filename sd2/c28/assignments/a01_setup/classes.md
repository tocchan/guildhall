
## What Is Present


- **RenderContext**: 

- **Camera**: 

- **Texture2D**: Previously called `Texture`.


## New
- **RenderOutput**: A display we are rendering to (usually user visiable, like a Window or screen).  Will control VSync options as well.  Primary job is to house the `SwapChain` of backbuffers.
- **TextureView**: An optional base class for the following (there may be similarities, )
  - **TextureView2D**: A view of a texture useable in a texture, contains a sampler; 
  - **ColorTarget**:  A renderable view of a Texture 
  - **DepthStencilTarget**:  A renderable view of a Texture mant for Depth/Stencil info
- **Sampler**: Rules for selecting a color from a texture view based on UV
- **Shader**: Rules for how something is rendered (the program/pipeline/etc...).  Tends to be fairly static (created and not changed).  
  - **ShaderProgram**: Potential breakout of programmable part of the shader
  - **ShaderStage**: A single programmable stage of a ShaderProgram - implementation specific.
  - Potential sub-structs to help organize;
    - `raster_state_t`: Controls rasterization phase of the pipeline.  Culling, winding order, and fill mode are the most common options; 
    - `depth_stencil_state_t`: Controls how we use the depth/stencil buffers.  Depth compare and operation are the options we'll use this for; 
    - `blend_state_t`: Controls how the output color is blended with the current color target(s).  We will only be using it just for color blending. 
- **Material**: Shader with data.  Material is to a shader what an object is to a class;  Also similar to the Sprite/SpriteDefinition paradigm;  

## Mid-Tier Goal 
This is a simple app to just render a single 

```cpp

// Game.cpp
void Game::Render()
{
   // what are we rendering to/
   RenderOutput *output = App::GetRenderOutput(); 
   ColorTarget *backbuffer = output->get_current_backbuffer(); 

   // Get the context we will use to render (combination of RenderDevice and RenderContext)
   RenderContext *rc = App::GetRenderContext();  // g_theRenderContext?

   // camera setup - probably done during setup, but as long as it is done BEFORE we start drawing using it; 
   // I'm rendering to the window's backbuffer, treating the screen as (0,0) bottom left to (1280,720) top right
   // and depth range of -10.0 to 10.0
   m_camera->set_color_target( 0, backbuffer ); // some people will treat "nullptr" as a backbuffer, that will come when we have a scene-graph. 
   m_camera->set_orthographic_projection( 0.0f, 0.0f, 1280.0f, 720.0f, -10.0f, 10.0f ); 

   // Draw!

   // 1. What are we drawing to?  The camera; 
   rc->begin_camera( m_camera ); 

   // 2. This is the first thing drawn this frame, so clearing
   rc->clear_color( rgba::BLUE ); // previously known as "clear_screen", renamed since we are actually clearing the currently bound camera; 


   // 3. What are we drawing?  Some quad; 
   std::vector<VertexPCU> vertices; 
   AddVertsForAABB2( vertices, AABB2::ThatContains( vec2(10.0f, 20.0f), vec2(300.0f, 50.0f) ), rgba::GREEN )

   // 4. (new) How are we drawing it?
   rc->set_shader( nullptr );          // nullptr will mean the default behaviour, which will eventually be an unlit textured alpha-blended shader

   // 5. Actually draw it; 
   rc->draw_vertex_array( vertices ); 

   // we're done drawing, so stop
   rc->end_camera(); 
}

// App.cpp
void App::EndFrame()
{
   // get our output, and present
   RenderOutput *output = App::GetRenderOutput(); 
   output->present();   // another good name is "swap_buffers()".
}
```