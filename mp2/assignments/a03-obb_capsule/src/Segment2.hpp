//--------------------------------------------------------------------------------
// Segment2.hpp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
class Segment2 
{
   public:
      Segment2( vec2 start, vec2 end );
     
      inline vec2 const& GetStart() const        { return m_start; }
      inline vec2 const& GetEnd() const          { return m_end; }
      vec2 GetCenter() const; 
      vec2 GetDirection() const; 

      // This is what we actually need; 
      // returns closest point on line segment (voronoi regions.  Either on the segment, or one of the end points)
      vec2 GetClosestPoint( vec2 pos ) const; 

   public:
      vec2 m_start;
      vec2 m_end; 
}; 


