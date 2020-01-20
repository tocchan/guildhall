Resolving Momentum
======

## Overview
So now that we can detect when something collides and correct out, the next step is to determine how the energy of the system resolves

## Helpful Link(s)
- [Elastic Collisions - Khan Academy](https://www.khanacademy.org/science/physics/linear-momentum/elastic-and-inelastic-collisions/a/what-are-elastic-and-inelastic-collisions):  Good explanation of the math involved (including Restitution). 
- [Elastic Collision - Wikipedia](https://en.wikipedia.org/wiki/Elastic_collision):  Simple version (no restitution)
- [Inelastic Collisions - Wikipedia](https://en.wikipedia.org/wiki/Inelastic_collision):  Equations we should use; 

## Restitution (think: Bounciness)
The **Coeffecient of Restitution** is defined as the ratio between the final velocity and the initial velocity after a collision; 

For a full 100% elasctic (no energy lost) collision, the object would have restitution of 1.0.  If all energy is loss (it hits and object and stops), then it would have a restitution of 0.0.

If you want something to give MORE energy for game purposes, you could use restitutions greater than 1.0, but would avoid ever using a restitution below 0 (Question: what would this do?).

When two objects collide, we'll be determining the restitituion of the collision the product of the restitutions of the two parties; ie **R0 * R1**.  *This is not **the** solution, just **a** solution.*.  Different physics systems may simplifiy this as the min, max, or even custom callback based on physics material.  But the product is a good starting point and you can play with the function.

The idea behind using product is that something that has no bounce (wet paper-towels) collides with something with a lot of bounce (rubber), the 0 restituion surface overrides the other side (bounciball can't bounce off web paper-tower clump).  The drawback may be that two relatively low restituion surfaces may act as if they have near 0 response.

How this will fit into our system is here; 

```cpp
struct physics_material_t 
{
  float restitution   = 1.0f;    // think: dampens perpindicular forces; 
  // float friction   = 0.0f;    // think: affects tangental forces during collision
}; 

class Rigidbody2D 
{
  // ...

  public:
    physics_material_t m_material; 
};
```


## Dynamics vs Statics
The first easy case for us to solve is a dynamic vs static.  We are solving for a collision along a normal; 

First - we're removing friction from the equation, so when resolving a collision, we're solving along the normal of impact and ignoring perpindicular momentum (that way, a ball will move off screen, not immediately stop).  

1. Compute the velocity along the normal of impact the object: **Vn** or `float vn` (we're solving the problem in 1D, in what we'll call *Normal Space*)
   - If you are already moving along the normal (**Vn** > 0), ignore this collision, you're already trying to move out of the object.
   - Also compute tangental velocity:  **Vt** or `vec2 vt`;
   - I keep the tangental velocity as a `vec2` for ease, and algorithm translates better to 3D, but you could also store it as `float` by computing the tangent on the surface first.
2. Compute the final velocity along the normal, **Vfn**, by reversing **Vn** scaled by the collision's restitution. 
   - `float vfn = -vn * (restitution0 * restitution1)`
3. Compute final velocity by keeping the tangental velocity and adding in the bounced velocity; 

**C4TODO:  Draw some pictures**


## Dynamics vs Dynamics
This is similar, but now need to figure out how the momentum transfers between the objects.  

So steps are similar to above for calculating **Vn** and **Vf**, but this time we calculate it for both sides (again, in 1D normal space), so we have **Vn0, Vn1, Vt0, & Vt1**.

Using the elastic collision equations (found [here](https://en.wikipedia.org/wiki/Elastic_collision)), we then solve for **Vfn0** and **Vfn1**.  Scale
both results by the collisions restitution, and then compute the final resulting velocity of each object using the same combination of terms as static (our derived velocity along the impact plane, and the unaffected tangental velocity)

*Note - if you're curious on how to drive these equations - this is a great video;*
- [KhanAcademy.com - Deriving the shortcut to solve elastic collision problems](https://www.khanacademy.org/science/physics/linear-momentum/elastic-and-inelastic-collisions/v/deriving-the-shortcut-to-solve-elastic-collision-problems) 


## Additional Notes
**TEST**! Even with the math right, due to having an imperfect simulation on discrete time steps, and corrections happening, we can get into odd situations; 

For example, if two dynamic objects are colliding in the air, traveling in the same direction at the same speed, they'll hit, and slow themselves down by restitution - hence cancelling the affect of gravity (causing your objects to float).  What should happen in this situation to give more believable behaviour that doesn't break the normal working cases?






