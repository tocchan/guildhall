//------------------------------------------------------------------------
// GPUMesh.hpp
//------------------------------------------------------------------------

class GPUMesh                            // A04
{
   public:
      // ...

      // Changed in A06
      void CreateFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_STATIC ); // AA06 edit
      void CopyFromCPUMesh( CPUMesh const *mesh, eGPUMemoryUsage mem = GPU_MEMORY_USAGE_DYNAMIC );  // A04

      inline BufferLayout const* GetLayout() const    { return m_layout; }

   public:
      // ...

      // Changed in A06
      BufferLayout const *m_layout; 
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------


// When copying or creating - this will be similar; 
void GPUMesh::CreateFromCPUMesh( CPUMesh *mesh, ... )
{
   // 1. Need to allocate a buffer to copy into that 
   // matches out layout; 
   BufferLayout const *layout = mesh->GetLayout(); 
   ASSERT_OR_DIE( layout != nullptr ); // mesh MUST have a layout

   size_t sizeNeeded = layout->GetStride() * mesh->GetVertexCount(); 
   void *buffer = malloc( sizeNeeded );  // BE SURE TO FREE WHEN DONE WITH THIS

   // 2. Copy into that buffer
   layout->m_copyFromMaster( buffer, mesh->GetVertices(), mesh->GetVertexCount() ); 

   // 3. Update or create using this buffer
   // ... same as before

   // 4. Save off the layout - needed to create input layouts later; 
   m_layout = layout; 

   // don't forget to free memory allocated!!!
   free( buffer ); 
}