class Capsule2
{
   public:
      Capsule2(); 
      Capsule2( vec2 pos );                  // Equivalent to a point; 
      Capsule2( vec2 center, float radius ); // Equivalent to a disc; 
      Capsule2( vec2 p0, vec2 p1, float radius ); 

      // Modification Utility
      void SetPosition( vec2 pos );                         // special care here;   Use the center of the line as position, but maintain shape
      void SetPositions( vec2 p0, vec2 p1 ); 
      void Translate( vec2 offset ); 
      
      void RotateBy( float degrees );                       // A04

      // Helpers for describing it; 
      vec2 GetStart() const; 
      vec2 GetEnd() const; 

      // Collision Utility
      bool Contains( vec2 worldPoint ); 
      vec2 GetClosestPoint( vec2 worldPoint ); 

      // Useful if you want to use AABB2 as a "broadphase" & "midphase" check
      // like checking if something fell outside the world
      AABB2 GetBoundingAABB() const; 

   public:
      vec2 m_start         = vec2::ZERO;
      vec2 m_end           = vec2::ZERO; 
      float m_radius       = 0.0f; 
      // Note: defaults basically make a "point" at 0; 
}; 