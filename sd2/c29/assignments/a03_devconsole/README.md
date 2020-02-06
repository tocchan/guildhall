C29.SD2.A03 : Dev Console
======

## Goal
This assignment gets our Renderer back to an SD1 feature list;
- Textures & Samplers
- Orthographic Matrix

Once that is in, we will finish up the dev console, supporting the ability to type in custom commands.

### Code Sample

First, we want rendering to work - drawing should work similar to SD1 now;  

```cpp
void Game::Startup()
{
   // ...
   m_camera->SetColorTarget( nullptr ); // will use swapchain backbuffer for size; 
   m_camera->SetProjectionOrthographic( 10.0f, -1.0f, 1.0f ); 
   m_camera->SetPosition( vec3(0.0f) ); 
}

void Game::Update()
{
   if (!g_theDevConsole->IsOpen()) {
      UpdateInput(); // nothing for now
   }

   // normal update; 
}

void Game::Render()
{
   m_renderer->BeginCamera( m_camera ); // should BindShader(nullptr) by default

   // draw the first quad a little to the right, use an invert color shader to show it working
   m_renderer->BindTexture( "Data/Image/SomeTexture.png" ); 
   m_renderer->BindShader( m_invertShader ); 
   m_renderer->DrawQuad( vec2(4.0f, 0.0f), 6.0f ); 

   // darw it normal on the left
   m_renderer->BindShader( nullptr ); // go back to default
   m_renderer->DrawQuad( vec2(-4.0f, 0.0f), 5.0f ); 

   m_renderer->EndCamera(); 
}
```

### Dev Console
The dev console should use a secondary camera that **does not clear** as it will be rendering
second.  

```cpp
void App::Render()
{
   g_theGame->Render(); // render game (will clear color)
   g_theDevConsole->Render(); // render console, draws over top of game, using its own game
}

void DevConsole::Render()
{
   if (IsClosed()) {
      return; 
   }

   m_renderer->BeginCamera( m_devConsoleCamera ); 
   
   // TODO: draw dev console...
   // ...
   
   m_renderer->EndCamera(); 
}
```


------

## Core Task List [85%]

This is the first assignment where you must supplement your assignment with some extras to get full credit.  Core tasks
only cover 85 of the total 100 points.  Make a note of all extras you attempt in your assignment, 
and remember you can submit extras from previous assignments.

- [ ] *15pts*: Get a Working Orthographic Camera
    - [ ] *05pts*: Implement `MakeOrthographicProjectionMatrixD3D` in `Engine/Math/MatrixUtils.hpp`, `.cpp`.  
    - [ ] *05pts*: Camera now owns a `UniformBuffer` for their matrices
    - [ ] *05pts*: Camera updates and binds their uniform buffer in `BeginCamera`
- [ ] *18pts*: Get Working Textures
    - [ ] *04pts*: Be able to create a `Texture` from an Image/File
    - [ ] *04pts*: `TextureView` supports shader resource views (srv)
    - [ ] *04pts*: Make a `Sampler` class
    - [ ] *03pts*: Default Samplers (Linear+Wrap, Point+Wrap)
    - [ ] *03pts*: `RenderContext::GetOrCreateTexture` works again.
- [ ] *08pts*: Blend States supported by shader.
    - [ ] *04pts*: Blend state is created/recreated and bound when shader is bound.
    - [ ] *04pts*: Only recreate blend state if changed since last binding
- [ ] *05pts*: Default Built-In Shader to use when `BindShader(nullptr)` is used
    - [ ] `Renderer::SetBlendState` should affect this shader; 
- [ ] *05pts*: Render two textured quads on screen
    - [ ] *04pts*: One using an invert color shader
    - [ ] *01pts*: One default
- [ ] *34pts*: Dev Console
    - [ ] *02pts*: User can open the dev console using the **tilde** key '\~'.  
                   You may use a custom key if you make a note in your assignment readme.
        - [ ] Upon opening console, be sure the input is cleared.
    - [ ] *02pts*: Pressing tilde again closes the console.
    - [ ] *09pts* If console is open, Escape should...
        - [ ] *03pts*: Clear input if there is any text currently there
        - [ ] *03pts*: Close the console if console is open.
        - [ ] *03pts*: Normal game behaviour otherwise (for example, for now it will quit your application)
    - [ ] *14pts*: User can type commands **only** while console is open.
        - [ ] *02pts*: Typing should insert characters at the carot
        - [ ] *02pts*: The carot blinks (every quarter to half second usually looks okay)
        - [ ] *02pts*; Left & Right arrow keys should move carot forward and back along string 
        - [ ] *02pts*: Delete should delete the character after the carot
        - [ ] *02pts*: Backspace should delete the character before the carot
        - [ ] *02pts*: Pressing `Enter` submits the command
        - [ ] *02pts* If it is an invalid command (it has not handler), print an error message
    - [ ] *02pts* Game input is ignored while dev console is open.
    - [ ] *03pts*: Support a `quit` command that quits the game.
    - [ ] *03pts*: Support a `help` command that lists all exposed commands
   
------

## Extras

- [ ] *X03.01 : 03pts*: Built-in error shader to use when a shader fails to compile.   
- [ ] *X03.02 : 04pts*: AutoComplete
- [ ] *X03.03 : 04pts*: Context Sensitive Auto Complete
- [ ] *X03.04 : 04pts*: Command History
- [ ] *X03.05 : 03pts*: Persistant Command History (requires X03.02)
- [ ] *X03.06 : 05pts*: Text Selection 
- [ ] *X03.10 : 04pts*: Cut, Copy & Paste from Window's Clipboard (Requires X03.04)
- [ ] *X03.11 : 02pts*: Text Scrolling
- [ ] *X03.12 : 02pts*: Vertical Scroll Bar (Requires X03.11)
- [ ] *X03.13 : 06pts*: Contextual Auto Complete (Requies X03.01)
- [ ] *X03.20 : 04pts*: Text Color Markup
- [ ] *X03.21 : 02pts*: Command Scripts
- [ ] *X03.22 : 02pts*: Mouse Input on Dev Console (scroll wheel to scroll, click to set carrot)
- [ ] *X03.23 : 04pts*: Mouse Selection (Requires X03.05 and X03.22)
- [ ] *X03.24 : 02pts*: Navigation Hotkeys 
- [ ] *X03.25 : 02pts*: Audio Cues
- [ ] *X03.26 : 02pts*: Open & Close Animations (Slides Open for Example)
- [ ] *X03.27 : 04pts*: Undo/Redo History (Ctrl+Z, Ctrl+Y)

------



## Documents

### New Headers
- [ ] [RenderContext.hpp](./RenderContext.hpp)
- [ ] [Camera.hpp](./Camera.hpp)
- [ ] [MatrixUtils.hpp](./MatrixUtils.hpp)
- [ ] [Texture.hpp](./Texture.hpp)
- [ ] [TextureView.hpp](./TextureView.hpp)
- [ ] [Sampler.hpp](./Sampler.hpp)

### Class Notes
- [ ] [New Default Shader](./default.hlsl)
- [ ] [Constant Buffers](./constantbuffer.md)
- [ ] [Orthographic Projection](./ortho.md)
- [ ] [Textures & Samplers](./textures.md)

------

## Extra Notes