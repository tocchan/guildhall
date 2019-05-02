A11 Suggested Order
======

## Debug Render Needs
- Be able to Debug Render a 3D Capsule and 3D Wire Capsule


## Ray3
- Add Ray3, and implement raycast methods needed
- Implement `Camera::ScreenPointToWorldRay(...)`
  - You can use `DebugRenderLine` over a few seconds to debug that you did this right.  Draw your raw, and move the camera, you should see your ray left in the world;  If not moving the camera, you should see just a single point; 
- Raycast against the plane of the map, and draw a point at the intersection point to test your Ray/Plane Intersection
- Use `DebugRenderCapsule` to render a capsule, and change color when you hit it.  Draw a point at intersection points; 


## Map & Game Simulation
- Add `GameHandle` to Game
- Add Entities to game
- Hook spacebar to spawn a 
