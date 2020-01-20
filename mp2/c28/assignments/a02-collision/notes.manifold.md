Collision Response
======

## Overview
Suggested tasks and order to do them; 

- Move Rigidbodies/Colliders to buckets seperated by simulation mode. 
- Update a physics step;
  - Update all dynamics
  - Resolve collisions
    - Dynamics vs Statics (*Optional: Squirrel recommended for better results*)
    - Dynamics vs Dynamics 
    - Dynamics vs Statics (*Leaves objects not stuck in the floor*)
- Implement Collision Resolution
  - Implement manifold generation for existing primitives
  - Implement correction logic
    - Dynamic vs Static (full push out of static)
    - Dynamic vs Dynamic (seperation is based on your mass)
    - **WEEK1 Checkpoint**
  - Implement elastic collision response
    - Dynamic vs Static (reflect velocity along normal of impact)
    - Dynamic vs Dynamic (correctly transfer momentum)
  - Test thoroughly
  - **Final Checkin**

## Buckets

```cpp
class RigidbodyBudcket
{
  public:
    std::vector<Rigidbody2D*> m_rigidbodies; 
    std::vector<Collider2D*> m_colliders; 
}
```

I would store colliders in the bucket as well - this allows us to have *trigger* colliders not associated with any rigidbody. 


## Manifold Generation

The manifold will represent the area of collision.  Will will do this by generating a normal and a distance, and how this is treated is, for
two objects *A* and *B*, the manifold generated is the direction and istance *A* has to move to not collide with *B*. 

```cpp
struct manifold2 // contact2?
{
   vec2 normal;
   float penetration; 
}; 
```

*Note: This is not final - when we add torque, we'll be adding some extra information to figure out rotational impact.*

So now the `Collision2D` object becomes;

```cpp
struct Collision2D 
{
   Collider2D *me; 
   Collider2D *them; 
   manifold2 manifold; // may be referred to as a "contact"
};
```

For this assignment, you'll need to write three manifold generation functions; 

```cpp
bool GetManifold( manifold2 *out, aabb2 const &obj0, aabb2 const &obj1 ); 
bool GetManifold( manifold2 *out, aabb2 const &obj0, disc const &obj1 ); 
bool GetManifold( manifold2 *out, disc const &obj0, disc const &obj1 ); 
```


## Correction
So our update will now look something similar to this; 

```cpp
void PhysicsSystem::run_step( float dt ) 
{
   MoveAllDynamicObjects();

   ResolveDynamicVsStaticCollisions( true );  // collison & resolution
   ResolveDynamicVsDynamicCollisions( true ); // collision & resolution
   ResolveDynamicVsStaticCollisions( false );  // JUST collision
                                        // suggest by Squirrel so that dynamic objects are not seen penetrating the world

   // Note: there is not static-vs-static.  They ignore each other; 
}; 
```

### Dynamic -vs- Static
For now, dynamic for static correction is easy.  ObjectA should always be dynamic, and ObjectB should always be static,
so you can just move the object as directed by the manifold; 

```cpp
void ResolveDynamicsVsStaticCollisions( bool resolveCollision )
{
   foreach (dynamicCollider) {
      foreach (staticCollider) {
         Collision2D collision; 
         if (GetCollision( &collision, dynamicCollider, staticCollider )) {
            Rigidbody2D *dynamicObject = dynamicCollider->GetRigidbody(); 

            // move the object by the manifolds correct vector; 
            // we move the dynamic object fully, and do not touch the static; 
            dynamicObject->MoveBy( collision->manifold.normal * collision->manifold.penetration ); 

            // TODO: Handle collision response; 
            if (resolveCollision) {
               // TODO: do collision response here
            }
         }
      }
   }
}

```


### Dynamic -vs- Dynamic
Dynamic -vs- Dynamic is slightly different.  Now, instead of moving out completely, we move
each out based on their mass vs the mass of the entire system; 

For example, if 3kg object, and a 1kg object are intersecting, then the total mass
involved is 4kg.  The 1kg object moves by 3/4 (75% of the way out), and the 3kg object moves
by 1/4 (25% the other way). 

```cpp
// if col0 and col1 are the colliders involved in a collision; 
Rigidbody2D *rb0 = col0->GetRigidbody();
Rigidbody2D *rb1 = col1->GetRigidbody(); 
float m0 = rb0->GetMass(); 
float m1 = rb1->GetMass(); 
float totalMass = m0 + m1; 

// correct amount
float correct0 = m1 / totalMass;   // move myself along the correction normal
float correct1 = m0 / totalMass;  // move opposite along the normal

rb0->MoveBy( manifold.normal * penetration * correct0 ); 
rb1->MoveBy( -manifold.normal * penetration * correct1 ); // notice the negative
```

*Note:  This is actually doing the same thing as the code from dynamics-vs-statics, where statics are assumed to have infinite mass.*





