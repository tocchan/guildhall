C29.SD2.A01 - DirectX11 Setup
======



## Goal 

*I recommend creating a new project for this titled `FPS`, and creating a `D3D11` branch to work in so you don't affect your MP2 project or current protogame.  By about week 3 or 4 the `RenderContext` should be back to fully functional again and we'll merge in.*


This is the intended interface we want to end with Game side.  Feel free to rename or make changes to code style to match your own engines.

Our first goal is to move a good chunk of our rendering and windowing systems over to Engine
so our `App` startup looks similar to the following; 

```cpp
//---------------------------------------------------------------------------------
// App.cpp
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void App::Startup()
{
    float windowClientRatioOfHeight = g_theConfig->Get( "window_height_ratio", 0.8f ); 
    float aspectRatio = g_theConfig->Get( "window_aspect", 16.0f / 9.0f ); 
    std::string windowTitle = g_theConfig->Get( "window_title", "SD2.A01" ); 

    // create the window
    m_window = new Window();  
    m_window->Open( window_title, windowClientRatioOfHeight, aspectRatio ); 
    m_window->SetInputSystem( g_InputSystem ); )

    // create the render context
    g_RenderContext = new RenderContext(); 
    g_RenderContext->Startup( m_window );  // sets up the rendering subsystem, creates sane defaults

    //... other setup?
}

//---------------------------------------------------------------------------------
void App::Shutdown()
{
    // other shutdown?
    // ...

    // cleanup the rendering system - this will report any leaks we have in debug
    delete g_RenderContext; 
    g_RenderContext = nullptr; 

    delete m_window; 
    m_window = nullptr; 
}

//---------------------------------------------------------------------------------
void App::BeginFrame()
{
    m_window->BeginFrame(); // windows message pump moved here 
    g_RenderContext->BeginFrame(); // give chance for render context to react to window changes

    // other begin frame things?
    // ...
}

//---------------------------------------------------------------------------------
void App::EndFrame()
{
    m_RenderContext->EndFrame();    // presents the swapchain
    m_window->EndFrame();           // just here for completeness - does nothing
}
```

Our rendering is mostly stripped out at this point, so we'll be just clearing the screen for now

```cpp
//---------------------------------------------------------------------------------
// Game.cpp
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void Game::Startup()
{
    // create a camera for the game to use; 
    m_camera = new Camera(); 
}

//---------------------------------------------------------------------------------
void Game::Update()
{
    float seconds = TimeGetSeconds(); 
    float red = RangeMap( SinDegrees( seconds ), -1.0f, 1.0f, 0.0f, 1.0f ); 
    rgba clearColor = rgba( red, 0.0f, 0.0f, 1.0f );

    // make it so I'm clearing to a different frame each time; 
    m_camera->SetClearMode( CLEAR_COLOR_BIT, clearColor, 0.0f, 0 ); 
}

//---------------------------------------------------------------------------------
void Game::Render() const
{
    g_RenderContext->BeginCamera( m_camera ); 
    g_RenderContext->EndCamera(); 
}
```

------
## Rubric

### Required Tasks
**Required Tasks** are tasks that the course relies on being complete, and will most likely be built upon on later assignments.  All required tasks must be attempted before a bonus task will be graded. 

- [ ] `Window` class
- [ ] `RenderContext` changes
- [ ] `SwapChain` created (seperately)
- [ ] Ability to create a debug context by pre defining `RENDER_DEBUG`
    - [ ] All non-release builds should define `RENDER_DEBUG` for now

### Bonus Tasks
**Bonus Tasks** are tasks that are related to the main assignment, but are not required for finishing the course, hopefully to allow for some personalization of your own engine.

No required tasks will build off bonus tasks, but future bonus tasks may (and they will clearly label which ones they require). 

These tasks *may be completed for any assignment in this semester*, but you must clearly state which extras you are turning in for which assignment.  You may resubmit an extra on multiple assignments to make up lost points.

