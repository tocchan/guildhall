
Collision Matrices
======

Below is an example of how a half-matrix could be implemented.  Just using a 2D array (or 1-day array where we index outselves).

When doing `x` vs `y` checks, we look up the callback to use using `x`, `y` to give us our index. 

A half matrix only fills out the diaganol, and one side of the diagnol.

```cpp

typedef bool (*collision_check_cb)( Collider2D const*, Collider2D const* ); 

static bool DiscVDiscCollisionCheck( Collider2D const* col0, Collider2D* const* col1 )
{
   // this function is only called if the types tell me these casts are safe - so no need to a dynamic cast or type checks here.
   DiscCollider2D const* disc0 = (DiscCollider2D const*)col0; 
   DiscCollider2D const* disc1 = (DiscCollider2D const*)col1; 

   // do the collision check between the two discs...
   // ...

   return result; 
}

// make a callback for the remaining 

// a "matrix" lookup is just a 2D array
static collision_check_cb gCollisionChecks[NUM_COLLIDER_TYPES * NUM_COLLIDER_TYPES] = {
   /*             disc,                         polygon, */
   /*    disc */  DiscVDiscCollisionCheck,      nullptr, 
   /* polygon */  DiscVPolygonCollisionCheck,   PolygonVPolygonCollisionCheck
};

```

The collision function now looks something like; 

```cpp
bool Collider2D::Intersects( Collider2D const* other ) const 
{
   eColliderType myType = get_type();
   eColliderType otherType = other->get_type();

   if (myType <= otherType) {
      int idx = otherType * NUM_COLLIDER_TYPES + myType; 
      collision_check_cb check = gCollisionChecks[idx]; 
      return check( this, other ); 
   } else {
      // flip the types when looking into the index.
      int idx = myType * NUM_COLLIDER_TYPES + otherType; 
      collision_check_cb check = gCollisionChecks[idx]; 
      return check( other, this ); 
   }
}
```


If the flipping code is odd - you could also fully fill out the matrix with functions that just call their mirror function in reverse... for example;

```cpp
static bool DiscVPolygonCollisionCheck( Collider2D const* col0, Collider2D* const* col1 )
{
   // assume implemented...
   return result; 
}

static bool PolygonVDiscCollisionCheck( Collider2D const* col0, Collider2D* const* col1 )
{
   return DiscVPolygonCollisionCheck( col1, col0 ); // call the other guy, but flip the parameters
}
```

------

## Manifold Matrix
Very similar, but the callback looks different (must have some way to return a manifold and let you know if it succeeded). 

The other consideration is that if you have to flip the matrix, or reverse the call, make sure you also invert the result (flip the normal); 