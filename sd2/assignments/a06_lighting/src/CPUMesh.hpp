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

   vec3 normal;            // A06
   // vec4 tangent;        // A07
}; 

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class CPUMesh            
{
   public:
      // ...

      // new stuff
      void SetLayout( BufferLayout const *layout ); // A06 

      BufferLayout const* GetLayout() const;       // A06
      VertexMaster const* GetVertices() const;     // A06 - just since I don't like accessing members direction

   public:
      // templated helper - so I can do things like
      // CPUMesh mesh;  
      // mesh.SetBufferLayout<VertexPCU>(); 
      template <typename T>
      void SetLayout() 
      {
         SetLayout( BufferLayout::For<T>() ); 
      }

   public: 
      std::vector<VertexMaster>  m_vertices;       // A04
      std::vector<uint>          m_indices;        // A04

      VertexMaster m_stamp;                        // A04
      BufferLayout const *m_layout;                // A06
};



