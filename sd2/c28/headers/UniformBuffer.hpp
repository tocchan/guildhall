//------------------------------------------------------------------------
// UniformBuffer.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
class UniformBuffer : public RenderBuffer       // A02
{
   public:
      UniformBuffer( RenderContext *owner );    // A02
      ~UniformBuffer();                         // A02

      void CopyCPUToGPU( void const *data, size_t const size );   // A02
}; 

