//------------------------------------------------------------------------
// RenderBuffer.hpp
// Render Buffers are GPU storage for information.  
//------------------------------------------------------------------------

// We have to specify what can a buffer be used 
// for.  It is possible for a buffer to serve multiple purposes
// though for our case, we'll be special casing them for clarity; 
enum eRenderBufferUsageBit : uint
{
   RENDER_BUFFER_USAGE_VERTEX_STREAM_BIT   = BIT_FLAG(0),   // Can be bound to an vertex input stream slot
   RENDER_BUFFER_USAGE_INDEX_STREAM_BIT    = BIT_FLAG(1),   // Can be bound as an index input stream.  
   RENDER_BUFFER_USAGE_UNIFORMS_BIT        = BIT_FLAG(2),   // Can be bound to a constant buffer slot; 
};
typedef uint eRenderBufferUsageBits; 

// In D3D11 and GL - buffers also are supplied hints on 
// desired usage patterns to help the GPU optimize for it
enum eGPUMemoryUsage 
{
   GPU_MEMORY_USAGE_GPU,     // Can be written/read from GPU only (Color Targets are a good example)
   GPU_MEMORY_USAGE_STATIC,  // Created, and are read only after that (ex: textures from images, sprite atlas)
   GPU_MEMORY_USAGE_DYNAMIC, // Update often from CPU, used by the GPU (CPU -> GPU updates, used by shaders.  ex: Uniform Buffers)
   GPU_MEMORY_USAGE_STAGING, // For getting memory from GPU to CPU (can be copied into, but not directly bound as an output.  ex: Screenshots system)
}; 


//------------------------------------------------------------------------
class RenderBuffer
{
   public:
      // for convenience - we'll have the RenderBuffer hold onto
      // the RenderContext used to create it so we're not constantly passing it in; 
      RenderBuffer( RenderContext *owner );
      ~RenderBuffer(); 

      size_t GetSize() const;    // return max byte size of this buffer; 
      bool IsStatic() const;     // has static usage?
      bool IsDynamic() const; 
      
   protected:
      // for doing initial setup - we'll mark 
      // it as protected as the higher level classes
      // will help limit the number of options; 
      bool Create( void const *initialData, 
         size_t bufferSize, 
         siez_t elementSize, 
         eRenderBufferUsageBits usage, 
         eGPUMemoryUsage memUsage );

      // Assumes a buffer is already created, and copies this data into it; 
      // Only valid for DYNAMIC buffers; 
      bool CopyCPUToGPU( void const *data, size_t const byteSize ); 

   public:
      RenderContext *m_owner; 
      eRenderBufferUsageBits m_bufferUsage; 
      eGPUMemoryUsage m_memoryUsage; 

      size_t m_bufferSize;    // total byte count of this buffer; 
      size_t m_elementSize;   // certain buffers have the idea of an element stride, which we'll store here; 

      // D3D11 Resource
      ID3D11Buffer *m_handle;  
}; 

//------------------------------------------------------------------------
// RenderBuffer.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// translate external enum to D3D11 specific options; 
// These will be used by texture as well,
// so may want to move this function to a common include; 
static D3D11_USAGE DXUsageFromMemoryUsage( eMemoryUsage const usage )
{
   switch (usage) {
      case GPU_MEMORY_USAGE_GPU:      return D3D11_USAGE_DEFAULT;
      case GPU_MEMORY_USAGE_STATIC:   return D3D11_USAGE_IMMUTABLE;
      case GPU_MEMORY_USAGE_DYNAMIC:  return D3D11_USAGE_DYNAMIC;
      case GPU_MEMORY_USAGE_STAGING:  return D3D11_USAGE_STAGING;
      default: ASSERT_RETURN_VALUE(false, D3D11_USAGE_DYNAMIC); 
   }
}


