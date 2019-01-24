Class Notes: 2019-01-23
======

## Resources
- [D3D11 Tutorials](http://www.directxtutorial.com/LessonList.aspx?listid=11):  A good tutorial series on setting up D3D11

## Branching Protogame
- [Mirror a Repot](../../../examples/common/gitclone)

First step will be to clone your Protogame to a new repot for this class, called 'RTS' or some variant.  

Once this is done, create a new branch for the Engine submodule called 'feature/d3d11', and make sure it is pushed to remote.  

## Converting to D3D11

### The Program
Our goal is to get code similar to the following working in your Engine; 

```cpp
void App::RenderFrame()
{
	RenderContext *rc = App::GetRenderContext();
	float time = GetCurrentTime();

	rc->BeginFrame(); 

	rc->BeginCamera( m_camera );

	// pick a animated clear color so we know multiple frames are being renderered
	// this moves between blue and red on a sin wave; 
	float lerpValue = (sin(time) + 1.0f) * .5f; 
	rgba clearColor = Lerp( rgba::BLUE, rgba::RED, lerpValue );  
	// TODO, rename to ClearColorTargets
	rc->ClearScreen( clearColor );  

	rc->EndCamera(); 

	// This was a gl call 
	rc->EndFrame(); // move swap buffers here;  
}
```

Get this coded up in OpenGL, or something similar that matches your current code base.  Once it works, check it in and push to origin.  

*Note: Remember if you made engine changes, to push Engine before pushing your game code so the engine commit number is updated.*

### Converting to D3D11

#### Cleanse your codebase of OpenGL
First into App, and break up the `CreateWindowAndOpenGLContext` method or equivalent into two pieces;  `CreateWindow` and `CreateRenderContext`.  The second function will just do the following; 

```cpp
void CreateRenderContext() 
{
	g_theRenderer = new RenderContext( g_hWnd );
}
```

All of render context creation will be moved to the constructor, and all it should need for this is the handle to the window, `g_hWnd`, that was created during the windows creation step. 

Next step to this refactor is we're going to strip OpenGL from our Engine, but leave the rendering interfaces intact (ie, RenderContext::BeginCamera stays, but may not doing anything yet).

Any function you delete the contents of, replace with a `GUARANTEE_AND_CONTINUE( false, "helpful message" );`.  Do this until it compiles.  Running it should hit this guarantees to let you know you still have work to do.  But this is a compilable state, so check-in and push to origin.  


#### D3D11 Initial Steps 
Next you need to create the `Device`, `Context`, and `SwapChain` as outlined in this demo; 

- [D3D11 Setup Code](../../../examples/d3d11/setup)

All code will be under `src/demo.cpp`.  The constructor will handle creating the pices.

`RenderContext::BeginFrame` should then do the step of grabbing the next backbuffer and creating a render target view for it; 

`RenderContext::EndFrame` will be where `Present` works, and freeing up the render target view for that frame.

`RenderContext::ClearScreen` or `RenderContext::ClearColorTargets` if you renamed it will be where the clear code should live.

That should be all that is needed to get a working clear screen.

