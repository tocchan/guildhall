class GPUMesh // A04
{
	public:
		void UpdateVertices( uint vcount, void const* vertexData, uint vertexStride, buffer_attribute_t const* layout ); 
		void UpdateInidces( uint icount, uint const* indices ); 


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
