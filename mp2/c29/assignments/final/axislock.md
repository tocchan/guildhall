Axis Locking
======

Another straight forward feature.  

Locking an axis prevents an object from moving in that direction.  And like most 
things with our system, you can get fairly complicated with the implementation or keep 
it simple.

## Simple Linear
For `X` and `Y` axis, this is as simple updating your `Rigidbody2D::Move` method to zero out the locked axis.  If both axis are locked you can treat the object as having infinite mass. 

This will mostly look correct and is a good starting point (and all I'm requiring for this point).

## Rotational
For **rotational locking**, it gets a little more complicated.  In this case, we want to treat the object as if it has infinite moment of inertia - causing the physics equation to go back to being almost purely linear.  

When applying impulses, you can do the same thing (this would cause torque generated from impulse to go to zero), though it may just be easier to zero out if you see the lock.

### Representing the Lock
To help make zero'ing out locked axis easier, recommend making the "lock" member just a `Vec3`, that defaults to `Vec3(1, 1, 1)` for having full freedom.  Set the axis you're locking to 0 on a lock, and you can just "multiply" by the lock in methods that apply to that axis to zero it out. 


## Complicated Linear (NOT REQUIRED)
The above is easy to implement, but does result in some odd looking behaviour (you seem to lose energy hitting from an angle with only one axis locked).

I'm not sure on the correct solution here unfortunetly, but if you have time and want to experiment, would love to hear what solutions people came up with on finals day.

Some ideas I want to try are...
- Breaking the collision along the X and Y axis, treating it as having infinite mass on locked axis. 
- Possibly using the direction of the impact combined with the direction of locking to determine a way to "scale" the mass.  So a collision perfectly perpindicular to the locked axis would be treated as infinite mass, and a collision parallel to it would behave perfectly normal.  How to combine the two is what I would like to experiment with...

I'm sure there are established solutions to this problem, but I have not yet solved it myself and usually like to try my hand at it before looking it up.  I'm biased in recommending this approach as I find it gives me a greater appreciation and understanding of other solutions when I do finally do my research.  
