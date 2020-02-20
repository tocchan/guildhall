Collision Resoultion
======



- First, we go through all our colliders and detect collisions (though ignore collision between static objects)
- Keep track of all our collisions `Collision2D`, into a list
- Walk the collision list at the end of the frame, and resolve each collision in order
- First, have objects push out of each other to prevent penetration
  - Continuous Collision (done much later) would rewind time to the point of collision
  - Discrete COllision (what we're doing now) just pushes out along the manifold normal
  - Push each object based on its type and mass as compared to the other object
- Next, if the collision involves a dynamic object...
  - Calculate a normal impulse at point of contact
  - Apply said impulse to all dynamic objects in the collision (1 or both)
    - Impulse is applied as normal to `me`
    - Impulse is negatved and applied to `them` (ie, they have an equal, but opposite reaction)
- ApplyImpulseAt will just immediately apply a change in velocity based on the objects mass
  - Caching off `inverse_mass` can speed this up a little.  A multiply is about 100-200x faster than a divide.

```cpp
struct Collision2D // PoD type - Plane Old Data
{
   Collider2D* me;
   Collider2D* them; 
   manifold2 manifold; 
};

class Physics2D
{
   // this is finding all manifolds and putting them in m_frameCollisions
   void DetectCollisions() 
   {
      /* ... */

      // Do not detect collisions between static objects
   }

   void ResolveCollisions()
   {
      foreach (collision) {
         ResolveCollision(collision); 
      }
   }

   void ResolveCollision( Collision2D const& col )
   {
      // Correcting - stop overlapping

      float myMass = col.me->GetMass();
      float theirMass = col.them->GetMass();
      float pushMe = theirMass / (myMass + theirMass);
      float pushThem = 1.0f - pushMe; 

      // Dynamic vs Dynamic (push each other)
      // Kinematic vs Kinematic (push each other)
      // STATICs don't move
      // Dynamics vs (Kinematic || Static) -> Only push dyanmic 100%
      // Kinematic vs Statis -> Only push kinematic 100%

      // Dynamic hits a Kinematic (Only Dynamic should push)
      col->me.Move( pushMe * col.GetNormal() * col.GetPenetration() ); 
      col->them.Move( -pushThem * col.GetNormal() * col.GetPenetration() ); 



      // Appply Impulses
   }

   std::vector<Collision2D> m_frameCollisions; 
};
