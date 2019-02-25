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
- Implmeent `CPUMesh` used for construction
  - This uses a vertex type `VertexMaster` that will be the super-set of all other vertex types used in class.
- Implement `AddCubeToMesh`
- Implement `GPUMesh` and `GPUMesh::CreateForCPUMesh`
- Implement `RenderContext::DrawMesh( GPUMesh *mesh )`
- Have game create a `GPUMesh` for a cube, and draw it each frame; 
- Should not be able to fly around a box (though it'll possibly look inverted due to no depth buffer)

## 6b: *Optional*:  Update `RenderContext::DrawVertexArrays`
- *Goal of this is to consolidate our rendering paths to just `DrawMesh`
- Implement `GPUMesh::UpdateFromVertexArrays` 
- Create a member on `RenderContext` that is `GPUMesh m_immediateMesh`
- Have `RenderContext::DrawVertexArrays` copy to `GPUMesh`, and then use `DrawMesh`  

## 8: UV Sphere
- Implement `AddUVSphereToMesh`
  - This uses SphericalCoordinates to generate; 
- Repeate steps for Cube

## 7: Depth Buffer
- Implement `Texture2D::CreateDepthStencilTargetFor(...)`
- Implement `DepthStencilView` class; 
- Implement `Texture2D::CreateDepthStencilTargetView(...)`
- Have `RenderContext` create a default back buffer member `m_defaultDepthStencilTexture` & `m_defaultDepthStencilView`
- Implement `Camera::SetDepthStencilView` - setting to nullptr should use the default backbuffer;
- Update `RenderContext::BeginCamera` to also set the depth stencil view - (make sure all color targets/depth stencil are the same size at this point!)
- Update `Shader` so that it will use the depth buffer
  - Implement `Shader::SetDepth`

## 7b: Shader XML
- Support being able to load a shader from an **xml** file. See Example
  - *Note: This does **NOT** replace creating directly from hlsl - check the extension to figure out the code path*
- It should currently support the following; 
  - Programmable Stages to use
  - Blend State
  - Depth Stencil State; 





  