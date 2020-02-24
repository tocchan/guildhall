class GPUMesh // A04
{
	public:
		void UpdateVertices( uint vcount, void const* vertexData, uint vertexStride, buffer_attribute_t const* layout ); 
		void UpdateIndices( uint icount, uint const* indices ); 

		int GetIndexCount() const; 

		// helper template
		template <typename VERTEX_TYPE>
		void UpdateVertices( uint vcount, VERTEX_TYPE const* vertices ) 
		{
			UpdateVertices( vcount, vertices, sizeof(VERTEX_TYPE), VERTEX_TYPE::LAYOUT ); 
		}

	public:
		VertexBuffer* m_vertices; 
		IndexBuffer* m_indices; 
};



