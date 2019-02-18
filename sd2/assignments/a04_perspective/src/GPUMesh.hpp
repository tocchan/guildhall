//------------------------------------------------------------------------
// GPUMesh.hpp
//------------------------------------------------------------------------

class GPUMesh                            // A04
{
   public:
      GPUMesh( RenderContext *ctx );     // A04

      // these actually mostly do the same thing - just differnt default
      // memory usage
      void CreateFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_STATIC ); // A04
      void CopyFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_DYNAMIC );  // A04

   public: 
      VertexBuffer *m_vertexBuffer; 
      IndexBuffer *m_indexBuffer; 
      
      // information for drawing; 
      uint m_elementCount; 
      bool m_useIndexBuffer; 
};

