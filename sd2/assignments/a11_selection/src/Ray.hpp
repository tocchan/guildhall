struct Ray3
{
   Ray3(); 
   Ray3( vec3 p, vec3 d ); 

   vec3 start        = vec3::ZERO; 
   vec3 direction    = vec3::FORWARD; // assumed normalized - by default, any unit vector is fine.

   // useful construction
   static Ray3 FromPoints( vec3 start, vec3 end ); 
}; 

// Functions
// All return ALL intersections (including negative).
uint Raycast( float *out, Ray3 ray, Sphere const &sphere );         // sphere
uint Raycast( float *out, Ray3 ray, Capsule3 const &capsule );      // capsule
uint Raycast( float *out, Ray3 ray, Plane3 const &plane );          // plane
uint Raycast( float *out, Ray3 ray, OBB3 const &bounds );           // obb3