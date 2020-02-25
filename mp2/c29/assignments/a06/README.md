MP2.A05 Friction
======

## Overview
Add in friction; 

**Turn-In Branch Name: `mp2/turnin/a05`**

**Due Date: Mar 5, 2020**

### Goal 
Some example code for how the system will be used in this assignment;

------

## Checklist
- [ ] Allow user to adjust mass when selected [can not be 0, so clamp at 0.001 on the low end.  No high end limitation]
    - [ ] Suggested keys are `[` and `]`
- [ ] Add `friction` to the physics material, defaulted to 0
- [ ] Allow user to adjust friction when selected [0 to 1]
    - [ ] Suggested keys are `<` and `>'`
- [ ] Add `drag` to the `Rigidbody2D`, defaulted to 0
- [ ] Allow user to adjust object drag when selected [minimum of 0]
    - [ ] Suggested keys are `:` and `\'` 
- [ ] Switch to use a fixed time step
    - [ ] Add a `Clock` class
        - [ ] Engine should have a `Master` clock that is creaetd, that all clocks use as their parent by default
        - [ ] Be sure to update your clock system in `App::BeginFrame`
    - [ ] Give the physics engine its own clock (either basded off a game clock or master)
    - [ ] Add a `Timer` class
    - [ ] Add `Physics2D::GetFixedDeltaTime()` && `Physics2D::SetFixedDeltaTime( float frameTimeSeconds )`
        - [ ] This should default to `120hz` or `1.0f / 120.0f` seconds per frame. 
    - [ ] Use timer to break up `Physics2D::Update` into equal steps
    - [ ] Add a console command `set_physics_update hz=NUMBER` to be able to change this at runtime.
- [ ] Allow user to adjust time scale of the physics clock
    - [ ] `P`: Pause & Resume
    - [ ] `8`: Half the time scale
    - [ ] `9`: Double the time scale
    - [ ] `0`: Reset time scale to 1 and resume
    - [ ] Print current time scale and pause state to screen somewhere
- [ ] Implement `Rigidbody2D::GetVerletVelocity` to return an objects actual velocity this frame. 
    - [ ] Save off an objects position at the start of a fixed physics step.
    - [ ] Calculate verlet by taking `(currentPosition - frameStartPosition) / fixedDeltaTimeSeconds`
- [ ] Use `VerletVelocity` in impulse calculations. 
    - Suggest using a function `Rigidbody2D::GetImpactVelocityAtPoint( vec2 point )` where point is unused atm, which 
      in turn calls `GetVerletVelocity()`, as we'll be doing more with this when rotational velocity comes online. 
- [ ] Have a tooltip near the cursor when hovering over an object to tell information about it
    - [ ] Simulation Mode
    - [ ] Mass
    - [ ] Current Velocity
    - [ ] Current Verlet Velocity
    - [ ] Coefficient of Restitution (`bounce`)
    - [ ] Coefficient of Friction (`friction`)
    - [ ] Drag value
- [ ] Implement tangental impulse using friction 
    - [ ] Follows similar rules to normal impulse when it comes to `DYNAMIC` vs `STATIC` vs `KINEMATIC` relative masses and when you apply it
    - [ ] Applied only to dynamic objects
- [ ] Apply drag force to all dynamic objects

------

## Resources

### Notes


### Links
- Deriving Moments of Inertia (Ring/Rod/Disc/Cylindar): https://www.youtube.com/watch?v=9cuKlt15yCo
- Parallel Axis Theorem: https://en.wikipedia.org/wiki/Parallel_axis_theorem
- Moments for a Triangle: https://www.efunda.com/math/areas/triangle.cfm
- Deriving Moment of Inertia of a Triangle: https://www.youtube.com/watch?v=2SkQy-GVTRk  