Mouse Input
======

## Contents

**BASICS** 

- Spaces
- Absolute & Relative Modes
- Position
- Frame Delta Position
- Buttons & Mouse Wheel
- Hiding the Mouse
- Locking (Clipping) the Mouse to the Window
- Detecting Window Focus

**ENGINE SPECIFIC**

- Storing Mouse Data
- Camera Class Extensions


## Basics
*Note To Self; Should I rename Screen Space to Display Space, or ClientSpace to be more consistent with windows?*

### Spaces
- Desktop Space:  Pixel space of the entire desktop of the windows machine.  X-Right, Y-Down
- Client Space (we'll call this **Screen Space** in code):  Pixel space, with the top left of the renderable (client) part of our window being (0,0).  X-Right, Y-Down. 
- World Space: The space we want to think of the world in.  So even if our resolution is (1280 x 720), we can think of it as (17.78 x 10).


### Absolute vs Relative Modes
Useful to have at least the two following cursor modes available to your engine;

- **Absolute**: How you usually use the mouse.  The moues position corresponds to where it is on screen, and the user can move it freely.  May or may not be locked to the window.  Useful for tools and RTS (real time strategy) games for example. 

- **Relative**: Mouse is hidden, and absolute position ignored or unusable.  Mouse is always locked to the window.  The only available information is how far did it move since last frame (relative offset).  Useful for FPS (first person shooters) for example.


### Getting/Setting Mouse Position
```cpp
::GetCursorPos( POINT* ); 
::SetCursorPos( int x, int y ); 
```

### Frame Delta Position
To determine how much a mouse has moved a frame, we just need to keep track of the last
place we read from (ie, keep where the mouse was last frame, and store the different to this frame).

When doing *Relative* mouse mode, you will reset the mouse position to the center of the screen after reading.  In which
case the previous position you would use to determine relative movement would always be the center of the screne. 

// Get, Set, Get


### Buttons & Mouse Wheel
Windows messages `WM_MOUSEWHEEL` and `WM_MOUSEMOVE` are used to determine the state of the mouse, as well as when the mouse wheel is used;

- [WM_MOUSEMOVE](https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-mousemove)
- [WM_MOUSEWHEEL](https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-mousewheel)
- [WM_LBUTTONDOWN](https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-lbuttondown) & [WM_LBUTTONUP](https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-lbuttonup).  Similar for `RBUTTON`, `MBUTTON`, etc...


### Hiding/Showing the Mouse
We will at times want to hide the Windows Cursor, either because we want to use our own, or we are in *Relative* mode.  

```cpp
// Our API
void ShowMouse(); 
void HideMouse(); 

// Win32 API Used
::ShowCursor( BOOL ); 
```

Upon losing or gaining focus, no action is needed as the cursor state is stored with the HWND.  

This call is actually a counter.  Your cursor starts with a count of `0`.  Every time you hide the cursor, the count is decreased, and every time you show cursor the count is increased.  The cursor will be visible be visible if the count is ever `>= 0`.  

`::ShowCursor` will return the value of the count after the operation, so if ever need to force it, you can do a tight while loop until the count is to a value you want (0 or -1 usually). 


### Locking/Clipping The Mouse

```cpp
// Our API
void LockMouse(); 
void UnlockMouse(); 

// Win32 API Used
::ClientToScreen( HWND, POINT *outPos ); 
::GetClientRect( HWND, RECT *outClientRect ); 
::ClipCursor( RECT *screenRect ); 
```

Be sure to unlock the the mouse window loses focus, and to relock the mouse upon gaining focus or if the window has moved. You can listen for `WM_MOVE` messages in the `WindowsProcedure`, and re-clip to the new position (since clip rectangles are specified in Desktop Space).  

This is not reference counted like cursor visibility, so you may want to do your own internal reference counting for this state.

*Hint:  Useful to have your dev-console unlock the mouse when opened for debug purposes.*


### Detecting Window Focus

When a window gains or loses focus, it will be sent a `WM_ACTIVATE` message to the `WindowsProcedure`. 

- [WM_ACTIVATE](https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-activate)

You can check the `LOWORD` of the `wParam` as non-zero to see if your window currently has focus.  If it is zero, you have lost focus.  Be sure to appropriately free up the mouse.   


## Engine System 

### Storing Mouse Data

- Mouse Keys are passed through "Key Events" - with fabricated keys; 
- 

```cpp

// Where does "mouse" information live?
// - InputSystem?
// - StandAlone?
// - Events?


```

### Camera Class Extensions

We want to add the following functionality to our camera for helping us convert from a screen coordinate (what the mouse input system will give us) to a world coordinate (such as a UIs orthographic space). 

```cpp
class Camera
{
	public:
		// Outputs ===---
		void SetColorTarget( ColorTargetView *ctv ); 

		// Viewport ===---
		void SetNormalizedViewport( vec2 min, vec2 max ); // treat this as (0,0) bottom left, (1,1) top rihgt
		aabb2 GetNormalizedViewport() const; 
		aabb2 GetPixelViewport() const; 		// use color target to determine the pixel viewport size; 

		// Projections ===---
		void SetProjectionOrthographic( vec2 min, vec2 max, float minZ = 0.0f, float maxZ = 1.0f ); 

		// Space Converions ===---
		// screen to world space
		vec2 ClientToUV( vec2 client ) const; 			// Not particularly useful yet, 
		vec3 ClientToNDC( vec2 client, float ndcDepth = 1.0f ) const;  
		vec3 NDCToWorld( vec3 ndc ) const;
		vec3 ClientToWorld( vec2 client, float ndcDepth = 1.0f ) const;
		// Ray3D ClientToWorldRay( vec2 client );   // Later when we start talking about ray casting/picking

		// Helpers when thinking about in-world UI
		// vec3 UVToNDC( vec2 uv, float ndcDepth = 1.0f ); 
		// vec2 UVToWorld( vec2 uv, float ndcDepth = 1.0f ); 

		// world to screen space(s); 
		vec2 WorldToClient( vec3 worldPos ) const; 
		vec3 WorldToNDC( vec3 worldPos ) const;  // WorldToViewport?
		vec2 WorldToUV( vec3 worldPos ); const;  

	public:
		ColorTargetView *m_colorTarget; 
		// DepthStencilTargetView *m_depthStencilTarget; 

		aabb2 m_pixelViewport;	    // Viewport needed by the API - very render target specific.  Min is top-left corner; 
		aabb2 m_normalizedViewport; // I like thinking of the screen as a 0 to 1 (bottom-left to top-right) - easier to cut the screen in half. 

		mat44 m_projection; 
		// mat44 m_view; // mat44 m_camera = Invert(m_view); ?  When added these will affect the above methods; 
}; 
```
