//------------------------------------------------------------------------
// VertexBuffer.hpp
//------------------------------------------------------------------------

class VertexBuffer : public RenderBuffer        // A02
{
   public:
      VertexBuffer( RenderContext *ctx );       // A02

      void CreateStaticFor( VertexPCU const *vertices, uint const count );    // A02
      void CopyCPUToGPU( VertexPCU const *vertices, uint const count );       // A02

   public: 
      uint m_vertexCount;                   // A02 
      BufferLayout const *m_layout;         // A05 
};

