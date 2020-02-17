## Perspective Camera

This works off the knowledge...

1. ...that there is an implicit `w` divide in the shader.

2. ...we're working with perspective points, which are are all points that satisfy `y / z = y' / z'` for some given `y'` and `z'`.

3. ...there is *a distance* away from the camera in which movement is one-to-one from world space to clip space.  That is, if I move one unit up, the user would actually see me move one unit up on the screen (1 unit being half the screen in clip space)








### Dealing with Aspect
Or another way to say it, do ou want FoV to be for your height or your width.  I do something different in my construction of 
a projection matrix in that I will pick FoV to be the minimum dimension to ensure that different aspect ratios never remove visible area, but instead always just add it.  


### Notes to Self
- The angle determines how far this representative plane is from us.  
- Construct matrix as we go; 

