MP2.Final : Fleshing Out the System
======

## Overview
Just going to flesh out the physics system so it can easily be used for a game.


## Checklist 

- [ ] Be able to attach user data to a rigidbody and/or collider to hook the physics system up to game code.

- [ ] Add collision/contact events
    - [ ] `OnOverlapBegin`
    - [ ] `OnOverlapStay`
    - [ ] `OnOverlapLeave`

- [ ] Add Trigger Volumes
    - [ ] `OnTriggerEnter`
    - [ ] `OnTriggerStay`
    - [ ] `OnTriggerLeave`

- [ ] Support Physics Layers
    - [ ] Be able to specify which layer a rigid body and/or collider belongs to.
    - [ ] Be able to enable or disable collision between two given layers.
    - [ ] Only process collisions if the two objects are allowed to interact
    - [ ] Only process triggers if the two objects are on the same layer

- [ ] **Optional**: Support axis locks.
    - [ ] Support `X` and `Y` axis locks, only allowing movement in those directions.
    - [ ] Support `Rotation` lock preventing the object from rotating. 


------
## Implementation Notes
I'm going to leave a lot of lee-way in how you implement the above features, but will discuss some options below.

- [./userdata.md](./userdata.md)
- [./collisionevents.md](./collisionevents.md)
- [./triggerevents.md](./triggerevents.md)
- [./layers.md](./layers.md)
- [./axislock.md](./axislock.md)


------
## Next Steps
This assignmetn just fleshes out the features of the system, but still has a lot of room for improvement.

If you find yourself using it in DFS or Ludum Dares, some areas of improvement are...
- `Transform2D`
  - Have a transform object that a rigidbody can watch/modify instead of having to copy back out.
  - That or allow transforms be parent/child relationships so that your game object can just child itself to the rigidbody.
- Optimization 
  - Better Memory Management
  - Scene Management
- Queries
  - `GetOverlappingObjects` - Ask the environment for all objects touching a shape with certain properties
  - `Raycast2D` - Be able to raycast against first hit, or all hit in the scene.
- Effectors
  - Objects in a scene that don't collider, but instead apply some other kind of force/behaviour on interaction
    - 1-Way Platforms
    - Gravity Wells
    - Conveyer Belts
- Constraints/Joints
  - Be able to connect rigidbodies together with a constraint/joint - usedful for ragdolling
    - `FixedJoint` - maintains rotation and dispacement between two objects
    - `SpringJoint` - maintains rotation, and behaves long a spring along the displacement vector
    - `HingeJoint` - Maintain distance, but allow rotation between two objects.
    - ...


