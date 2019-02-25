//------------------------------------------------------------------------
// CPUMesh.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// VertexMaster is a super-set of all vertex parameters we
// will want our CPUMesh to know about; 
// (Historically I would call CPUMesh a MeshBuilder, as it is used 
// for construction of a mesh before it is sent to the GPU.  Like an Image for a Texture
struct VertexMaster
{
   vec3 position;          // A04
   rgba color;             // A04
   vec2 uv;                // A04;

   // vec3 normal;         // A05
   // vec4 tangent;        // A06

   // skin weights         // Summer
   // skin indices         // Summer
}; 

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class CPUMesh              // A04
{
   public:
      CPUMesh();           // A04

      void Clear();        // A04; 

      // Modify the stamp;
      void SetColor( rgba color );           // A04
      void SetUV( vec2 uv );                 // A04

      // Stamp a vertex into the list - return the index; 
      uint AddVertex( VertexMaster const &m );     // A04
      uint AddVertex( vec3 const &pos );           // A04

      // Adds a single triangle; 
      void AddIndexedTriangle( uint i0, uint i1, uint i2 );    // a04

      // adds two triangles (bl, tr, tl) and (bl, br, tr)
      void AddIndexedQuad( uint topLeft, uint topRight, uint bottomLeft, uint bottomRight );    // A04

      // Helpers
      uint GetVertexCount() const;                 // A04
      uint GetIndexCount() const;                  // A04

      inline bool UsesIndexBuffer() const          { return GetIndexCount() > 0; }
      inline uint GetElementCount() const          { return usesIndexBuffer() ? GetIndexCount() : GetVertexCount(); }


   public: 
      std::vector<VertexMaster>  m_vertices;       // A04
      std::vector<uint>          m_indices;        // A04

      VertexMaster m_stamp;                        // A04
};


//------------------------------------------------------------------------
// I prefer keeping my construction functions as C functions
// as I like to pick and choose the ones I want instead of having them
// all live in CPUMesh.hpp
//------------------------------------------------------------------------
void CPUMeshAddQuad( CPUMesh *out, aabb2 quad ); 

void CPUMeshAddCube( CPUMesh *out, aabb3 box );                                                          // A04
void CPUMeshAddUVSphere( CPUMesh *out, vec3 center, float radius, uint wedges = 32, uint slices = 16 );  // A04



//------------------------------------------------------------------------
// CPUMesh.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void CPUMeshAddQuad( CPUMesh *out, aabb2 quad )
{
   out->Clear(); 

   set->SetColor( RGBA::WHITE ); 
   // out->SetNormal( vec3::BACK ); 

   // 0 --- 1
   // |   / |
   // | /   |
   // 2 --- 3
   out->SetUV( vec2(0.0f, 0.0f) ); 
   out->AddVertex( quad.GetTopLeft() ); 
   
   out->SetUV( vec2(1.0f, 0.0f) ); 
   out->AddVertex( quad.GetTopRight() ); 
   
   out->SetUV( vec2(0.0f, 1.0f) ); 
   out->AddVertex( quad.GetBottomLeft() ); 
   
   out->SetUV( vec2(1.0f, 1.0f) ); 
   out->AddVertex( quad.GetBottomRight() ); 

   out->AddFace( 0, 2, 1 ); 
   out->AddFace( 2, 3, 1 ); 
}

//------------------------------------------------------------------------
void CPUMesh::AddIndexedTriangle( uint i0, uint i1, uint i2 )
{
   ASSERT( i0 < m_vertices.size() ); 
}
