class OBB2 
{
   public:
      OBB2(); 
      OBB2( vec2 center,
         vec2 size = vec2::ZERO, 
         float rotationDegrees = 0.0f ); 

      // Modification Utility
      void Translate( vec2 offset );
      void MoveTo( vec2 position ); 

      void RotateBy( float degrees );           // A04
      void SetRotation( float degrees ); 

      void SetSize( vec2 size ); 

      // Helpers for describing it; 
      vec2 GetRight() const;
      vec2 GetUp() const; 
      vec2 GetCenter() const; 
      vec2 GetSize() const; 

      vec2 GetBottomLeft() const; 
      vec2 GetBottomRight() const; 
      vec2 GetTopLeft() const; 
      vec2 GetTopRight() const; 

      // Collision Utility
      vec2 ToLocalPoint( vec2 worldPoint ) const; 
      vec2 ToWorldPoint( vec2 localPoint ) const; 

      bool Contains( vec2 worldPoint ) const;
      vec2 GetClosestPoint( vec2 worldPoint ) const; 

      // Useful if you want to use AABB2 as a "broadphase" & "midphase" check
      // like checking if something fell outside the world
      AABB2 GetBoundingAABB() const; 

   public:
      vec2 m_right         = vec2( 1.0f, 0.0f ); 
      vec2 m_up            = vec2( 0.0f, 1.0f );   // not *needed* - Squirrel doesn't use it since you can easily derive it from "right"
      vec2 m_center        = vec2( 0.0f, 0.0f ); 
      vec2 m_half_extents  = vec2( 0.0f, 0.0f ); 
}; 