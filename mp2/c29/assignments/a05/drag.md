Drag Force
======

A drag force is the force applied by the medium the object is moving through (water/air/etc) against the motion of movement.  Calculating
this "for reals" can be fairly complicated as we need to figure out the cross section, aerodynamics, surfacae area, etc, of our object along the direction of movement against the medium.

We're not doing any of that though, and doing something much simpler and "gamey" that is easier for us to adjust and code, as well as being computationally cheap. 

```cpp
void Rigidbody2D::ApplyDragForce()
{
   vec2 velocity = GetVerletVelocity(); 
   vec2 dragForce = -velocity * m_drag; 
   AddForce( dragForce ); 
}
```

We will boil down all the complex calculations to a single value, `drag` that is the ratio of our speed we apply as force against our movement. 

This way, if we want something to come to a stop on its own, we can give it some drag and eventually it'll slow down.  How much drag and how much mass it has determines how quickly it can come to a stop.

So a feather (low mass) and a bowling ball (high mass) can have the same drag, but the feather will slow down quicker since the force would affect the velocity more.   

So thing of `drag` more as a number representing how aerodynamic the object is.  

