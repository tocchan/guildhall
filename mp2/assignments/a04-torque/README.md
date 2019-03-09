A04 - Torque
======

## Overview

## Grading 
- [ ] All features from previous assignment work
- [ ] Objects can be created with any set of 3 constraints (x, y, z)
- [ ] Be able to create a level, and save it out (will save out all static geometry)
- [ ] Paddles Work

## Resources
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032 : Equations at the bottom.
  - http://chrishecker.com/images/e/e7/Gdmphys3.pdf : Link from previous article explaining the equations;
  
- http://www.euclideanspace.com/physics/dynamics/collision/twod/index.htm#twodr : Our actual problem
- https://en.wikipedia.org/wiki/Moment_of_inertia : Needed for the physics step (objects will have torque applied to them - how does the physics step handle it)
- https://en.wikipedia.org/wiki/Parallel_axis_theorem : Used for calculating moments of inertia with an offset from the axis.
- https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics : Found it generally pretty help coming to grips with terms; 

## Demo
*Coming Soon*


## Source (Headers)

## Checklist

- [ ] Calculate Moment of Inertia for objects
    - [ ] Based on mass and collider shape
    - [ ] Use parrallel axis therem when colliders have a local offset from the rigidbody (center of the local shape)
- [ ] Implement Torque and Angular Velocity
    - [ ] Update takes that into account
- [ ] Update collision code to deal with impulse forces
- [ ] Apply that impulse immediately to resolve the collision.  Use current physics timestep for resolving it (impulse just will influence velocities, not positions)
- [ ] Implement Constraints
    - [ ] Implement positional (x,y) constraints
	    - [ ] For correction, solve correction on a per-axis basis, treating thing with contraints as static (so fully push in that direction)
	    - [ ] For collision response, just solve as normal as if it didn't have a constraint (not sure if this is correct, but we'll see how it feels)
    - [ ] Implement rotational constraint
        - [ ] When resolving an impulse, use the rotational constraint to determine how much energy goes toward angular momentum and how much goes toward linear velocity;  
