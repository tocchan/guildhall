MP2.A04 Collision REsponse
======

## Overview
Add in basic collision response; 

**Turn-In Branch Name: `mp2/turnin/a04`**

**Due Date: Feb 20, 2020**

### Goal 
Some example code for how the system will be used in this assignment;

------

## Checklist

- [ ] Stop coloring objects red that are intersecting (it becomes a strobe light in this assignment otherwise)
- [ ] Make sure your game creates polygons as `STATIC` by default, and discs create as `DYNAMIC` by default
- [ ] Add support for `disc Collider2D::GetWorldBounds` for existing colliders
	- [ ] **Easier Option**: Implement this using `AABB2` bounding boxes
    - [ ] **Challenge Option**: Implement this using a bounding `Disc`
    - [ ] Update this bounds whenever you update the world shape of the object.
- [ ] Switch `Collider2D::Intersects` to be non-virtual, and instead use a matrix lookup
    - [ ] Use a *mid-phase* check to early out of a collision by comparing the bounds first.
    - [ ] If passes the bounds check, lookup and call the appropriate callback
- [ ] Add `Collider2D::GetManifold` that also uses a collision matrix or half matrix
    - [ ] Again do an early out using the bounds.
    - [ ] Implement a disc versus disc manifold
    - [ ] Implement a disc versus polygon manifold
    - **Note: We are not doing polygon -vs- polygon yet**
- [ ] `Collider2D` gets gets a `PhysicsMaterial`
    - [ ] `PhysicsMaterial` has a property for `restitution` (or `bounciness` if you prefer)
    - [ ] `Collider2D::GetBounceWith(Collider2D const* other) const` implemented to compute restitution between two objects
        - There are multiple methods for computing the restitution.  Product, Min, Max, Spherical, Lookup, etc...   
    - [ ] Allow user to adjust bounciness using `+` and `-` while the object is selected.
    - [ ] Set the alpha for the fill color to the counciness of the object.
- [ ] Add a `Collision2D` object that contains..
    - Two pointers to the `Collider2D` objects involved in the collision (`me` and `them`)
    - A `manifold2` struct containing...
      - `normal` describing the normal at the impact point
      - `penetration` How deeply interpenetrated are the two objects.
- [ ] `Physics2D` during a `SimulateStep` should now do the following
    - [ ] `DetectCollisions` to compute all collisions between all colliders.
    - [ ] `ResolveCollisions` to resolve all detected collisions
    - [ ] `ResolveCollision` to resolve a single collision
- [ ] `ResolveCollision` should...
    - [ ] `CorrectObjects`, pushing them out of their respective objects
        - [ ] Push should depend on the ratio of the masses
        - [ ] Static and Kinematic objects assume infinite mass vs dynamic objects
        - [ ] Kinematic vs Kinematic resolves based on masses
        - [ ] Static objects never move, and should fully push the other object if it is not static.
        - [ ] Two static objects do not correct at all.
    - [ ] Calculate **normal impulse** at point of collision. 
    - [ ] `Rigidbody2D::ApplyImpulseAt` to both objects (`impulse` to A, `-impulse` to B)
- [ ] `Rigidbody2D::ApplyImpulseAt( vec2 worldPos, vec2 impulse )` implemented
    - [ ] Impulse uses force to apply an instant change in velocity
        - `delta_velocity = impulse * inverse_mass`
    - [ ] Ignore `worldPos` for now, it is there for when we apply rotational forces

------

## Resources

### Update Loop

```cpp
Physics2D::SimulateStep( float dt ) 
	ApplyGlobalForcesToAllObjects();
	MoveAllObjects();
	DetectCollisions();
	ResolveCollisions();

Physics2D::ResolveCollisions
	foreach (Collision) 
		ResolveCollision(Collision); 

Physics2D::ResolveCollision
	CorrectObjectsInCollision();
	CalculateCollisionImpulse();
	ApplyImpulse( FirstObject, Impulse );
	ApplyImpulse( SecondObject, -Impulse ); 
```


### Notes
Note: I'm trying something new with this cohort where we detect all collisions first, 
and then resolve all collisions based.  In previous cohorts we would correct during collision detection.  If the results are poor, we'll switch back to this (will be a fairly minimal change).

- [Collision Matrix](./collisionmatrix.md)
- [Manifold Generation](./manifold.md)
- [Collision Resolution](./collision.md)
- [Impulse Forces](./impulses.md)


### Links
- [Elastic Collisions - Khan Academy](https://www.khanacademy.org/science/physics/linear-momentum/elastic-and-inelastic-collisions/a/what-are-elastic-and-inelastic-collisions):  Good explanation of the math involved (including Restitution). 
- [Elastic Collision - Wikipedia](https://en.wikipedia.org/wiki/Elastic_collision):  Simple version (no restitution)
- [Inelastic Collisions - Wikipedia](https://en.wikipedia.org/wiki/Inelastic_collision):  Equations we should use; 
- [KhanAcademy.com - Deriving the shortcut to solve elastic collision problems](https://www.khanacademy.org/science/physics/linear-momentum/elastic-and-inelastic-collisions/v/deriving-the-shortcut-to-solve-elastic-collision-problems) 

- [Three Points Defining a Circle](https://www.khanacademy.org/math/geometry-home/triangle-properties/perpendicular-bisectors/v/three-points-defining-a-circle) 
- [Line-Line Intersection](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection): How we can determine the circumcenter of three points