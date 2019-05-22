```cpp
enum eFrustumFace {
   FRUSTUM_RIGHT = 0,
   FRUSTUM_TOP,
   FRUSTUM_FRONT, 
   FRUSTUM_LEFT,
   FRUSTUM_BOTTOM, 
   FRUSTUM_BACK, 

   FRUSTUM_SIDE_COUNT,
};

struct frustum
{
   public:
      bool ContainsPoint( vec3 pos ) const; 

   public:
      plane3 planes[FRUSTUM_SIDE_COUNT]; 
};

class Camera 
{
   public:
      frustum GetWorldFrustum() const 
      {
         // gl would be -1 to 1 to GDC
         aabb3 ndc = aabb3::ThatContains( vec3(-1, -1, 0), vec3(1, 1, 1) ); 
         Matrix4x4 worldtoNDC = GetView() * GetProjection(); 
         Matrix4x4 NDCtoWorld = worldToNDC.GetInverse(); 
         
         //    6-----7
         //   /|    /|
         //  2-----3 |
         //  | 4---|-5
         //  |/    |/
         //  0-----1
         vec3 corners[8];
         ndc.GetCorners( corners ); 
         TransformPoints( 8, corners, NDCToWorld ); 

         frustum f; 

         // LH for left-handed basis - hence left handed winding order
         f.planes[FRUSTUM_LEFT] = plane3::FromTriangleLH( corners[0], corners[4], corners[6] ); 
         // ... 5 more times

         return f; 
      }

      frustum GetWorldFrustumFromClientRegion( aabb2 clientRegion ) 
      {
         vec3 min = ClientToNDC( clientRegion.GetMin(), 0.0f );
         vec3 max = ClientToNDC( clientRegion.GetMax(), 1.0f ); 
         aabb3 subNDC = aabb3::FromPoints( min, max ); 

         // repeat steps from above (or create a helper function that 
         // is GetWorldFrustumFromNDCRegion()...
         // ...         
      }
}

```