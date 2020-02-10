## Clip Space & Normalized Device Coordinates (NDC)

GPUs are fast at rendering, but only render objects that are within the defined clipping 
bounds.  In D3D, this space spans `(-1, -1, 0)` to `(1, 1, 1)`. 

Positions passed to the raster stage are passed in *Homogeneous Coordinates*, aka *Projective Coordinates* `clipPos = (x, y, z, w)` which will be converted to their 
representitive point automatically by doing...

`ndcPos = (clipPos.xyz) / clipPos.w;` 

We will refer to this as the **Normalized Device Coodindate**, or **NDC** for short.  Once
we get to this space, the GPU automacially clips everything outside the clipping bounds.

The `x` and `y` coordinates of the `ndcPos` will then be put at the position 
on the output target (texture).  Think of this as a `uv` coordinate. 

The `z` is used for depth testing, which we'll discuss later; 

So our job is to convert a position in the world as we think of it, to a `ndcPos`.

------

## `OrthoMin` and `OrthoMax`

Let's start passing our `OrthoMin` and our `OrthoMax` up to the *Vertex Shader*.  

```cpp
struct camera_ubo_data_t
{
	vec2 orthoMin; 
	vec2 orthoMax; 
};
```

1. Give you camera a *Uniform Buffer* to store this
2. Update your `orthoMin` and `orthoMax` when you `BeginCamera`
3. Update your camera's UBO
4. Bind the camera's UBO 

Once there, we will use them to convert the passed in coordinate to a valid `clipPos` for how it should show up on screen.  We can do this using a tool from MP1 called `RangeMap`. 

We're going to take our `worldPos`, and `RangeMap` it to our clip space.  Saying this another way;

```
clipPos.x = RangeMap( worldPos.x, orthoMin.x, orthoMax.x, -1, 1 );   
clipPos.y = RangeMap( worldPos.y, orthoMin.y, orthoMax.y, -1, 1 );
clipPos.z = 0; // not using yet
clipPos.w = 1; // we are outputting the representitive point exactly
```

We will need to define `RangeMap` in the shader.

Pass this out to the rasterizer and we should see **orthographic projection** working again. 

------

## Orthographic Projection as a Matrix

- What does a matrix represent?
- How can we fit what we're doing to that?
- Update the Camera's UBO to use a matrix representation?

```hlsl 
float4 worldPos = float4( input.position, 1 ); 
float4 clipPos = mul( PROJECTION_MATRIX, worldPos ); 

v2f.position = clipPos; 
```