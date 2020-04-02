void Physics2D::RunStep( float dt ) 
{
   // current approach
   MoveAllObjects(); 
   DetectCollisions();
   PushOutCollisions();
   ApplyImpulses();
}

// this is what I called a Manifold
class Contact2D
{
   vec2 position;
   vec2 normal;
   float penetration; 

   // sequential keeps some information here... be back in a second...
   float accumulated_jn = 0.0f; 
   float accumulated_jt = 0.0f; 
}

class Collisin2D
{
   Collider2D* me; 
   Collider2D* them;
   std::vector<Contact2D> contacts; 
}

// Sequential Impulse Approach
void Physics2D::RunStep( float dt )
{
   MoveAllObjects(); // PreStep()

   DetectAllCollisions();  // all contacts are stored in a collision

   // We do not push out
   for (uint seqStep = 0; seqStep <= NUM_SEQUENTIAL_STEPS; ++seqStep) {
      foreach (collision this frame) {
         UpdateImpulse(collision); 
      }
   }

   // if no bounce, no more work...
   // if there bounce, we need to add MORE impulse in
   foreach (collision this frame) {
      foreach (contact in collision) {
         ApproachApplyImpulse( bounce * contact.get_accumpulated_impulse() ); 
      }
   }
}

// (1 + e) * dv * some_factor
// (dv * some_factor) + (e * some_factor)

void UpdateImpulse( Collision2D* col ) 
{
   foreach (contact) {
      ResolveContact( col, contact )
   }
}

void ResolveContact( col, contact ) 
{
   // take out bounce in this
   // no longer clamp this to 0.0f... let it go negative.
   float jn = CalculateNormalImpulse( contact );

   // delta normal impulse
   float jn0 = contact->accumulated_jn; 
   contact.accumulated_jn = Max( 0.0f, jn0 + jn ); 
   float delta_jn = contact.accumlulated_jn - jn0; 

   ApplyImpulse( contact_point, delta_jn * contact.normal ); 

   // then do tangent...
   // you clamp the tangental impulse the total accumplated (contact.accumulated_jn)
   // then compute delta step (new step)
   // ...

}