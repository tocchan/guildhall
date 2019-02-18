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


## 3: Mouse Input
We're going to tie the mouse to moving our camera, so let us hook up the mouse;  (need to see someones engine for this)

- `void MouseSetMode( MOUSE_RELATIVE );` 
- `void MouseHideCursor();` 



  