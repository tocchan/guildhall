// In D3D11, NDC clip space is defined as only 0 to 1 on the z, as opposed to -1 to 1 
// that OpenGL does.  So I have implementations for both;
#define NDC_IS_ZERO_TO_ONE


//------------------------------------------------------------------------
// These matrices are assuming a "Z-Forward" basis (0 is near, 1 is far)
mat44 MakeProjectionOrthographic( float nx, float fx,
   float const ny, float const fy, 
   float const nz /*= 0.0f*/, float const fz /*= 1.0f*/ )
{
   float sx = 1.0f / (fx - nx);
   float sy = 1.0f / (fy - ny);
   float sz = 1.0f / (fz - nz);

   // maps (n, f) to (-1, 1)
   // == (x - n) / (f - n) * (2) - 1;
   // == 2(x - n) / (f - n) - (f - n)/(f-n)
   // == (2x / (f - n)) + (-2n - f + n)/(f - n)
   // == (2x / (f - n)) - (f + n) / (f - n);

   // So x = n, (- f - n + 2n) / (f - n) = -(f - n)/(f - n) == -1, checks out
   // And x = f, (-f - n + 2f) / (f - n) = (f - n) / (f - n) = 1, checks out

   #if defined(NDC_IS_ZERO_TO_ONE)
      return mat44(
         2.0f * sx,        (0.0f),             0.0f,         0.0f,
         0.0f,             (2.0f * sy),        0.0f,         0.0f,
         0.0f,             (0.0f),             1.0f * sz,    0.0f,
         -(fx + nx) * sx,  (-(fy + ny) * sy),  -nz * sz,     1.0f
      );
   #else
      return mat44(
         2.0f * sx,        (0.0f),             0.0f,             0.0f,
         0.0f,             (2.0f * sy),        0.0f,             0.0f,
         0.0f,             (0.0f),             2.0f * sz,        0.0f,
         -(fx + nx) * sx,  (-(fy + ny) * sy), -(fz + nz) * sz,  1.0f
      );
   #endif
}

//------------------------------------------------------------------------
mat4 MakeProjectionPerspective( float fovDegrees,  // what is our view angle; 
   float aspect,     // width/height of your target
   float nz,         // near clip plane
   float fz );       // far clip plane
{
   float rads = DegreesToRads(fovDegrees); 
   float size = 1.0f / tanf(rads / 2.0f);

   float w = size;
   float h = size;

   // I always grow the side that is large (so fov is for the minimum dimension)
   // This is a personal choice - most engines will just always prefer either width
   // or height (usually height); 
   if (aspect > 1.0f) {
      w /= aspect;
   } else {
      h *= aspect;
   }

   float q = 1.0f / (fz - nz);

   #if defined(NDC_IS_ZERO_TO_ONE)
      return mat44(
         w,          0.0f,         0.0f,            0.0f,
         0.0f,       h,            0.0f,            0.0f,
         0.0f,       0.0f,         (fz) * q,        1.0f,
         0.0f,       0.0f,         -nz * fz * q,    0.0f
      );
   #else
      return mat44(
         w,          0.0f,         0.0f,                 0.0f,
         0.0f,       h,            0.0f,                 0.0f,
         0.0f,       0.0f,         (fz + nz) * q,        1.0f,
         0.0f,       0.0f,         -2.0f * nz * fz * q,  0.0f
      );
   #endif
}