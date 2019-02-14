//------------------------------------------------------------------------
// UniformBuffer.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// convenience class - makes working with a RenderBuffer
// for the purposes of constant data a little easier; 
class UniformBuffer : public RenderBuffer
{
   public:
      UniformBuffer( RenderContext *owner ); 
      ~UniformBuffer(); 

      // UniformBuffer's is public, as we can now make some assumptions
      // (for example, UniformBuffer's are usually dynamic based on our usage
      // (they don't have to be, but they are usually used for data that changes
      // so we'll have the default behaviour do that).
      void CopyCPUToGPU( void const *data, size_t const size ); 
}; 

//------------------------------------------------------------------------
// UniformBuffer.cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
bool UniformBuffer::CopyCPUToGPU( void const *data, size_t const byteSize ) 
{
   // if we don't have enough room, or this is a static
   // buffer, recreate (Create should release the old buffer)
   if (byteSize > GetSize() || IsStatic()) {
      bool result = CreateBuffer( data, 
         byteSize,        // total size needed for buffer?
         byteSize,        // 1U or byteSize is fine - not used for a UniformBuffer
         RENDER_BUFFER_USAGE_UNIFORMS_BIT, 
         GPU_MEMORY_USAGE_DYNAMIC ); // probably want dynamic if we're using copy

      return result; 

   } else {
      // non-static and we have enough room, so call down 
      // to our base classes CopyGPUToGPU that assumes
      // enough room is already allocated; 
      ASSERT( IsDynamic() ); 
      if (RenderBuffer::CopyCPUToGPU( vertices, byteSize )) {
         return true; 
      }
   }

   return false; 
}
}