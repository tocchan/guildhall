//------------------------------------------------------------------------
// VertexBuffer.hpp
//------------------------------------------------------------------------

// Convenience class - makes working with render buffers meant for
// vertex buffers a little easier; 
class VertexBuffer : public RenderBuffer
{
   public:
      // Future: creating a static unchanging vertex buffer
      void CreateStaticFor( VertexPCU const *vertices, uint const count );

      // Used now - creates dyanmic vertex buffer meant to be changed often
      void CopyCPUToGPU( VertexPCU const *vertices, uint const count ); 

   public: 
      uint m_vertexCount;                   // how many vertices are stored currently; 
      // BufferLayout const *m_layout;   // TODO: VertexBuffer knows what kind of vertices are in it; 
};

//------------------------------------------------------------------------
// VertexBuffer.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void VertexBuffer::CreateStaticFor( VertexPCU const *vertices, uint const count )
{
   // just always create for static a new static buffer; 
   bool result = CreateBuffer( vertices, 
      sizeNeeded,        // total size needed for buffer?
      siezof(vertexPCU), // stride - size from one vertex to another
      RENDER_BUFFER_USAGE_VERTEX_STREAM_BIT, 
      GPU_MEMORY_USAGE_STATIC ); // probably want dynamic if we're using copy

   if (result) {
      m_vertexCount = count; 
      return true; 
   } else {
      m_vertexCount = 0U; 
      return false; 
   }
}

//------------------------------------------------------------------------
// Similar to UBO - since we are assuming if they are using this method 
// it is dynamic, and we only need to remake if the size changes; 
void VertexBuffer::CopyCPUToGPU( VertexPCU const *vertices, uint const count )
{
   // how many bytes do we need
   size_t sizeNeeded = count * sizeof(VertexPCU); 

   // if we don't have enough room, or this is a static
   // buffer, recreate (Create should release the old buffer)
   if (sizeNeeded > GetSize() || IsStatic()) {
      bool result = CreateBuffer( vertices, 
         sizeNeeded,        // total size needed for buffer?
         siezof(vertexPCU), // stride - size from one vertex to another
         RENDER_BUFFER_USAGE_VERTEX_STREAM_BIT, 
         GPU_MEMORY_USAGE_DYNAMIC ); // probably want dynamic if we're using copy

      if (result) {
         m_vertexCount = count; 
      } else {
         m_vertexCount = 0U; 
      }

      return result;

   } else {
      // non-static and we have enough room
      ASSERT( IsDynamic() ); 
      if (RenderBuffer::CopyCPUToGPU( vertices, sizeNeeded )) {
         m_vertexCount = count; 
         return true; 
      }
   }

   return false; 
}

