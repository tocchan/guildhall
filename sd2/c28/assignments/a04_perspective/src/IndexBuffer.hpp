//------------------------------------------------------------------------
// IndexBuffer.hpp
//------------------------------------------------------------------------

class IndexBuffer : public RenderBuffer        // A04
{
   public:
      IndexBuffer( RenderContext *ctx );       // A04

      // Technically you can create 16-bit or 32-bit index buffers
      // but for simplicity we'll stick to just 32-bit; 
      void CreateStaticFor( uint const *indices, uint const count );          // A04
      void CopyCPUToGPU( uint const *indices, uint const count );            // A04

   public: 
      uint m_indexCount;                      // A04
};

