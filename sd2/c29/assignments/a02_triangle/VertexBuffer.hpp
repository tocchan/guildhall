class VertexBuffer : public RenderBuffer
{
	public: 
		// information about our vertex
		size_t m_stride; // how far from one vertex to the next
		buffer_attribute_t const* m_attributes; // array describing the vertex
};