MP2.A07 GJK
======

## Overview
Add in Polygon Intersection.  

**Turn-In Branch Name: `mp2/turnin/a07`**

**Due Date: Apr 02, 2020**

### Goal 

- [ ] Remove screen wrap and bottom of screen bounce.
    - [ ] Add some static world geometry (at least a floor) to catch falling objects.
    - [ ] Destroy objects that fall out of world bounds
        - [ ] Option A: Define a world bounds and draw it so users can see where objects should destroy at.
        - [ ] Option B: Destroy objects that leave screen either left/right/bottom, but NOT top. 
- [ ] Polygon-v-Polygon Intersection Check (using GJK)
- [ ] Polygon-v-Polygon Manifold (normal/penetration) - object push out correctly
- [ ] Polygon-v-Polygon Contacts - Objects generate one or two contact points.  
    - [ ] Manifold now returns a contact edge (two points) instead of just a single contact point.  For cases where only one contact exists, 
          return an edge with the start and end point being the same. 
- [ ] *Optional*: Apply Impulse using contact points
    - [ ] *Optional*: Calculate impulse using the center of the contact edge.
    - [ ] *Optional*: When applying impulse, use the point on the edge closest to each body's center of mass.
- [ ] *Optional*: To make friction/bounce more stable - calculate normal impulse first, apply it, and then generate tangent impulse, and apply it. 

------

## Resources

### Applying Impulse
With our current system, we can calculate impulse once, and apply it once.  This doesn't work well with a contact region
as we have now, but we can get something fairly close by trying to apply the impulse as close as possible to the center of mass
of each object.

When calculating the impulse, just use the center of your contact points.  Somehwat like taking an average of all contacts.

When applying an impulse to an object, apply it at the point along the contact edge closest to each center of mass.  Meaning
each object may apply it using a different point.  

This may add or remove energy from the system, but will result in something that looks passable, and will work for the project we'll be
making with it. 

If you're curious on a more robust approach, you can switch to a sequential impulse approach (Erin Catto - GDC 2006)

### Notes


### Links
- GJK - Detecting Intersections in 2D: https://blog.hamaluik.ca/posts/building-a-collision-engine-part-1-2d-gjk-collision-detection/
- GJK - Penetration Vectors: https://blog.hamaluik.ca/posts/building-a-collision-engine-part-2-2d-penetration-vectors/
- Sequential Impulse: https://box2d.org/files/ErinCatto_SequentialImpulses_GDC2006.pdf
- GJK-Intersection in 200 lines of C: https://github.com/kroitor/gjk.c