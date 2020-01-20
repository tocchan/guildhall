//------------------------------------------------------------------------
// RenderBuffer.hpp
// Render Buffers are GPU storage for information.  
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// GPU memory allocation; 
class RenderBuffer
{
   public:
      RenderBuffer( RenderContext *owner );  // A02
      ~RenderBuffer();                       // A02

      size_t GetSize() const;                // A02 
      bool IsStatic() const;                 // A02
      bool IsDynamic() const;                // A02
      
   protected:
      // for doing initial setup - we'll mark 
      // it as protected as the higher level classes
      // will help limit the number of options; 
      bool Create( void const *initialData, 
         size_t bufferSize, 
         siez_t elementSize, 
         eRenderBufferUsageBits usage, 
         eGPUMemoryUsage memUsage );         // A02

      // Assumes a buffer is already created, and copies this data into it; 
      // Only valid for DYNAMIC buffers; 
      bool CopyCPUToGPU( void const *data, size_t const byteSize );  // A02

   public:
      RenderContext *m_owner;                   // A02
      eRenderBufferUsageBits m_bufferUsage;     // A02
      eGPUMemoryUsage m_memoryUsage;            // A02

      size_t m_bufferSize;                      // A02
      size_t m_elementSize;                     // A02 

      // D3D11 Resource
      ID3D11Buffer *m_handle;                   // A02
}; 

