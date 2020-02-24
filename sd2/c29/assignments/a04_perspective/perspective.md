## Perspective Camera

This works off the knowledge...

1. ...that there is an implicit `w` divide in the shader.

2. ...we're working with perspective points, which are are all points that satisfy `y / z = y' / z'` for some given `y'` and `z'`.

3. ...there is *a distance* away from the camera in which movement is one-to-one from world space to clip space.  That is, if I move one unit up, the user would actually see me move one unit up on the screen (1 unit being half the screen in clip space)


```cpp
// float fov is the field of view you want
// float farZ and nearZ are the depth range you want to see
// -> do **not** span zero here.

// Goal is to...
// - setup a default "depth" where (1, 1) == (1, 1) after projection (1 / tan(fov * .5f))
// - map z to w, so the z divide happens (we go along negative, so we want a negative w-divide)
// - map nearZ to 0, farZ to -farZ, since a -Z divide will happen
//   and this will result in mapping nearZ to 0, and farZ to 1 in NDC space 
//   -> ((z - nz) / (fz - nz)) * fz + 0
//   -> fz / (fz - nz) * z      + (-fz * nz) / (fz - nz)

float height = 1.0f / tan(fov * .5f); // how far away are we for the perspective point to be "one up" from our forward line. 
float zrange = farZ - nearZ; 
float q = 1.0f / zrange; 

mat44 proj( 
   height / aspect,          0,           0,         0,
                 0,     height,           0,         0, 
                 0,          0,     -fz * q,        -1,
                 0,          0, nz * fz * q,         0 
);       
```

### Dealing with Aspect
Since we're mapping to -1 to 1 on width and height - the equation is already mapping 
corretly to height, but our width will be stretch, so we need "bring it in" by the aspect, hence the apsect divide in the (0, 0) element. 

