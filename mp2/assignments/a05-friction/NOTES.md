## Friction 
Friction *in our simulation* is a way to limit the *tangental force* being applied, and is to simulate the loss of energy due to contact. 0 friction will lose no energy, with 1 meaning it'll get the full tangental force.  More than one will compound it (like velcro)

The `friction` value is used to calculate a `frictional coefficient` to scale the resultant tangent force by.  

- The **Tangental Impulse** is calculated identical to the **Normal Impulse**, except you use a tangent to put of contact.  In 2D, we can just rotate the normal. 
  - A tangent impulse should not be greater than the normal impulse (ie, if the object isn't pushing into the surface, than it should not be "grinding" together).  

```cpp
   // Coulumb's Law
   if (Abs(tangent_impulse) > friction * normal_impulse) {
      tangent_impulse = Sign(tangent_impulse) * normal_impulse * friction; 
   } else {
      tangent_impulse *= friction; 
   }
```

- Getting the frictional coefficient between two objects is not an exact science, and can really depend on how you want to think of it.  The tutorial listed with this assignment uses a circular combination, but that means that if either object has friction, there is going to be a frictional component to the collision.  If you want your game to have "ice" that always causes objects to slide on it, you may want to do a product combination similar to restitution. 
  - Suggest playing with a 3D graphing calculator online (for example: https://www.geogebra.org/3d?lang=en, or just https://www.google.com), and seeing what curves you like.
- **Static Friction** refers to an object at rest being harder to move than an object in motion.  Some physics expose this concept - up to you if you want to.  I did not...

- 
## Constraints 
Some artifacts I noticed implementing constraints...

- Correction is no longer going to guarantee to bring you 100% out of the object if you constrain movement to one dimension of freedom without rewriting collision code.  This is fine - it will eventually correct over multiple frames; 
   - This did result in my having some rotational introducted when spheres collided.  I'm fine with this;  
   - Impulses are calculated correctly, which actually feels "off" when there are consrtaints.  (ie, a ball not bouncing off a constrained object, but instead just stopping since it *should have* transferred momemntum).  
      - You can solve the impulse equation for each axis seperately, treating constrained axis as having infinite mass (like static objects).  This felt better to me - but is not required for the assignment. 
      - If you have an angular constraint, you can treat the object as having an infinite moment of inertia (causing all momentum to come from the linear hit part - notice that in the impulse equation infinite inertia would cause those components to go to zero). 

## Drag
Drag can be implemented using forces or just applying a per frame dampen.

- Drag forces apply opposite current velocities...
   - A drag force acts opposite to current velocity (hence why it usually slows you down).  I would recommend applying this as a change directly as an acceleration instead of a force (small time steps and low mass objects can result in very high drag forces, that at a fixed timestep may result in erratic behaviour.  `accel += -velocity * GetLinearDrag()`
   - Similar for angular forces - I would just apply a negative angular acceleration based on current angular velocity;  `angular_accel += -angular_velocity * GetAngularDrag()`
- Drag as a dampening force would just have you dampen both angular velocity and linear velocity before applying the current frames forces.  Something as simple as `velocity = velocity * (GetLinearDrag() * fixedStep)`, and similarly for angular velocity.

- Since both methods are easy to implement, I encourage you to try both and see what you like the best; 

