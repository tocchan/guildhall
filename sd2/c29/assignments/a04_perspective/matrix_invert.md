## Model Matrix

A model matrix tells where an object is "in the world" - letting us know its heading, orientation, and position.

It can be computed from...
- `T` - Translation Part (where it is)
- `R` - Rotation Part (orientation)
- `S` - Scale Part (how large/small - 1 is default)

So to convert a local point to a world point by the model, it goes through the 
following transforms;

```cpp
Vec3 localPoint; 
Vec3 worldPoint = S * localPoint;
worldPoint = R * worldPoint;
worldPoint = T * worldPoint; 
return worldPoint;  
```
...or...
```
Vec3 worldPoint = T * R * S * localPoint;
```

Meaning 

```cpp
ModelMatrix = T * R * S; 
```

------

## Matrix Invert
A matrix `M` has an inverse `Mi` that is defined by the equation;

`M*Mi = Mi*M = I`

That is, the matrix that transform M back to the identity.  This can also be thought of as the reverse transform (If `M` is moving right, then `Mi` would move left the same amount).

Calculating an invert can be complex for a general matrix (see the last section), but in certain special cases it is much quicker; 

------

## Invert Translation
If we have a translation matrix `T(p)` where `p` is the position we want to move to, the inverse of that is just moving back `T(-p)`.

Can intuit by...

```cpp
vec4 pos = T(p) * vec4(0,0,0,1); // pos = (p.x, p.y, p.z, 1)
vec4 finalPos = T(-p) * pos;     // finalPos = (p.x - p.x, p.y - p.y, p.z - p.z, 1) = (0, 0, 0, 1)
```

## Invert Ortho Normal

A rotation matrix is **orthonormal**, meaning each basis vector has length one, and all basis vectors are orthogonal to each other. 

To invert this, we can just take the Transpose of that matrix.

A transpose flip the matrix along its diagonal - or switches all the rows with the corresponding columns.

```cpp
    | ix   jx   kx   tx |                    | ix   iy   iz   0 |
    | iy   jy   ky   ty |                    | jx   jy   jz   0 |
M = | iz   jz   kz   tz | ==> Transpose(M) = | kx   ky   kz   0 |
    |  0    0    0    1 |                    | tx   ty   tz   1 |
```

Thisc can be intuited by the fact that if `M` tells how to take a point relative to us and put it into the world, then `Mi` takes a point in the world and tells us what it is relative to us.  

If something is orthonormal, then `relative` to us is just projecting along the basis vectors, which is what the `Transpose(M)` matrix would end up doing.

### Translation?

Adding translation to rotation makes the matrix no longer orthonormal, since the translation column is usually not normal, and can't be orthogonal to the three other basis (unless we go to 4d).

So we can do an inversion of a `T * R` matrix by knowing that..

`Invert( T * R ) == Invert(R) * Invert(T)`

That is, if a transform is the composite of two other transforms, the inverse of it is the composite of the invert of the transforms in reverse. 

So...

```cpp
Invert(T(p) * R) = Invert(R)    * Invert(T(p))
                 = Transpose(R) * T(-p)
```

Which is what the followign function is accomplishing;

```cpp
mat44 InvertOrthonormal( mat44 const& mat )
{
   mat44 inverse = mat; 
   inverse.set_translation( 0.0f, 0.0f, 0.0f ); 
   inverse.transpose(); 

   vec3 translation = mat.get_translation(); 
   vec3 inverse_translation = inverse.TransformPosition( -translation ); 
   inverse.set_translation( inverse_translation ); 

   return inverse; 
}
```

## View Matrix
So now that we can invert a model matrix, we can now compute our view matrix pretty
quickly by...

1. Get the model of the camera.
2. Invert it (quickly using `InvertOrthoNormal`)
3. This is our view!

```cpp
void Camera::GetViewMatrix()
{
   // vec3 m_position; 
   // vec3 m_eulerRotation;

   // local to world
   mat44 cameraModel = mat44::FromScaleRotationTranslation( 
      vec3(1.0f), 
      m_eulerRotation, 
      m_position 
   ); 

   // world to local
   m_view = InverseOrthonormal( cameraModel );
}

mat44::FromRotationTranslation( vec3 euler, vec3 position )
{
   mat44 transform = mat44::FromPitchRollYaw( euler ); // rotateUp * rotateForward * rotateRight (v multiplies on the right)
   transform.SetTranslation( position ); 
   return transform; 
}
```



