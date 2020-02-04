- Definition of a Polygon
  - Convex -vs- Concave
    - `OBB2` is an example of a convex polygon
  - Convex Hull
    - Definition of a Plane
  - Checking for Convexity
  - Checking point inside or outside
  - Checking distance to a polygon
    - Distance to a line
    - Distance to a line segment (similar to OBB2)

## Polygon
Collection of point in counter-clockwise o

```cpp
class Polygon2D
{
	public:
		bool IsValid() const; // must have at least 3 points to be considered a polygon

		bool IsConvex() const; 
		bool Contains( Vec2 point ) const; 
		float GetDistance( Vec2 point ) const; 
		Vec2 GetClosestPoint( Vec2 point ) const; 

		// accessors
		int GetVertexCount() const;
		int GetEdgeCount() const; 
		void GetEdge( int idx, Vec2* outStart, Vec2* outEnd ); 

	public: // static constructors (feel free to just use a constructor - I just like descriptive names)
			// in this case, these two take the same parameters but behave differently

		// construct from a counter-clockwise line loop
		static Polygon2D MakeFromLineLoop( Vec2 const* points, uint pointCount );

		// create a convex wrapping of a collection of points;  
		static Polygon2D MakeConvexFromPointCloud( Vec2 const* points, uint pointCount ); 

	private:
		std::vector<Vec2> m_points; 
};
```

## `Plane2D`

```cpp
struct Plane2D 
{
	Vec2 normal; 
	float distanceFromOriginAlongNormal; // distance; 

	float GetDistance( Vec2 position ) const; 
}; 
```