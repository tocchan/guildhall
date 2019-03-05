A04 - Torque
======

## Overview

## Grading 
- [ ] Allow static objects to transfer velocity (be able to "hit" things)
- [ ] Support applying "Torque" to an object; 
- [ ] Physics step takes into account torque, angular velocity, and angular momentum; 
- [ ] Support Rigidbody Constraints (x, y, and rotation)
- [ ] Support transfer of linear momentum
- [ ] 

## Resources
- http://www.euclideanspace.com/physics/dynamics/collision/twod/index.htm#twodr : Our actual problem
- https://en.wikipedia.org/wiki/Moment_of_inertia : Needed for the physics step (objects will have torque applied to them - how does the physics step handle it)
- https://en.wikipedia.org/wiki/Parallel_axis_theorem : Used for calculating moments of inertia from simpler shapes; 
- https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics : Found it generally pretty help coming to grips with terms; 

## Demo
*Coming Soon*


## Source (Headers)

## Checklist

[ ] Dynamics -vs- Statics.  Take into account static velocity
  - [ ] Calcualte static velocity based on movement since last frame
  - [ ] Introduce `Rigidbody2D::Teleport` to move something but keep previous velocity; 
  - [ ] Solve the inelastic equation assuming infinite mass for the static object
