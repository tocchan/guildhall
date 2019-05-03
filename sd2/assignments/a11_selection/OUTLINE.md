A11 Suggested Order
======

## Debug Render Needs
- Be able to Debug Render a 3D Capsule and 3D Wire Capsule


## Ray3
- Add Ray3, and implement raycast methods needed
- Implement `Camera::ScreenPointToWorldRay(...)`
  - You can use `DebugRenderLine` over a few seconds to debug that you did this right.  Draw your raw, and move the camera, you should see your ray left in the world;  If not moving the camera, you should see just a single point; 
- Implement `Map::RaycastTerrain` to give intersection point for map
  - Raycast against the plane of the map...
  - `DebugRenderPoint` at the intersection point to test this
- Use `DebugRenderCapsule` to render a capsule, and change color when you hit it.  Draw a point at intersection points; 
  - This will test your Ray/Capsule intersection, and therefore your Ray/Sphere intersection

## Map & Game Simulation
- Add `GameHandle` to Game
- Add Entities to game
- Setup the command queue on `Game`
  - A frame will now want to do these steps in this order; 
    1. UpdateInput
    2. Process Commands
    3. Update Map (and therefore entities, will also purge)
    4. Render
- Create a `CreateUnitCommand`
  - GameController creates this command when spacebar is pressed
  - Should spawn at where the ray intersects the map.
- `Map::CreateEntity` implemented, and properly give a unique handle
- `Map::FindEntity` implemented for fetching an entity from a handle
- Have entities render themselves using a capsule or wire capsule
- Allow `GameController` to select a unit
  - `Map::RaycastEntity` raycasts against selectable entities
- Keep track of selected unit on the Game Controller
- Have Right Click issue a `MoveCommand`, targeting the selected unit and moving to the ray/map intersection point.

