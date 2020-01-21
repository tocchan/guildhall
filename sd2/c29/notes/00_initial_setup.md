## Code Considerations
My code looks very different from your engines.  In class notes I will try to use my best guess at what you named them in Semester 1.  So if something is unclear, let me know.  You may not have it, or you may have it by a different name. 

My coding style is also different, and I may fall into it by habit writing samples.  Just make sure that if you're transcribing to your code base, you match the style you want for *your* engine. 

I will not dictate much outside of code interface in this class.  As long as your application can do what I ask, and is readable, it is fine.  


## D3D11

- Physical Device
- Device
- Device Context
- SwapChain
- Textures
  - Render Targets (ie: Color Target)

- Host -vs- Client
  - *Host* - The GPU, who we are making requests to
  - *Client* - Out application


## Overview

- Refactoring Macros, `UNIMPLEMENTED` and `TODO` *optional*
- Bind up `Window` code into a new `Window` class
- Remove all `OpenGL` initialization code
- Go strip out all OpenGL functionals from `RenderContext`, replace with `UNIMPLEMENTED`.  This will tell us what we have left to fix, or break if we hit an unimplemented code path; 


- `Device`
- `DeviceContext`
- `Texture`
- D3D11 Reference Counting
- `TextureView`


## Goal 

```cpp
void App

void App::BeginFrame()
{
    m_window->BeginFrame(); 
    g_theRenderer->BeginFrame(); 
}

void App::Render()
{
    // optional game/render
    float r = RangeMap( SinDegrees(seconds), -1.0f, 1.0f, 0.0f, 1.0f ); 
    RGBA clearColor = RGBA( r, 0, 0, 1 ); 
    m_camera->SetClearMode( CLEAR_COLOR_BIT, clearColor ); 

}

void App::EndFrame()
{
    g_theRenderer->EndFrame();
    g_theRenderer->Present(); 
}

## `UNIMPLEMENTED` macro
