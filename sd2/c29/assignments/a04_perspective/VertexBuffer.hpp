class VertexBuffer : public RenderBuffer
{
   public:
      VertexBuffer();

      void UpdateVertices( uint vcount, void const* vertexData, uint vertexStride, buffer_attribute_t const* layout ); // A02

	public: 
		buffer_attribute_t const* m_attributes; // array describing the vertex
};