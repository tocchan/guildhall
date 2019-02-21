A04 Outline
======

Any section sharing a number (ex: 2A, 2B) can be done in any order within that section; 

## 1: Initial Setup
- Make Protogame3D (as we'll default this to a 3D/Perspective Camera)
- Update `camera_buffer_t` to have `mat44 view;` and `mat44 projection` matrices; 
  - Default both to `mat44::IDENTITY`
- Update your shader to support this;  `default_unlit.00.hlsl`
- Clear to black and draw a white triangle (should take up bottom left side of the screen)
   `{ (-1.0f, -1.0f, 0.0f), (1.0f, -1.0f, 0.0f), (-1.0.f, 1.0f, 0.0f) }`
- CHECKPOINT 1:  Make sure you see that triangle; 
- Set an ortho projection, using size 10.0f
  - Triangle should now take up 1/5th, in the center of the screen; 


## 2A: View Matrix
- In game, create a position (vec3) you can control (WASD).  Need at least controls for Left/Right and Forward/Back; 
- Construct a translation matrix based on this position and set it as the Camera's model matrix;  
- Set the camera's model using `Camera::SetModelMatrix` and set its view to the inverse (`InvertOrthonormal`).  This should end up just being the negative translation. 
- Run the game, make sure you can now move (you move right, the triangle should move left for example).  Forward and back movement will not do anything yet; 


## 2B: Perspective Projection
- Implement or copy the projective matrix code.
- Implement `Camera::SetPerspectiveProjection` - use the currently bound color target to determine aspect ratio. 
- Run game again - moving forward and back should now have a noticable difference in size; 

## 3: Window Context
- Implement `WindowContext` per spec
- Update `Main_Win32.cpp` to create a `WindowContext` instead of the `Window`
- Update `RenderContext` to take a `WindowContext` to startup (instead of an HWND)
- `ProcessMessagePump` rolls into `WindowContext::BeginFrame`
- Create a `AppWindowsProc` method that does `App` and `Game` things with the window
  - Register this using `WindowContext::SetAppWindowsProcedure`


## 4: Mouse Input
- Implement `WindowContext::GetMouseClientPosition()`
- Button Input - ???  (depends on implementation from SD1)
- Implement `WindowContext::SetMouseMode( eMouseMode mode );`
- Have the game set itself `MOUSE_MODE_RELATIVE`
  - Be sure that when gain focus you re-lock the mouse; 
  - Print out client position in your window to test (should be hw, hh)
- Use `WindowContext::GetMouseFrameMovement()` to get mouse to get `pitch` and `yaw` for the camera;
  - `pitch` is rotation around right, `yaw` is rotation around up.  For now this will be I and J; 
  - Update the Camera Model matrix using current yaw/pitch, should now be able to move around; 


## 5: Camera Relative Movement
- Instead of moving in absolute world directions, we will now tie movement controls to `Camera`'s current 
  basis; 
- Update rotation first, then use `Matrix4x4::GetI()`, `GetJ()`, and `GetK()` to tell you which direction you want to move;
  - This is equivilant to multiply your "local" movement by the rotation matrix; 
- Update position
- Translate your rotation matrix;
- Apply this as your Camera's model matrix. 


## 6: Constructing a Cube
- Implement `IndexBuffer`
- Implement `Mesh` 
- Implement `AddCubeToMesh`
- Construct a `Mesh` for game, and draw it every frame; 
- Should not be able to fly around a box (though it'll possibly look inverted due to no depth buffer)


## 7: Depth Buffer


## 8: UV Sphere


  