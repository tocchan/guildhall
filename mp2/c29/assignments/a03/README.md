MP2.A02 Polygons and Physics Step
======

## Overview
Add in support for polygon colliders and get the basic physics loop in place. 

**Turn-In Branch Name: `mp2/turnin/a03`**

**Due Date: Feb 13, 2020**

### Goal 
Some example code for how the system will be used in this assignment;

------

## Checklist
I may mark tasks as **CHALLENGE** if I believe them to be more difficult relative to the rest of the assignment.  This is just to hint to you that you may want to save those tasks for the end if you have time, or skip altogether if not.  

- [ ] *25pts*: Add a `PolygonCollider2D`
    - [ ] *02pts*: Can construct from counter-clockwise list of points
        - [ ] `GUARANTEE_OR_DIE` points represent a convex polygon
    - [ ] *05pts*: **CHALLENGE** - Can construct from a point cloud
    - [ ] *03pts*: Implmeent `Contains`
    - [ ] *05pts*: Implement `GetClosestPoint`
    - [ ] *05pts*: Implement `Intersects`, but only vs disc, return `false` if intersecting with another polygon for now.
        - [ ] Be sure disc now checks against both discs and polygons useing `GetType`
    - [ ] *05pts*: Can render it - should work as before.
- [ ] *25pts*: Can construct `GameObject` with a `PolygonCollider`
    - [ ] Press `2` to enter a polygon draw mode, place first point where cursor is when `2` was pressed
    - [ ] Draw a line from last point added to cursor
    - [ ] When pressing `Left Mouse Button`, add a point if it is a legal (will maintain convex polygon)
    - [ ] When pressing `Right Mouse Button`, end, closing off the loop.
    - [ ] Pressing `Escape` while in this mode should cancel the creation.
    - [ ] Construct object with polygon as its collider, exiting creation mode
        - [ ] Pick 'center', or position of rigidbody by either
            - Average of all points
            - Center of box that encapsulates all points
            - **CHALLENGE**: Center of mass, assuming uniform density
- [ ] *05pts*: `Rigidbody2D` can `Enable` or `Disable` itself, taking it out of the simulation, but still rendering
    - [ ] Objects being dragged should set themselves to disabled, and re-enable when unselected.
- [ ] *05pts*: When drawing, draw the location of the `Rigidbody` as an `X`, red if disabled, blue if enabled. 
- [ ] *10pts*: `Physics2D::Update` loop implemented
    - [ ] Apply all global forces
    - [ ] Simulate all rigidbodies
    - [ ] Cleanup destroyed objects (should be happening from A01)
- [ ] *10pts*: Add `Rigidbody2D::SetVelocity` to change the velocity of the object
    - [ ] When releasing an object, set its velocity based on mouse movement (ie, be able to throw an object)
- [ ] *05pts*: `Rigidbody2D` can `SetSimulationMode` to either `STATIC`, `KINEMATIC`, or `DYNAMIC`
    - [ ] `STATIC` objects do not apply forces or simulate.
    - [ ] `KINEMATIC` objects do not apply forces, but will simulate.  
    - [ ] `DYNAMIC` objects apply forces and simulate
    - [ ] While selected, `1`, `2`, and `3` should switch the object to `STATIC`, `KINEMATIC`, or `DYNAMIC` respectively
- [ ] *05pts*: `Physics2D::SetSceneGravity` implemented
    - [ ] Applies a flat acceleration force to every dynamic rigidbody in the scene beginning of frame. 
    - [ ] Use `+` and `-` keys to adjust gravity amount
    - [ ] Current gravity should be visible on the screen (hotkey hint here is nice)
- [ ] *05pts*: Objects that fully fall off the sides of the screen should appear on the opposite side, maintaining velocity
- [ ] *05pts*: Implement `Rigidbody2D::GetVelocity`
    - [ ] Objects that falling off the bottom of the screen, reverse its velocity along Y (bounce it)
    - [ ] Only bounce if it is falling down (-y)

------

## Resources

### Notes
- [Polygon Notes](./polygon.md)
- [Polygon -vs- Disc](./poly-v-disc.md)
- [Polygon -vs- Plane](./poly-v-plane.md)
- [Collision Matrix](./collisionmatrix.md)
- [Physics Step Notes](./physicsupdate.md)


### Links


