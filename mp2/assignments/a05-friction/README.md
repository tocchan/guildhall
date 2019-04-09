A05 - Friction, Drag, Constraints, Clocks, and Levels
======

## Overview
This assignment is a bunch of little topics; 

First, we'll handle frictional forces.  When an object collides with another object, we first have the normal impulse, but there is also a frictional impulse.  This is nearly exactly the same as your normal impulse calculation, but is instead calculated using the tangent for the impact. 

Second, we'll introduce the idea of `drag forces`, which we'll wave our handes at and just say it is a force that will act opposite our current velocities, hence slowing us down to a stop over time;  

Third, we'll add the ability to constaint objects.  **Constaints** in the context of this class is restricting an axis of movement.  For 2D, we have three possible constraints, **X**, **Y**, and **Rotational** limits; 

Fourth, we'll add the concept of nested clocks, so we can slow and speed up time for only our game or simulation and not the entire system. 

And finally, we'll add teh ability to save our and load a level.  


## Grading 
- [ ] 20%: Expose Friction controls for objects
- [ ] 10%: Implement a hacky `drag` and `angular_drag` 
- [ ] 10%: Friction and Drag appear in the debug view; 
- [ ] 30%: Expose contraint controls for objects
- [ ] 10%: Ability to control time for only the game simulation (using a nested clock). 
- [ ] 10%: Add console command to save all objects to a file (your choice between XML or Binary format)
- [ ] 10%: Add console command to load all objects to a file. 


## Resources
- https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table--gamedev-7756:  Handling Friction tutorial.  *Note: Determining direction of tangent vector is over-thought.  You can always just rotate your normal by -90 degrees to get the tangent to use for impulse calculations*. 


## Demo
- [./mp2.a05.zip](./mp2.a05.zip)


## Source (Headers)

## Checklist
- [ ] Implement Friction
    - *Note: up to you if you want to support both static and kinetic friction, or just kinetic*.  
       -  Static friction makes a stopped body harder to move.  It is more accurate, but may not be needed in a game environment. 
    - [ ] Add friction to `Rigidbody2D`
    - [ ] Determine how to combine friction for a collision (similar choice to restitution calculation - left to reader's discretion within reason (ie, don't just return 0))
    - [ ] Calculate frictional impulses along the tangent of impact. 
    - [ ] Apply this impulse to impacted bodies; 
    - [ ] Test by trying to roll an object down a hill; 
- [ ] Implement Drag Forces (drag constants) - we'll do a fairly weak version
    - [ ] Linear and Angular Drag Constants Exposed
    - [ ] During a rigidbody update, apply a force that is `-velocity * drag` before apply forces that frame.  **This is not physically accruate, but does get the idea across**
- [ ] Expose Constraints for `Rigidbody`
    - [ ] Support `Rigidbody2D::SetConstraints( eRigidbodyConstraints constraints );`
    - [ ] Support `RIGIDBODY_CONSTRAIN_X`, `RIGIDBODY_CONSTRAIN_Y` and `RIGIDBODY_CONSTRAIN_ROTATION`
    - [ ] Make appropriate changes throughout update loop to prevent that object from moving
        - Be sure to also zero out velocities for constrained axes, otherwise collisions can get funny. 
- [ ] Change game to use its own clock, which the Physics System uses; 
    - [ ] Create a Game Clock
    - [ ] `PhysicsSystem2D` should take a clock to use at `Startup`. 
    - [ ] Previous time constrol keys from SD1 should now affect the master clock.
    - [ ] Add controls controls for the Game Clock
        - [ ] Play/Pause
        - [ ] Step 
        - [ ] Speed Up
        - [ ] Slow Down
        - [ ] Reset Scale
    - [ ] Confirm you can slow the game clock without slowing the system clock by pausing and then opening your dev console (the blinking or dev console animations should not be affected by the game clock, but game simulation should be)
- [ ] Implement Save/Load
    - [ ] ??? (How I will suggest doing this task will depend on what you have from SD1)
    - [ ] When loading, destroy all objects before loading


