// Matrix Needs
// Translate
// RotationAroundX
// RotationAroundY
// RotationAroundZ
// RotationEuler( vec3, eRotationOrder ); 

enum eRotationOrder
{
   ROTATION_ORDER_XYZ,     // Unreal/Squirrel Defualt
   ROTATION_ORDER_ZXY,     // Unity/Forseth Defualt

   ROTATION_ORDER_DEFAULT = ROTATION_ZXY; 
}; 



//------------------------------------------------------------------------
// This is just here to show you how I store my matrix
// You will not be able to JUST copy and paste this; 
// NOTE:  My matrix math will look transposed from yours
// so transcribe to how yours if filled 
struct mat44 
{
   // alias as either a float array or 4 columns; 
   union {
      float[16] data; 
      vec4 col[4]; // columns; 
   }; 

   vec4 get_row( uint i ) const; 
   vec4 get_col( uint i ) const; 

   // I lay i, j, k along the rows; 
   vec4 get_i() const { return get_row(0); }
   vec4 get_j() const { return get_row(1); }
   vec4 get_k() const { return get_row(2); }
   vec4 get_t() const { return get_row(2); }

   // transforms
   vec4 transform( vec4 const v ) const;              
   vec3 transform_point( vec3 const other ) const;       // w = 1 (apply translation)
   vec3 transform_direction( vec3 const other ) const;   // w = 0 (don't translate) 
   mat44 transform( mat44 const &other ) const;          


   // new
   // optionally can return mat4& to "this", so you 
   // for some code shorthand, though I prefer to keep 
   // them one per line; 
   void make_identity(); 
   void transpose();
   void invert_orthonormal() const; 
   void invert(); 

   // helpers - just construct a rotation, and transform "this" by it; 
   void rotate_around_x( float deg ); 
   void rotate_around_x( float deg ); 
   void rotate_around_x( float deg ); 

   // add to the translation vector (do not affect the w)
   void translate( vec3 offset );     


   // STATIC named constructors
   static mat4 const IDENTITY;   // should be defualt - 1s along the diagnol, 0's everywhere else; 

   // Should be present from SD1
   static mat4 RotationAroundX( float deg ); 
   static mat4 RotationARoundY( float deg ); 
   static mat4 RotationARoundZ( float deg ); 
   static mat4 ForEuler( vec3 eulerDegrees, eRotationOrder rotOrder = ROTATION_ORDER_DEFAULT ); 
   static mat4 ForEulerXZY( vec3 eulerDegrees, eRotationOrder rotOrder = ROTATION_ORDER_DEFAULT ); 
   static mat4 Translation( vec3 pos ); 
}; 


//------------------------------------------------------------------------
// Implementation (Matrix4.cpp)
//------------------------------------------------------------------------
// Only implementing methods I believe are new from SD1.  Let me know if I have made an incorrect assumption; 



//------------------------------------------------------------------------
// Lifted from GLU - an expansion of Cramer's Rule
void mat44::invert_orthonormal()
{
   // Most transforms are used in games will end up being orthonormal or othogonal
   // "Orthogonoal" - all basis vectors are orthogonoal to each other (ie, Dot(i,j) = 0, Dot(j,k) = 0, and Dot(i,k) = 0)
   // "Orthonormal" - orthogonal where each basis vector is unit length; 

   // An orthonormal matrix represents a Rotation & Translation; 
   // R = Rotation;
   // T = Translation
   // this = R * T; 

   // So, I can invert by computing
   // invert(T) * invert(R)

   // mat44 m = R * T; 
   // then inverse(m) = inverse(T) * inverse(R)
   // inverse(R) = Transpose(R) (since it is orthonormal without translation)
   // inverse(T) = -T (just negate the translation part); 

   // so to quickly compute the rotation is...
   vec3 translation = get_t(); 
   set_t( vec3::ZERO );  // remove translation; 
   transpose(); // invert the rotation part;

   vec3 new_translation = transform( -translation ); // transforming the inverted translation by the new rotation (since invert T comes first)
   set_t( new_translation ); 
}

//------------------------------------------------------------------------
void mat44::invert()
{
   double inv[16];
   double det;
   double m[16];
   uint i;

   for (i = 0; i < 16; ++i) {
      m[i] = (double) data[i];
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

   inv[8] = m[4]  * m[9] * m[15] - 
            m[4]  * m[11] * m[13] - 
            m[8]  * m[5] * m[15] + 
            m[8]  * m[7] * m[13] + 
            m[12] * m[5] * m[11] - 
            m[12] * m[7] * m[9];

   inv[12] = -m[4]  * m[9] * m[14] + 
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] - 
            m[8]  * m[6] * m[13] - 
            m[12] * m[5] * m[10] + 
            m[12] * m[6] * m[9];

   inv[1] = -m[1]  * m[10] * m[15] + 
            m[1]  * m[11] * m[14] + 
            m[9]  * m[2] * m[15] - 
            m[9]  * m[3] * m[14] - 
            m[13] * m[2] * m[11] + 
            m[13] * m[3] * m[10];

   inv[5] = m[0]  * m[10] * m[15] - 
            m[0]  * m[11] * m[14] - 
            m[8]  * m[2] * m[15] + 
            m[8]  * m[3] * m[14] + 
            m[12] * m[2] * m[11] - 
            m[12] * m[3] * m[10];

   inv[9] = -m[0]  * m[9] * m[15] + 
            m[0]  * m[11] * m[13] + 
            m[8]  * m[1] * m[15] - 
            m[8]  * m[3] * m[13] - 
            m[12] * m[1] * m[11] + 
            m[12] * m[3] * m[9];

   inv[13] = m[0]  * m[9] * m[14] - 
            m[0]  * m[10] * m[13] - 
            m[8]  * m[1] * m[14] + 
            m[8]  * m[2] * m[13] + 
            m[12] * m[1] * m[10] - 
            m[12] * m[2] * m[9];

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

   for (i = 0; i < 16; i++) {
      data[i] = (float)(inv[i] * det);
   }
}

//------------------------------------------------------------------------
STATIC mat44 mat44::ForEuler( vec3 euler, eRotationOrder rot ) 
{
   mat44 rotx = RotationAroundX(euler.x); 
   mat44 roty = RotationAroundY(euler.y); 
   mat44 rotz = RotationAroundZ(euler.z); 

   // for games, we usually go around
   // forward, then right, then up
   mat44 ret; 
   if (rot == ROTATION_ORDER_ZXY) {
      ret = rotz;  // same as rotz.transform(ret) since ret is the identity
      ret = rotx.transform(ret); 
      ret = roty.transform(ret); 
   } else {
      // other cases
      ASSERT_CONTINUE(false); // implement unknown case
   }

   return ret; 
}

//------------------------------------------------------------------------
STATIC mat44::ForEulerZXY( vec3 euler, vec3 translation ) 
{
   mat44 mat = mat44::ForEuler( euler, ROATATION_ORDER_ZXY ); 
   mat.SetTranslation( translation ); 

   return mat; 
}