Example, say for the `DevConsole` assignment, you attempt the extra `auto-complete`, worth 8 points. While testing, I notice that it crashes if backspace is pressed midway through an autocompleted command, but otherwise works.  You are awarded say 5/8 for that extra.  You may then fix that bug, resubmit, and get the remaining 3/8 on a later assignment.  After that though no more points may be acquired from that extra.  

Note, most bonus tasks are twice the work for half the points of a required task, and will not be graded at all if not all required tasks are attempted.  

- [ ] (X01.00 : 05pts) Borderless Window Support
- [ ] (X01.01 : 05pts) Fullscreen Support.  See notes, the default D3D11 support for this will not count.
- [ ] (X01.10 : 02pts) Window alignment options
- [ ] (X01.11 : 01pts) Allow change of window title at runtime
- [ ] (X01.12 : 02pts) Set a custom window icon
- [ ] (X01.13 : 02pts) Allow change of window icon at runtime
- [ ] (X01.14 : 02pts) Show window loading progress in task bar. 

------
## Notes

---

### X01.00 : Borderless Window Support
If you're going to support a full-screen mode, this is the one I'd recommend.

- Fairly straight forward to implement.
- Can be debugged easily
- Not as many problems when gaining/losing focus to worry about.

Things to look into; 

- Window Styles (you want a top level, borderless window)
- Determining Desktop Resolution
  - `EnumDisplayMonitors`
  - `GetMonitorInfo`
- Position the window client region to coincide with the monitor you're full screening on.

---

### X01.01 : Dedicated Fullscreen Support
Similar to the above, but this allows you to fully take over a monitor for your game.  This alone *may* give you some slight performance gains.  The main difference between this is borderless is that you can pick a different resolution then the desktop to switch too (so a 4k monitor may render at 1080p, but still take up the full screen).

If I was writing an Engine, I'd probably not even bother supporting it these days.  Including it mostly for the WinAPI practice; 

Using this mode is similar to borderless in window creation and swapchain creation; 

- When creating your swapchain, be sure to specify you want a fullscreen swapchain.
- Only allow valid supported resolutions for the display; 
  - `EnumDisplaySettings`
- Switching the monitor to a dedicated fullscreen mode;
  - `ChangeDisplaySettings`
---


### X01.10 : Window Alignment Option
When creating the window, be able to specify where it appears within the desktop. 

Support the following changes; 

```cpp
void Window::Open( std::string const& title, float heightRatio, float aspect, vec2 alignment ); 
void Window::SetAlignment( vec2 alignment ); 
```

This alignment should affect the full window size after styles have been taken into account. 

---

### X01.11 : Change Title at Runtime
Just be able to change the title of the window at run time, giving your window a method
such as...

```cpp
void Window::SetTitle( std::string const& title ); 
```

---


### X01.12 : Set Custom Window Icon
When creating your window, be able to specify a set of icons to use.  This is handled when you
register the window class.  You can have the icon loaded as a resource embedded in your executable, as a file near your exe, or as an image your construct in code. 

Icons use the *.ico file format.

---

### X01.13 : Change Window Icon at Runtime

Be able to set the icon of your window (and the taskbar icon) at runtime - really a novelty feature.  

Usually, if you're going to set an icon, you would set it when you create the window.  But you can set it after the window is created by passing it special window messages, `WM_SETICON`.

---

### X01.14 : Windows Taskbar Loading Progress
A somewhat new feature in Windows is the ability to show progress in the task bar.  Nice if you have an idle game, tools, etc, where you expect the user to alt-tab away;  

Look into `ITaskbarList3` interface for implementation; 

Suggest an interface such as...

```cpp
enum eWindowProgressMode
{
    WND_PROGRESS_NONE,
    WND_PROGRESS_INDETERMINATE, 
    WND_PROGRESS_VALUE,
    WND_PROGRESS_ERROR, 
    WND_PROGRESS_PAUSED,
};

void Window::SetProgress( eWindowProgressMode mode, float progress = 0.0f ); 
```
