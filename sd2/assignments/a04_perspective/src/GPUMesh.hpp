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

      void CopyVertexArray( VertexPCU const *vertices, uint count );                           // A04 Optional; 
      void CopyIndices( uint const *indices, uint count );                                          // A04 Optional;

      void SetDrawCall( bool useIndexBuffer, uint elemCount ); 

   public: 
      VertexBuffer *m_vertexBuffer; 
      IndexBuffer *m_indexBuffer; 
      
      // information for drawing; 
      uint m_elementCount; 
      bool m_useIndexBuffer; 
};


//------------------------------------------------------------------------
// GPUMesh.cpp
//------------------------------------------------------------------------
GPUMesh::SetDrawCall( bool usesIndices, uint elemCount ) 
{
   if (usesIndices) {
      ASSERT( m_indexBuffer->GetIndexCount() >= elemCount ); 
   } else {
      ASSERT( m_vertexBuffer->GetVertexCount() >= elemCount ); 
   }

   m_elementCount = elemCount; 
   m_useIndexBuffer = usesIndices; 
}


//------------------------------------------------------------------------
void GPUmesh::CreateFromCPUMesh( CPUMesh const *mesh ) 
{
   // updated in A05 - NOT FINAL

   std::vector<VertexPCU> pcuVertices; 

   uint vcount = mesh->GetVertexCount(); 
   pcuVertices.reserve( vcount ); 

   for ( uint vIdx = 0; vIdx < vcount; ++vIdx ) {
      VertexPCU pcu; 
      
      // copy from master;
      pcu.position = mesh->m_vertices[vIdx].position; 
      pcu.color = mesh->m_vertices[vIdx].color; 
      pcu.uv = mesh->m_vertices[vIdx].uv; 

      pcuVertices.push_back(pcu); 
   }

   m_vertexBuffer->CreateFromCPU( pcuVertices ); 
   m_indexBuffer->CreateFromCPU( mesh->m_indices, mesh->GetIndexCount() ); 

   SetDrawCall( mesh->UsesIndexBuffer(), 
      mesh->GetElementCount() ); 
}
