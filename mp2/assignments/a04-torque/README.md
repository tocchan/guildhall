A04 - Torque
======

## Overview
Working off previous assignment, we'll now add teh ability for objects to rotate and be oriented.  This involves updating our intersection code to generate a contact point, as well as be able to apply rotational forces (torque).  

We'll add some debug tools as well to confirm things are working.  


## Grading 
- [ ] All features from previous assignment continue to work
- [ ] Intersection code now also returns a contact point
- [ ] Debug Menu showing object state; 
    - [ ] At minimum must include...
        - [ ] Position
        - [ ] Mass
        - [ ] Velocity
        - [ ] Moment of Inertia
        - [ ] Angular Velocity
- [ ] Collisions now properly affect angular movement


## Resources
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032 : Rotational forces and manifold generation tutorial.  
  - http://chrishecker.com/images/e/e7/Gdmphys3.pdf : Link from previous article explaining the equations;
- https://en.wikipedia.org/wiki/Moment_of_inertia : Needed for the physics step (objects will have torque applied to them - how does the physics step handle it)
  - https://en.wikipedia.org/wiki/List_of_moments_of_inertia : Common moment of inertia equations 
  - https://en.wikipedia.org/wiki/Parallel_axis_theorem : Used for calculating moments of inertia with an offset from the axis.  Needed with above to determine our shapes' moments of inertia. 
  - 
- https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics : Found it generally pretty help coming to grips with terms; 

## Demo
- [A04 Demo](./mp2.a04.zip)

## Source (Headers)
- ...

## Checklist
- [ ] Be able to calculate contact points for Rounded Box to Rounded Box
  - [ ] Point to Point
  - [ ] Point to Edge
  - [ ] Edge to Edge  (trickier)
  - *Note: Debug Draw system for rendering impacts really helps in debugging this*
- [ ] Calculate Moment of Inertia for objects
    - [ ] Updated either when a mass or collider changes
    - [ ] Use parrallel axis therem when colliders have a local offset from the rigidbody (center of the local shape)
        - *Note: You can get by without doing this "yet", but eventually we will have paddles that have an off-center axis of rotation.  Just keep a note you'll be needing to do it at some point.*
- [ ] Implement Torque and Angular Velocity
    - [ ] You can add torque like you can accumulate forces per frame, and are reset at the end of a frame. 
    - [ ] Angular Velocity is added to the `Rigidbody2D`
    - [ ] Torque is used to update angular velocity, which in turn updates rotation; 
    - *Note: All of our shapes are some variation of a rounded OBB, so you could effectively just support that calculation.  Look for moment of inertia for a box and a disc (cylindar).*
- [ ] Update collision code to deal with impulse forces
    - [ ] Implement `Rigidbody2D::ApplyImpulses` and `Rigidbody2D::ApplyImpulseAt` methods
    - [ ] Update impulse calculation to take into account effective velocity at point of contact, and rotational momentum
    - [ ] Apply impulse at point of contact; 


## Notes
- One way to handle an explosion is to take the point closest to the explosion origin, and apply an impulse force.  Having a generic "get closest point" for your shapes would be useful for this;  If you are inside the shape - treat the normal as going directly to the center; 
- A `Motor` can be used to add forces/torque to a `Rigidbody` each frame.  This will go well with `Effector` and `Joint` concepts later; 