//------------------------------------------------------------------------
// Convert a buffer usage bitfield to a DX specific 
// version; 
static uint DXBufferUsageFromBufferUsage( eRenderBufferUsageBits const usage )
{
    uint ret = 0U;

   if (bind_flags & RENDER_BUFFER_USAGE_VERTEX_STREAM_BIT) {
      ret |= D3D11_BIND_VERTEX_BUFFER;
   }
   
   if (bind_flags & RENDER_BUFFER_USAGE_INDEX_STREAM_BIT) {
      ret |= D3D11_BIND_INDEX_BUFFER;
   }
   
   if (bind_flags & RENDER_BUFFER_USAGE_UNIFORMS_BIT) {
      ret |= D3D11_BIND_CONSTANT_BUFFER;
   }

   return ret;
}

//------------------------------------------------------------------------
// Creates a buffer; 
bool RenderBuffer::Create( void const *initialData, 
         size_t bufferSize, 
         siez_t elementSize, 
         eRenderBufferUsageBits usage, 
         eGPUMemoryUsage memUsage )
{
   // Free the old handle
   DX_SAFE_RELEASE(m_handle); 

   // can not create a 0 sized buffer; 
   ASSERT_RETURN_VALUE( (bufferSize > 0U) && (elementSize > 0U), false );

   // static buffers MUST be supplied data.
   ASSERT_RETURN_VALUE( (memUsage != GPU_MEMORY_USAGE_STATIC) || (initialData != nullptr), false );

   // Setup the buffer
   // When creating a D3D object, we setup
   // a DESC object to describe it...
   D3D11_BUFFER_DESC bd;
   MemZero( &bd );

   bd.ByteWidth = (UINT) bufferSize;
   bd.StructureByteStride = (UINT) elemSize;
   bd.Usage = DXMemoryUsageFromMemoryUsage(memUsage);
   bd.BindFlags = DXBufferUsageFromBufferUsage(bufferUsage);

   // give us write access to dynamic buffers
   // and read/write access to staging buffers; 
   bd.CPUAccessFlags = 0U;
   if (memUsage == GPU_MEMORY_USAGE_DYNAMIC) {
      bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   } else if (ci.usage == GPU_MEMORY_USAGE_STAGING) {
      bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
   }

   // TODO: support structured buffers (Summer Semester)
   //       I will probably forget about this....

   // Map initial data if relevant (something was passed in)
   D3D11_SUBRESOURCE_DATA data;
   D3D11_SUBRESOURCE_DATA *data_ptr = nullptr;
   if (initialData != nullptr) {
      MemZero( &data );
      data.pSysMem = initialData;
      data_ptr = &data;
   }

   // Create it - devices create resources; 
   ID3D11Device *dev = m_owner->m_device; 
   HRESULT hr = dev->CreateBuffer( &bd, data_ptr, &m_handle );

   if (SUCCEEDED(hr)) {
      // save off options; 
      m_bufferUsage = usage;
      m_memoryUssage = mmeUsage; 
      m_bufferSize = bufferSize; 
      m_elementSize = elementSize; 
      return true;
   } else {
      return false;
   }
}

//------------------------------------------------------------------------
bool RenderBuffer::CopyCPUToGPU( void const *data, size_t const byteSize )
{
   // staging or dynamic only & we better have room; 
   ASSERT( !IsStatic() ); 
   ASSERT( byteSize <= m_bufferSize ); 

   // Map and copy
   // This is a command, so runs using the context
   ID3D11DeviceContext *ctx = m_owner->m_context; 

   // Map (ie, lock and get a writable pointer)
   // Be sure to ONLY write to what you locked
   D3D11_MAPPED_SUBRESOURCE resource; 
   HRESULT hr = ctx->Map( GetHandle(), 
      0,    // resource index (for arrays/mip layers/etc...)
      D3D11_MAP_WRITE_DISCARD,  // says to discard (don't care) about the memory the was already there
      0U,   // option to allow this to fail if the resource is in use, 0U means we'll wait...
      &resource ); 

   if (SUCCEEDED(hr)) {
      // we're mapped!  Copy over
      memcpy( resource.pData, data, byteSize ); 

      // unlock the resource (we're done writing)
      ctx->Unamp( GetHandle(), 0 ); 
      
      return true; 

   } else {
      return false; 
   }
}