## General Purpose Invert
```cpp
//------------------------------------------------------------------------
// Lifted from GLU
// assuming memory is going (ix, jx, kx, tx, iy, jy, ...)
mat44 Invert( mat44 const& mat )
{
   double inv[16];
   double det;
   double m[16];
   uint i;

   for (i = 0; i < 16; ++i) {
      m[i] = (double) mat.data[i];
   }

   inv[0] = m[5]  * m[10] * m[15] - 
            m[5]  * m[11] * m[14] - 
            m[9]  * m[6]  * m[15] + 
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] - 
            m[13] * m[7]  * m[10];

   inv[4] = -m[4]  * m[10] * m[15] + 
             m[4]  * m[11] * m[14] + 
             m[8]  * m[6]  * m[15] - 
             m[8]  * m[7]  * m[14] - 
             m[12] * m[6]  * m[11] + 
             m[12] * m[7]  * m[10];

   inv[8] = m[4]  * m[9]  * m[15] - 
            m[4]  * m[11] * m[13] - 
            m[8]  * m[5]  * m[15] + 
            m[8]  * m[7]  * m[13] + 
            m[12] * m[5]  * m[11] - 
            m[12] * m[7]  * m[9];

   inv[12] = -m[4]  * m[9]  * m[14] + 
              m[4]  * m[10] * m[13] +
              m[8]  * m[5]  * m[14] - 
              m[8]  * m[6]  * m[13] - 
              m[12] * m[5]  * m[10]  + 
              m[12] * m[6]  * m[9];

   inv[1] = -m[1]  * m[10] * m[15] + 
             m[1]  * m[11] * m[14] + 
             m[9]  * m[2]  * m[15] - 
             m[9]  * m[3]  * m[14] - 
             m[13] * m[2]  * m[11] + 
             m[13] * m[3]  * m[10];

   inv[5] = m[0]  * m[10] * m[15] - 
            m[0]  * m[11] * m[14] - 
            m[8]  * m[2]  * m[15] + 
            m[8]  * m[3]  * m[14] + 
            m[12] * m[2]  * m[11] - 
            m[12] * m[3]  * m[10];

   inv[9] = -m[0]  * m[9]  * m[15] + 
             m[0]  * m[11] * m[13] + 
             m[8]  * m[1]  * m[15] - 
             m[8]  * m[3]  * m[13] - 
             m[12] * m[1]  * m[11] + 
             m[12] * m[3]  * m[9];

   inv[13] = m[0]  * m[9]  * m[14] - 
             m[0]  * m[10] * m[13] - 
             m[8]  * m[1]  * m[14] + 
             m[8]  * m[2]  * m[13] + 
             m[12] * m[1]  * m[10] - 
             m[12] * m[2]  * m[9];

   inv[2] = m[1]  * m[6] * m[15] - 
            m[1]  * m[7] * m[14] - 
            m[5]  * m[2] * m[15] + 
            m[5]  * m[3] * m[14] + 
            m[13] * m[2] * m[7] - 
            m[13] * m[3] * m[6];

   inv[6] = -m[0]  * m[6] * m[15] + 
             m[0]  * m[7] * m[14] + 
             m[4]  * m[2] * m[15] - 
             m[4]  * m[3] * m[14] - 
             m[12] * m[2] * m[7] + 
             m[12] * m[3] * m[6];

   inv[10] = m[0]  * m[5] * m[15] - 
             m[0]  * m[7] * m[13] - 
             m[4]  * m[1] * m[15] + 
             m[4]  * m[3] * m[13] + 
             m[12] * m[1] * m[7] - 
             m[12] * m[3] * m[5];

   inv[14] = -m[0]  * m[5] * m[14] + 
              m[0]  * m[6] * m[13] + 
              m[4]  * m[1] * m[14] - 
              m[4]  * m[2] * m[13] - 
              m[12] * m[1] * m[6] + 
              m[12] * m[2] * m[5];

   inv[3] = -m[1] * m[6] * m[11] + 
             m[1] * m[7] * m[10] + 
             m[5] * m[2] * m[11] - 
             m[5] * m[3] * m[10] - 
             m[9] * m[2] * m[7] + 
             m[9] * m[3] * m[6];

   inv[7] = m[0] * m[6] * m[11] - 
            m[0] * m[7] * m[10] - 
            m[4] * m[2] * m[11] + 
            m[4] * m[3] * m[10] + 
            m[8] * m[2] * m[7] - 
            m[8] * m[3] * m[6];

   inv[11] = -m[0] * m[5] * m[11] + 
              m[0] * m[7] * m[9] + 
              m[4] * m[1] * m[11] - 
              m[4] * m[3] * m[9] - 
              m[8] * m[1] * m[7] + 
              m[8] * m[3] * m[5];

   inv[15] = m[0] * m[5] * m[10] - 
             m[0] * m[6] * m[9] - 
             m[4] * m[1] * m[10] + 
             m[4] * m[2] * m[9] + 
             m[8] * m[1] * m[6] - 
             m[8] * m[2] * m[5];

   det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
   det = 1.0 / det;

   mat44 ret; 
   for (i = 0; i < 16; i++) {
      ret.data[i] = (float)(inv[i] * det);
   }

   return ret; 
}
```