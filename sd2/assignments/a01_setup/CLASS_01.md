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

   // Shader *m_shader; in Game.hpp
   m_shader = ctx->GetOrCreateShader( "path/to/shader/a01.hlsl" ); 
}

void Game::Render()
{
   RenderContext *ctx = App::GetRenderContext(); 
   ColorTargetView *rtv = ctx->GetFrameColorTarget(); 

   // First, what are we rendering to?
   //   TODO: Make nullptr default to this frame's backbuffer; 
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
```


## Classes & Methods
The above will help us drive the classes and new methods we'll need, so going very minimalistic lets make some classes that would at least allow the above code to compile and run. 

Believe you will want to put the following new classes `Engine/Renderer/...`

```cpp
//------------------------------------------------------------------------
// Renderer.hpp/cpp
//------------------------------------------------------------------------
class Renderer
{
   public:
      // ...
      // *updated* These now do more work;
      void BeginFrame(); 
      void EndFrame(); 

      void BeginCamera( Camera *cam ); 
      void EndCamera(); 

      void ClearColorTargets( rgba const &color ); 

      // *new* bind a shader to the context; 
      void BindShader( Shader *shader ); 

      // *new* basic draw methods
      // Draws a number of vertices out of the currently bound vertex buffer (todo)
      void Draw( uint vertex_count, uint byte_offset = 0U ); 

   public: // resources *new*
      Shader* GetOrCreateShader( std::string const &filename ); 
};

//------------------------------------------------------------------------
// *new* ColorTargetView.hpp
//------------------------------------------------------------------------
class ColorTargetView
{
   public: // a renderable view of a texture // private
      ID3D11RenderTargetView *m_rtv; 
}; 

//------------------------------------------------------------------------
// Camera.hpp/cpp
//------------------------------------------------------------------------
class Camera 
{
   public:
      void SetColorTarget( ColorTargetView *ctv ); // what are we rendreing to; 

   public:
      ColorTargetView *m_color_target; // What are we rendering to?
}; 

//------------------------------------------------------------------------
// *new* Shader.hpp/cpp
//------------------------------------------------------------------------
// A programmable shader stage, either vertex or pixel fornow; 
class ShaderStage
{
   public:
      ShaderStage()
         : m_handle(nullptr) {}

      ~ShaderStage()
      {
         DX_SAFE_RELEASE(m_handle); 
      } 


   public:
      union {
         ID3D11Resource *m_handle; 
         ID3D11VertexShader *m_vs; 
         ID3D11PixelShader *m_ps; 
      };

      // m_vs = nullptr;
      // m_handle = 0xDEADBEEF; 
}; 

//------------------------------------------------------------------------
class Shader 
{
   public:
      bool create_from_file( std::string const &filename ); 

   public:
      ShaderStage m_vertex_shader; 
      ShaderStage m_pixel_shader; 
}; 

```

## The Color Target View
A **Color Target View**, more commonly referred to as the **Render Target View** in documentation, is a writable view of a texture.

For example, out backbuffer is created as `R8G8B8A8_UNORM` color format, meaning each pixel is four channels, each byte getting a channel that is 
one byte large.  Our clear function, and eventually shader, clear using four floats (R, G, B, A), between 0 and 1.  The view is how the program knows 
how to store four floats into the bytes of the texture.  

In short, any time we want to render **to** a texture, we need to make a renderable view of it.  For now, this is just our backbuffer; 

So, updating the code, we'll generate the view at the start of a frame, and release it at the end; 

```cpp

void Renderer::BeginFrame()
{
   // first, acquire the backbuffer (this adds one reference to the backbuffer)
   ID3D11Texture2D *back_buffer = nullptr;
   m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
  
   // Okay, we have an rtv, store it in our own class
   m_frame_backbuffer_ctv = new ColorTargetView();
   m_frame_backbuffer_ctv->CreateForInternalTexture( back_buffer ); 

   // ctv will hold a reference to the backbuffer, so we
   // can release this functions hold on it; 
   DX_SAFE_RELEASE( back_buffer ); 
}

//------------------------------------------------------------------------
void Renderer::EndFrame() 
{
   m_swapChain->Present( 0, 0 ); 

   // free this up; 
   // This means we should also release our RTV, otherwise we
   // have a leak; 
   delete m_frame_backbuffer_ctv; 
   m_frame_backbuffer_ctv = nullptr; 

   // TODO: We don't need to new and delete this
   // object each frame, just update the m_rtv (being 
   // sure to release the old one)
}

//------------------------------------------------------------------------
// ColorTargetView.cpp
//------------------------------------------------------------------------
ColorTargetView::~ColorTargetView() 
{
   DX_SAFE_RELEASE( m_rtv ); // be sure to release the reference; 
}

// this is an internally used method - not called outside the of the render system; 
// update the internal handle; 
ColorTargetView::CreateForInternalTexture( ID3D11Texture2D *tex )
{
   DX_SAFE_RELEASE( m_rtv ); // release the old one; 

    // next, create the rtv for it; 
   HREUSLT hr = m_device->CreateRenderTargetView( tex,  // texture to create the view for
      nullptr,   // default options (use texture info)
      &m_rtv );    // out variable to store the view

   GUARANTEE_OR_DIE( SUCCEEDED(hr) ); // make sure we could get it 
   // if this fails, check your output - DX will usually tell you why
   // (likely texture wasn't setup to be an output)
}

```

## The Camera
The camera has two major jobs; 

1. Being treated like a camera in movie production.  Something we can move around and setup a shot.  This also gives us information how about
to tell where the camera is and what we can see. 
2. What are we drawing to.  Which texture, and what part of that texture.  

We're going to focus more on number 2 first.

So now that we have the concept of a `ColorTargetView`, we can now set it as the camera's output; 

```cpp
// Camera.cpp
void Camera::SetColorTarget( ColorTargetView *ctv )
{
   m_color_target = ctv; 
}
```

That is all there is to it.  The actual work happens now in `RenderContext::BeginCamera`

```cpp
void RenderContext::BeginCamera( Camera *cam )
{
   // this was probably empty before, but now it has something to do!

   // first, figure out what we're rendering to; 
   ColorTargetView *view = cam->m_color_target; 

   // Bind this as our output (this method takes an array, so 
   // this is binding an array of one)
   m_context->OMSetRenderTargets( 1, &(view->m_rtv), nullptr );

   // Next, we have to describe WHAT part of the texture we're rendering to (called the viewport)
   // This is also usually managed by the camera, but for now, we will just render to the whole texture
   D3D11_VIEWPORT viewport;  
   memset( &viewport, 0, sizeof(viewport) );
   viewport.TopLeftX = 0U;
   viewport.TopLeftY = 0U;
   viewport.Width = view->GetWidth();
   viewport.Height = view->GetHeight();
   viewport.MinDepth = 0.0f;        // must be between 0 and 1 (defualt is 0);
   viewport.MaxDepth = 1.0f;        // must be between 0 and 1 (default is 1)
   m_context->RSSetViewports( 1, &viewport );
}
```

So, what don't we have?  The width and height!  So, this leads up back to ColorTargetView to fetch this information; 

```cpp
void ColorTargetView::CreateForInternalTexture( ID3D11Texture2D *tex ) 
{
   // normal code from above; 
   // ...

   // cache off size from the texture description 
   D3D11_TEXTURE2D_DESC desc;
   tex->GetDesc( &desc )

   m_size = vec2( desc.Width, desc.Height ); 
}

// meaning the class needs that member
class ColorTargetView
{
   public:
      // ...

      inline float GetWidth() const    { return m_size.x; }
      inline float GetHeight() const   { return m_size.y; }

   public:
      // ...

      vec2 m_size = vec2::ZERO; 
};
```

### Clearing Screen
`ClearScreen` should be changed to `ClearColorTargets`, and should clear the currently bound color targets (or camera); 

```cpp
void RenderContext::ClearColorTargets( rgba const &color ) 
{
   // so this needs a camera reference to the currently bound rtv, 
   // which means your context should hold onto the currently bound camera; 

   ID3D11RenderTargetView *rtv = m_current_camera->m_color_target; 

   float clear_color[4] = { color.r, color.g, color.b, color.a };
   gD3DContext->ClearRenderTargetView( rtv, clear_color );
}
```

**WARNING** Be careful here as the clear does not take into account the scissor or viewport;  IF you want something better - draw a full screen quad that just outputs a color. 

Now, since we're caching off the current camera - `EndCamera` should just remember to clear it; 


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

## Shader

```cpp

// RenderContext
Shader* RenderContext::GetOrCreateShader( std::string const &filename ) 
{
   // similar to texture UNLESS i need make it; 

   // ... oh, gotta make it;
   { 
      Shader *shader = new Shader(); 
      shader->CreateFromFile( filename ); 

      if (shader->IsValid()) {
         m_shaderMap.set( filename, shader ); 
      }
   }
}

// Shader.cpp
bool Shader::CreateFromFile( std::string const &filename ) 
{
   void *buffer = CreateBufferForFile( filename ); 

   m_vertexShader.LoadShaderFromSource( filename, buffer, SHADER_STAGE_VERTEX );
   m_pixelShader.LoadShaderFromSource( filename, buffer, SHADER_STAGE_FRAGMENT ); 
}

enum eShaderStage
{
   SHADER_STAGE_VERTEX, 
   SHADER_STAGE_FRAGMENT, 
};

static char const* GetEntryForStage( eShaderstage stage ) 
{
   switch (stage) {
      case SHADER_STAGE_VERTEX: return "VertexFunction"; 
      case SHADER_STAGE_FRAGMENT: return "FragmentFunction";
      default: GUARANTEE_OR_DIE(false, "Unknown shader stage"); 
   }
}


static char const* GetShaderModelForStage( eShaderstage stage ) 
{
   switch (stage) {
      case SHADER_STAGE_VERTEX: return "vs_5_0"; 
      case SHADER_STAGE_FRAGMENT: return "ps_5_0";
      default: GUARANTEE_OR_DIE(false, "Unknown shader stage"); 
   }
}

class ShaderStage
{
   public:
      bool LoadShaderFromSource( RenderContext *ctx, std::string const &filename, void const *source, eShaderStage stage )
      {
         m_stage = stage; 
         ID3DDevice *device = ctx->m_device; 

         ID3DBlob *bytecode = HLSLCompileToByteCode( filename, source, GetEntryForStage(stage), GetShaderModelForStage(stage) ); 
         if (bytecode == nullptr) {
            return false; 
         }

         switch (stage) {
            case SHADER_STAGE_VERTEX:    // Compile the byte code to the final shader (driver/hardware specific program)
               device->CreateVertexShader( bytecode->GetBufferPointer(), 
                  bytecode->GetBufferSize(), 
                  nullptr, 
                  &m_vs );
               break;

            case SHADER_STAGE_FRAGMENT: 
               device->CreatePixelShader( bytecode->GetBufferPointer(), 
                  bytecode->GetBufferSize(), 
                  nullptr, 
                  &m_fs );
               break; 
            }
         }

         DX_SAFE_RELEASE(byte_code); 
         return IsValid();
      }

      inline bool IsValid() const { return m_handle != nullptr; }

   public:
      eShaderStage m_stage; 
      union {
         ID3D11Resource *m_handle; 
         ID3D11VertexShader *m_vs; 
         ID3D11FragmentShader *m_vs; 
      };
}

```