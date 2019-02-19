class Plane2 
{
   public:
      Plane2()
         : m_data( 0.0f, 1.0f, 0.0f ) {}  // Plane point up on the origin; 
      explicit Plane2( vec3 data )
         : m_data(data) {}
      Plane2( vec2 normal, float distanceToOrigin )
         : m_data( normal, distanceToOrigin ) {}
     
      // positive -> in front of the plane
      // negative -> behind the plane; 
      float GetDistance( vec2 point ) const; 
      inline bool IsInFront( vec2 point ) const { return (GetDistance(point) > 0.0f); }
      inline bool IsBehind( vec2 point ) const { return (GetDistance(point) < 0.0f); }

      // Not needed for this class, but useful
      // vec2 GetProjectedPoint( vec2 point ) const; 


   public:
      union { 
         vec3 m_data; 
         struct { 
            vec2 m_normal; 
            vec2 m_distance; // distance along normal to get to the origin; 
         }
      }

   public:
      // named constructors;
      static Plane2 AtPosition( vec2 normal, vec2 pointOnPlane ); 
      static Plane2 FromPoints( vec2 p0, vec2 p1 ); 
}; 