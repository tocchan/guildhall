## Friction 
Friction *in our simulation* is a way to limit the *tangental force* being applied, and is to simulate the loss of energy due to contact. 0 friction will lose no energy, with 1 meaning it'll get the full tangental force.  More than one will compound it (add energy to the system)

The `friction` value is used to calculate a `frictional coefficient` to scale the resultant tangent impulse by.  
- `0` would never apply the tangental force, so something like really slick ice.
- `1` would apply the full force, so sliding across the surface is almost as bad as hitting the surface directly (think gears sliding next to each other)
- `>1` would result in adding energy to the system.

The **Tangental Impulse** is calculated identical to the **Normal Impulse**, except you use a tangent to the point of contact.  In 2D, we can just rotate the normal. 

There is more more limit on tangent impulse though - in that the impulse due to friction can not be greater then the impulse due to impact.  Stated another way - objects don't have friction unless they're pressing into each other to generate it.  A plane hovering just above a field does not generate friction with the ground.


```cpp
   // Coulumb's Law
   // https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table--gamedev-7756
   float friction = myCollider->GetFrictionWith( theirCollider ); 
   if (AbsFloat(tangent_impulse) > friction * normal_impulse) {
      tangent_impulse = SignFloat(tangent_impulse) * normal_impulse * friction; 
   }
```

```cpp
float SignFloat( float val )
{
   return (val >= 0.0f) ? 1.0f : -1.0f;  
}
```

- Getting the frictional coefficient between two objects is not an exact science, and can really depend on how you want to think of it.  The tutorial listed with this assignment uses a circular combination, but that means that if either object has friction, there is going to be a frictional component to the collision.  If you want your game to have "ice" that always causes objects to slide on it, you may want to do a product combination similar to restitution.

And like restitution, different engines use different functions for `Collider2D::GetFrictionWith(Collider2D const* other) const;`, or may even let you pick as part of the physics material.  And again,  most accurate is a lookup table for pairs of materials, but nice to have a nice default callback for when people don't have the time for that.  


- **Static Friction** refers to an object at rest being harder to move than an object in motion.  Some physics expose this concept, and some do not.  You can implement this as exposing friction as a function of velocity, or just having two sets of friction (`friction` and `static friction`)