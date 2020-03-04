MP2.A05 Friction
======

## Overview
Add in rotation.  

**Turn-In Branch Name: `mp2/turnin/a06`**

**Due Date: Mar 12, 2020**

### Goal 

------

## Checklist
- [ ] Angular Update - Add members to `Rigidbody2D`
    - [ ] Add `float m_rotationInRadians`
    - [ ] Add `float m_angularVelocity`   
    - [ ] Add `float m_frameTorque`       
    - [ ] Add `float m_moment`
    - [ ] Add appropriate getter/setter methods
- [ ] `Rigidbody2D::CalculateMoment` to calculate moment based on attached collider and mass
    - [ ] `virtual float Collider2D::CalculateMoment( float mass )`
    - [ ] Implement `DiscCollider::CalculateMoment`
    - [ ] Implement `PolygonCollider::CalculateMoment`
    - [ ] When setting **mass** or **collider** on a rigidbody - recalculate your moment.
        - *Note: When setting mass, you can do less work by taking advantage of the ratio of new mass to old mass should match the ratios of the moments*. 
- [ ] Rotating a `Rigidbody2D` properly updates the **world shape** of the collider.
    - [ ] Add controls to rotate a selected object
        - Suggest **R** and **F** 
    - [ ] Add controls to update the rotational velocity of an object
        - Suggest **T**, **G**, and **V** to increase, decrease, and reset
- [ ] `Rigidbody2D` now do angular calculations during their update.
    - [ ] Use torque to compute an angular acceleration
    - [ ] Apply angular acceleration to update angular velocity
    - [ ] Apply angular velocity to update rotation
- [ ] Update `Rigidbody2D::GetImpactVelocity` to take into account rotational velocity.
- [ ] Update `ApplyImpulseAt` to apply impulse to torque based on positions. 
- [ ] Update impulse calculations to take into account rotational forces, see PDF in lnks
- [ ] Update tooltip to show...
    - [ ] Moment of Inertia
    - [ ] Current Rotation (degrees)
    - [ ] Current Angular Velocity


------

## Resources

### Notes
- I suggest using radians as your unit of rotation within the system.  You can have 
methods to take Degrees, such as `SetRotationDegrees( float deg )`, but when calculating
impact velocities, radians translate to linear space directly (since a radian is a relation to how are radius relates to distance traveled when rotating). 

- Calculating moment is easier to do in local space for each shape.  Once calculated, you can use **Parallel Axis Theorem** to  get the moment of the rigidbody based on the colliders local offset.  This is useful if want to use a different center of mass. 

- Calculating the moment of a polygon is the sum of the moments of its triangles, and then
translated to their corresponding position using the parallel axis theorem.




### Links
- http://chrishecker.com/images/e/e7/Gdmphys3.pdf : Link from previous article explaining the equations;
- Deriving Moments of Inertia (Ring/Rod/Disc/Cylindar): https://www.youtube.com/watch?v=9cuKlt15yCo
- Parallel Axis Theorem: https://en.wikipedia.org/wiki/Parallel_axis_theorem
- Moments for a Triangle: https://www.efunda.com/math/areas/triangle.cfm
- Deriving Moment of Inertia of a Triangle: https://www.youtube.com/watch?v=2SkQy-GVTRk  