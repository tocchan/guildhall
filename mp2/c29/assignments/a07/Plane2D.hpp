struct Plane2D
{
   vec2 normal; 
   float distance; 

   // methods
   Plane2D()
      : normal(0,1)
      , distance(0) {}

   Plane2D( vec2 n, vec2 pointOnPlane )
      : normal(n)
      , distance(Dot(pointOnPlane,n)) {}


   bool GetPointIsInFront( vec2 point )
   {
      float pointDistFromPlane = Dot( point, normal ); 
      float distanceFromPlane = pointDistFromPlane - distance; 
      return distanceFromPlane > 0.0f; 
   }

   float GetSignedDistanceFromPlane( vec2 point )
   {
      float pointDistFromPlane = Dot( point, normal ); 
      float distanceFromPlane = pointDistFromPlane - distance; 
      return distanceFromPlane; 
   }
};