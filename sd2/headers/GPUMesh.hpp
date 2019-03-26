//------------------------------------------------------------------------
// GPUMesh.hpp
//------------------------------------------------------------------------

class GPUMesh                            // A04
{
   public:
      GPUMesh( RenderContext *ctx );     // A04

      // these actually mostly do the same thing - just differnt default
      // memory usage
      void CreateFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_STATIC ); // A04 - updated A06
      void CopyFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_DYNAMIC );  // A04

      void CopyVertexArray( VertexPCU const *vertices, uint count );                           // A04 Optional; 
      void CopyIndices( uint const *indices, uint count );                                          // A04 Optional;

      void SetDrawCall( bool useIndexBuffer, uint elemCount ); 

      inline BufferLayout const* GetLayout() const    { return m_layout; }

   public: 
      BufferLayout const *m_layout; 
      VertexBuffer *m_vertexBuffer; 
      IndexBuffer *m_indexBuffer; 
      
      // information for drawing; 
      uint m_elementCount; 
      bool m_useIndexBuffer; 
};

