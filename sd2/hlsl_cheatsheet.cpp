

// SHADER DATA DECLARATION
// if a register is not set - compiler will auto-choose one.
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd607359(v=vs.85).aspx

// DECLARING INPUTS
// [c# is actually forcing a certain buffer offset]
float3 name : SEMANTIC : register(c#) 

// CONSTANT BUFFER
cbuffer name : register(b#) { /* data */ }      // docs say (cb#) ?

// TEXTURES
Texture1D name : register(t#)
Texture1DArray
Texture2D
Texture2DArray
Texture2DMS       // Multisample
Texture2DMSArray  // Multisample Array
Texture3D
Texture3DArray
TextureCube
TextureCubeArray

// STRUCTURED BUFFERS [same registers as textures]
StructuredBuffer<float4x4> tSkinMatrices : register(t5);

// Samplers
Sampler name : register(s#)

// UAVs
RWBuffer log : register(u#)
RWByteAddressBuffer // indexed by byte, stores uints [so index is mul of 4]
RWStructedBuffer<T>
RWTexture1D
RWTexture1DArray
RWTexture2D
RWTexture2DArray
RWTexture3D
RWTexture3DArray

// Variable Prefixes
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509706(v=vs.85).aspx
globallycoherent: optional prefix - atomic operations are over whole GPU

// COMPUTE PREFIXES
groupshared: Variable is shared within a thread group


// HLSL CHEAT SHEET
//
// key: 
// b: bool
// i: integer
// u: unsigned integer
// f: float
// d: double
//
// s: scalar
// v: vector
// m: matrix
//
// For example, abs can take either an scalar, vector or matrix, and returns a scalar, vector or matrix
// so I write it takes an svm0.  Meaning it is the first type, and it returns what we pass in,  And that thing
// be any of the three.  There is not type specified, so assume any type works.
//
// acos can also take svm, but only works with floats. So it is fsvm0

// FLOW
void abort();  // Cancels the draw call
void errorf( string format, ...arguments ); // submits an error message to the information queue. [see ID3D11InfoQueue]
void printf( string format, ...arguments ); // submits an debug message to the information queue.


// LOGIC
bool all( svm0 v ); // returns true if all components are non-zero.
bool any( svm0 v ); // returns true if any component is non-zero.
uint countbits( uint v ); // returns number of on bits (non-zero) bits. 
uint reversebits( uint v ); // reverses the bits.
int firstbithigh( isv0 input ); // returns the first set bit counting from highest order bit (negative numbers always return 0) (there are unsigned equivalents)
int firstbitlow( isv0 input ); // returns the first set bit counting from the lowest order bit.  Unsigned equivalents exist.

// LIGHTING 
float4 lit( float n_dot_l, float n_dot_h, float m ); // returns a lighting coefficient vector (ambient, diffuse, specular, 1), where
                                                     //   ambient = 1, 
                                                     //   diffuse = n_dot_l < 0 ? 0 : n_dot_l, 
                                                     //   ambient = (n_dot_l < 0) || (n_dot_h < 0) ? 0 : n_dot_h * m


// MATH
svm0 abs( svm0 v );  // returns absolute value
fsvm0 ceil( fsvm0 v ); // takes the ceiling (rounds up) the input components
fsvm0 floor( fsvm0 v ); // takes the floor (rounds down) of input components
fsvm0 round( fsvm0 v ); // rounds to the nearest whole number
fsvm0 clamp( fsvm0 x, fsvm0 min, fsvm0 max ); // returns x clamped to min and max (min must be <= max);
fsvm0 saturate( fsvm0 x ); // clamps x to the range [0, 1];
svm0 min( svm0 a, svm0 b ); // returns per component the minimum element
svm0 max( svm0 a, svm0 b ); // returns per component the maxiumum element
isvm0 sign( fsvm0 x ); // returns the sign of x (1, 0, or -1)

fsvm0 degrees( fsvm0 radians ); // convert radians to degrees
fsvm0 radians( fsvm0 degrees ); // converts degrees to radians.

fsvm0 sqrt( fsvm0 x ); // returns sqrt of x
fsvm0 fmod( fsvm0 x, fsvm0 y ); // returns the remainder of x / y.  
fsvm0 modf( fsvm x, out fsvm0 ip ); // returns signed fractional part of x.  ip contains the signed whole part.
fsvm0 frac( fsvm0 v ); // returns the fractional part of v (so value in [0, 1])
fsvm0 rcp( fsvm0 x ); // calculates an approximate, fast reciprocal.
fsvm0 rsqrt( fsvm0 x ); // calculates a fast approximate of the squareroot of the reciprocal

fsvm0 exp( fsvm0 v ); // returns e^x
fsvm0 exp2( fsvm0 v ); // return 2^x
fsvm0 pow( fsvm0 x, fsvm0 exp ); // returns x^exp
fsvm0 log( fsvm0 v ); // return base-e log function of v
fsvm0 log2( fsvm0 v ); // returns base-2 log function of v
fsvm0 log10( fsvm0 v ); // returns base-10 log function of v


bsvm0 isfinite( fsvm0 v ); // returns if v is finite
bsvm0 isinf( fsvm0 v ); // returns if v is infinite
bsvm0 isnan( fsvm0 v ); // returns of v is NAN or QNAN

svm0 mad( svm0 a, svm0 b, svm0 c ); // returns (a * b) + c for any single precision numeric type (not double)
dsvm0 fma( dsvm0 a, dsvm0 b, dsvm0 c ); // returns (a * b) + c for double types.

// blending/checking
fsvm0 lerp( fsvm0 x, fsvm0 y, fsvm0 b ); // Return the linear interpolation between x and y determined by b
fsvm0 step( fsvm0 compare, fsvm0 v ); // returns: (v >= compare) ? 1 : 0
fsvm0 smoothstep( fsvm0 min, fsvm0 max, fsvm0 x ); // returns a hermite interpolation between min and max based on x


// trig
fsvm0 cos( fsvm0 radians ); // returns cos of input
fsvm0 acos( fsvm0 v ); // returns arccos in radians
fsvm0 sin( fsvm0 radians ); // returns sin of input
fsvm0 asin( fsvm0 v ); // returns arcsin in radians
fsvm0 tan( fsvm0 radians ); // returns tangent in radians;
fsvm0 atan( fsvm0 v ); // returns arctangent in radians
fsvm0 atan2( fsvm0 y, fsvm0 x ); // returns arctangent in radians (quadrant aware)
void sincos( fsvm0 radians, out fsvm0 sinv, out fsvm0 cosv ); // calculates both sin and cos of radians.

// hyperbolic
fsvm0 cosh( fsvm0 radians ); // returns hyperbolic cos of input
fsvm0 sinh( fsvm0 radians ); // returns hyperbolic sin of input
fsvm0 tanh( fsmv0 radians ); // returns hyperbolic tan of input


// linear
s0 dot( v0 a, v0 b ); // returns the dot product of two vectors
float3 cross( float3 a, float3 b ); // returns the cross product of a and b;
v0 mul( v0 v, m0 m ); // returns v * m (where v is a row vector multiplied by a square matrix).  (these can be flipped)

float determinant( fm0 square_matrix ); // returns the detminant of a square matrix
fm0 transpose( fm0 square_matrix ); // transpose the matrix.

float distance( fv0 a, fv0 b ); // returns the distance between two vectors.
fv0 dst( fv0 a, fv0 b ); // returns the distance vector between two vectors.
float length( fv0 a ); // returns the length of the supplied vector
fv0 normalize( fv0 v ); // returns a normalized vector.

fvo reflect( fv0 direction, fv0 normal ); // returns direction reflected by normal
fv0 refract( fv0 ray_direction, fv0 normal, float ref_index ); // returns refraction vector


fv0 facefoward( fv0 n, fv0 i, fv0 ng ); // -n× sign(dot(i, ng)).  (Flips the surface-normal (if needed) to face in a direction opposite to i; returns the result in n.)


// Conversions
double asdouble( uint lowbits, uint hibits ); // converts bit pattern to a double
fsvm0 asfloat( svm1 v ); // converts input to float type
isvm0 asint( svm1 v ); // converts input to int type
usvm0 asuint( svm1 v ); // converts input to uint type
fsv0 f16tof32( usv0 v ); // convert float16 stored in loword of v to a float.
usv0 f32tof16( fsv0 v ); // converts float to a float16 rep stored in loword of returned uint.

// Strange/Specialized
fsvm0 frexp( fsvm0 v, out fsvm0 exp ); // returns the mantissa, and puts the expononent in exp.
fsvm0 ldexp( fsvm0 x, fsvm exp ); // returns: x * 2^exp 
uint4 msad4( uint ref, uint2 source, uint4 accum ); // see MSDN


///////////////////
// FRAGMENT ONLY //
///////////////////
// FLOW
void clip( float v );   // Discards this fragment if (v < 0)

// MATH
fsvm0 ddx( fsvm0 v ); // returns partial derivative of the x parameter (think of it running on 3 fragments at one, your current, and the one to each side)
fsvm0 ddx_course( float v ); // computes low precision partial derivative with respect to screen-space x coordinate
fsvm0 ddx_fine( float v ); // computes high precision partial derivative with respect to screen-space x coordinate;
fsvm0 ddy( fsvm0 v ); // returns partial derivative of the y parameter (think of it running on 3 fragments at one, your current, and the one to each side)
fsvm0 ddy_course( float v ); // computes low precision partial derivative with respect to screen-space y coordinate
fsvm0 ddy_fine( float v ); // computes high precision partial derivative with respect to screen-space y coordinate;

fsvm0 fwidth( fsvm0 v ); // returns:  abs(ddx(x)) + abs(ddy(x))

// TEXTURE FUNCTIONS (Tex1D and such) are 
// deprectaed in favor of Sampler objects.
texN.Sample( samplerN, floatN );




// Declaring Inputs/Outputs [shader stage AND functions
[InputModifier] Type Name [: Semantic] [InterpolationModifier] [= Initializers]

/**
 InputModifier;
   in:         // Input Only
   inout:      // Input and Output
   out:        // Output Only
   uniform:    // Input only constant data

 Type: 
   HLSL Type

 Semantic: 
   User Defined Name

 InterpolationModifier:
   linear:           Default Perspective Correct Linear Interpolation
   centroid:         ?
   nointerpolation:  Do not interpolate
   noperspective:    Do not perform perspective correction
   sampler:          4.1 or Later:  Superssampling... ?
*/

