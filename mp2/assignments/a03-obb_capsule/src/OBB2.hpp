
//--------------------------------------------------------------------------------
// OBB2.hpp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
class OBB2 
{
   public:
      OBB2(); 
      OBB2( vec2 center,
         vec2 size = vec2::ZERO, 
         float rotationDegrees = 0.0f ); 
      explicit OBB2( AABB2 const &aabb ); 

      // Modification Utility
      void Translate( vec2 offset );
      void MoveTo( vec2 position ); 

      void RotateBy( float degrees );           // A04
      void SetRotation( float degrees ); 

      void SetSize( vec2 size ); 

      // Helpers for describing it; 
      inline vec2 GetRight() const        { return m_right; }
      inline vec2 GetUp() const           { return m_up; } 
      vec2 GetCenter() const; 
      vec2 GetSize() const; 

      inline vec2 GetBottomLeft() const   { return center - m_extents.x * GetRight() - m_extents.y * GetUp(); }
      vec2 GetBottomRight() const; 
      vec2 GetTopLeft() const; 
      vec2 GetTopRight() const; 
      
      // void GetCorners( vec2 *out ) const; // tl, tr, bl, br
      void GetPlanes( Plane2 *out ) const; 

      // Collision Utility
      vec2 ToLocalPoint( vec2 worldPoint ) const; 
      vec2 ToWorldPoint( vec2 localPoint ) const; 

      bool Contains( vec2 worldPoint ) const;
      vec2 GetClosestPoint( vec2 worldPoint ) const;

      bool Intersects( OBB2 const &other ) const; 

      // Useful if you want to use AABB2 as a "broadphase" & "midphase" check
      // like checking if something fell outside the world
      AABB2 GetBoundingAABB() const; 

   public:
      vec2 m_right         = vec2( 1.0f, 0.0f ); 
      vec2 m_up            = vec2( 0.0f, 1.0f );   // not *needed* - Squirrel doesn't use it since you can easily derive it from "right"
      vec2 m_center        = vec2( 0.0f, 0.0f ); 
      vec2 m_extents       = vec2( 0.0f, 0.0f ); 
}; 

//--------------------------------------------------------------------------------
// OBB2.cpp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
OBB2::OBB2( AABB2 const &aabb )
{
   m_right = vec2( 1.0f, 0.0f ); 
   m_up = vec2( 0.0f, 1.0f ); 
   m_center = (aabb.GetMin() + aabb.GetMax()) * 0.5f;
   m_extents = (aabb.GetMax() - aabb.GetMin()) * 0.5f; 
}


//--------------------------------------------------------------------------------
void OBB2::SetRotation( float degrees )
{
   m_right = vec2( CosDegrees(degrees), SinDegrees(degrees) );
   m_up = vec2( -m_right.y, m_right.x ); 
}

//--------------------------------------------------------------------------------
void OBB2::RotateBy( float degrees )
{
   m_right.RotateBy( degrees ); 
   m_up = m_right.RotateBy90Degrees(); 
}

//--------------------------------------------------------------------------------
vec2 OBB2::ToLocalPoint( vec2 worldPoint ) 
{
   vec2 disp = worldPoint - GetCenter();  

   // world i = m_right;
   // world j = m_up
   float localI = Dot( disp, GetRight() ); 
   float localJ = Dot( disp, GetUp() ); 

   return vec2( localI, localJ ); 
}

//--------------------------------------------------------------------------------
vec2 OBB2::ToWorldPoint( vec2 localPoint ) 
{
   // world i = m_right;
   // world j = m_up

   return localPos.x * GetRight() + localPos.y * GetUp()    // Rotate
      + center;                                             // Translate
}

//--------------------------------------------------------------------------------
bool OBB2::Contains( vec2 worldPoint ) const
{
   vec2 localPoint = ToLocalPoint( worldPoint ); 
   vec2 absLocalPoint = Abs(localPoint); 
   return (absLocalPoint.x < m_extents.x) && (absLocalPoint.y < m_extents.y); 
}

//--------------------------------------------------------------------------------
vec2 OBB2::GetClosestPoint( vec2 worldPoint ) 
{
   vec2 localPoint = ToLocalPoint( worldPoint ); 
   vec2 clampedPoint = Clamp( localPoint, -m_extents, m_extents ); 

   return ToWorldPoint( clampedPoint ); 
}

//--------------------------------------------------------------------------------
bool OBB2::Intersects( OBB2 const &other ) const
{
   Plane2 planesOfThis[4];    // p0
   Plane2 planesOfOther[4];   // p1

   vec2 cornersOfThis[4];     // c0
   vec2 cornersOfOther[4];    // c1

   GetPlanes( planesOfThis ); 
   GetCorners( cornersOfThis ); 

   other.GetPlanes( planesOfOther ); 
   other.GetCorners( cornersOfOther ); 

   for (uint planeIdx = 0; planeIdx < 4; ++planeIdx) {
      Plane2 const &planeOfThis = planesOfThis[planeIdx]; 
      Plane2 const &planeOfOther = planesOfOther[planeIdx]; 

      uint inFrontOfThis = 0U; 
      uint inFrontOfOther = 0U; 
      for (uint cornerIdx = 0; cornerIdx < 4; ++cornerIdx) {
         vec2 const &cornerOfThis = cornersOfThis[cornerIdx]; 
         vec2 const &cornerOfOther = cornersOfOther[cornerIdx];

         float otherFromThis = planeOfThis.GetDistance( cornerOfOther ); 
         float thisFromOther = planeOfOther.GetDistance( cornerOfThis ); 
         inFrontOfThis += (otherFromThis >= 0.0f) ? 1U : 0U; 
         inFrontOfOther += (thisFromOther >= 0.0f) ? 1U : 0U; 
      }

      // we found a sepearting plane; 
      if ((inFrontOfThis == 4) || (inFrontOfOther == 4)) {
         return false;
      }
   }

   // if I get to the end, I intersect
   return true; 
}


void GetManifoldForOBB2WithRadius( OBB2 const &a, float ra,  
   OBB2 const &b, float rb ) 
{
   if (GetManifold( out, a, b )) {
      out->penetration += (ra + rb); 
      return true;
   }

   Segment2 sidesA[] = a.GetSides(); 
   Segment2 sidesB[] = b.GetSides(); 
   vec2 cornersA = a.GetCorners(); 
   vec2 cornersB = b.GetCorners(); 

   vec2 bestA; 
   vec2 bestB; 

   for each (sideA and sideB) {
      foreach (cornerA and cornerB) {
         vec2 closestA = sideA.GetClosestPoint(cornerB); 
         vec2 closestB = sideB.GetClosetsPoint(cornerA); 

         // distances are...
         // closestA to cornerB
         // closestB to cornerA
         if ((closestA - cornerB).LengthSquared() < bestMatch) {
            bestA = closestA; 
            bestB = cornerB; 
            bestMatch = lengthSquared;
         }

         if ((closestB - cornerA).LengthSquared() < bestMatch) {
            bestA = cornerA; 
            bestB = cloestB; 
            bestMatch = lengthSquared; 
         }
      }
   }

   // two closets points are bestA, bestB; 
   // normal is directoin between them;
   // penetration is sum of radius - distance; 


}



