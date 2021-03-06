A03 - OBB & Capsule
======

## Overview
We will be implementing two new objects;

- Oriented Bounding Box (`OBB2`)  
- Capsule (`Capsule2`)

This assignment will likely change nothing about your simulation, but will give us some new shapes to play with, and set us up for rotational 
velocity in the next assignment; 

Some tertiary goals for this assignment are as follows
- Implement mouse controls
- Merge the D3D Branch, and setup a game orthographic camera (we can get a larger scene)
- Implement a larger play space; 

## Grading 
- [ ] *30%*:  Demo works with Capsules
- [ ] *30%*:  Demo works with OBBs
- [ ] *40%*:  New input control scheme; 
    - [ ] *20%*: Mouse controls   
    - [ ] *10%*: Ability to Zoom
    - [ ] *10%*: Camera is contained to world bounds; 


## Resources
*Note: If you find a good resource on this - please share to help future cohorts;*

- [TutsPlus: Seperating Axis Theorem](https://gamedevelopment.tutsplus.com/tutorials/collision-detection-using-the-separating-axis-theorem--gamedev-169)
- [RealTimeRendering.com: Intersections](http://www.realtimerendering.com/intersections.html)


## Demo
- [mp2.a03.zip](./mp2.a03.zip)


## Source (Headers)
- [Capsule2.hpp](./src/Capsule2.hpp)
- [OBB2.hpp](./src/OBB2.hpp)
- [Plane2.hpp](./src/Plane2.hpp)
- [Segment2.hpp](./src/Segment2.hpp)

## Checklist

### Core Tech
- [ ] Merge D3D11 Branch into Master - fix resultant bugs; 
- [ ] Implement `Capsule2`
- [ ] Implement `OBB2`
- [ ] Add ability to render both (good test case for using index buffers)
    - *These names/fucntions are suggestions.  As long as the shapes are visually on screen, you're good for this assignment.*
    - [ ] `AddBoxToMesh( Mesh *out, OBB2 const &obb, RGBA const &color )`
    - [ ] `AddCapsuleToMesh( Mesh *out, Capsule2 const &cap, RGBA const &color );` 
- [ ] Add `Collider2D` types for both
    - [ ] `BoxCollider2D` for `OBB2`
    - [ ] `CapsuleCollider2D` for `Capsule2`
    - *Note: These can completely replace `AABB2` and `Disc` if you don't want to write as many collision cases.*
- [ ] Implement `Plane2` class - will help with the manifold generation; 
- [ ] Implement `GetManifold` for the same cases;  These will be used for collisions; 
    - [ ] `GetManifold( manifold2 *out, OBB2 const &a, OBB2 const &b, float radius )` 
    - [ ] `GetManifold( manifold2 *out, OBB2 const &a, OBB2 const &b )`
    - [ ] `GetManifold( manifold2 *out, Capsule2 const &a, Capsule2 const &b )`
    - [ ] `GetManifold( manifold2 *out, OBB2 const &a, Capsule2 const &b )`

### Input and Game
- [ ] Define a `WorldBounds` AABB2 as part of your game;
- [ ] Define a game camera that you can move around using `WASD` keys, but is contained within the world box; 
- [ ] Move cursor control over to the mouse; 
    - [ ] Hide the mouse for the game (we're using a custom cursor)
    - [ ] Convert client mouse position to world position, and set your cursor there each frame; 
        - [ ] Implement `Camera::ClientToWorldPoint( vec2 clientPoint )`
    - [ ] Left Click selects while mouse is held down, deselects once mouse is released; 
    - [ ] Ability to move selected object (possess)
    - [ ] Right Click creates an objects
        - [ ] Can set create options: *Type, Simulation Mode, Size, Mass, and Restitution*  
        - [ ] Start point picked when mouse held down
        - [ ] End point picked when mouse is released - object is created between start and end points (see demo) 
    - [ ] Can zoom using mouse wheel (or keyboard keys)


