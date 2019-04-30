Rays (2D & 3D)
======

## Lines, Rays, and Segments (2D)

There are three concepts, all of which can be represented in data the same (or similar) ways.  

We will represent each in its parametric form, p(t) = p0 + dir * t; 

```cpp

struct line2 
{
	vec2 start; 
	vec2 direction; 
};

struct ray2
{
	vec2 start; 
	vec2 direction;
};

struct segment2
{
	vec2 start; 
	vec2 end; 

	// through - you could as easily represent this as 
	// vec2 start; 
	// vec2 direction; 
	// float end_time; 
};
```

Notice that all three have the same parametric representation.  The only difference is conceptual - When dealing with intersections, what results do we care about?

- Line: `(-INFINITY, INFINITY)`
- Ray: `(0, INFINITY)`
- Segment: `(0, length)`

So all interesection tests are going to be done for a line, and then we just clamp the results.  

That said, all intersection tests are going to be written to return an intersection time instead of an intersection point.  The point can be derived using the line equation, and the time is needed for clamping and sorting.  


### Line vs Disc
Do they intersect vs where.

If you just care *if* they intersect, it is enough to find the closest point, and compare the distance to the radius; 

#### Intersection Test
We just need to find the closest point on the line to the center of the sphere.  If you drew a line from the center of the sphere to nearest point on line, you can intuitively tell it should be perpindicular to the line (moving in either direction from there would increase the distance).

So this is as simple as projecting the vector (disc.center - line.start) onto the line vector.  

#### Intersection Time(s)
This can be approached in two ways.  One is geometric using the above, that once you have closest point, you can than solve delta along the ray using Pythagorean 's Theorem (a^2 + b^2 = c^2)

Second is solving it purely algebraically.  Given the equation of a circle and the equation of the line - at which points is the line equal to the outside of the sphere.


### Line vs Plane
Again, we'll take the notation for a plane `Dot(normal, p) = distance`, and we just need to solve for when the ray solves this equation

Another way to think about it, is how long would it take us to reach the plane (what is our speed along the normal).


### Line vs Line
This is the first problem we're solving that is going to be very different in 2D vs 3D.  

The 2D version is much simpler.  If we have two lines, and they intersect, we can just solve in terms of what times cause the x's and the y's to be equal.  This will give us two equations, and two unknowns, which can be solved using simple linear Algebra.  

You can see if there is no solution, the lines must be parallel; 


### Line vs Capsule
If we have line vs line, we can think of the capsule as being a line segment.  We can solve this using the Line vs Line equation, clamping the result for the second line within the segment area. 

This is the closest point to the line, and we then do a radius check. 


### Ray vs Whatever
Now that we have solved everything in terms of lines.  The only change to doing a ray intersection is to clamp results in `(0, INFINITY)`.

In the case of planes, we can just fail if we get a negative results.

In terms of radius checks, we clamp the times before evaluating distances. 


## Moving to 3D
The only equation above that is fundamentally different in 3D is *Line vs Line*.  That is because we are not guaranteed that there is even an intersection, so the problem now becomes find the two closest points between two lines. 


