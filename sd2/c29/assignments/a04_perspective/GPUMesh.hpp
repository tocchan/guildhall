class GPUMesh // A04
{
	public:
		GPUMesh( RenderContext* owner ); 

		void UpdateVertices( uint vcount, void const* vertexData, uint vertexStride, buffer_attribute_t const* layout ); 
		void UpdateIndices( uint icount, uint const* indices ); 

		int GetIndexCount() const; 

		void UpdateIndices( std::vector<uint> const& indices ) 
		{
			UpdateIndices( indices.size(), &indices[0] ); 
		}

		void UpdateVertices( std::vector<VertexPCU> const& vertices )
		{	
			UpdateVertices( vertices.size(), 
				&vertices[0], 
				sizeof(VertexPCU), 
				VertexPCU::LAYOUT ); 
		}

		// helper template
		template <typename VERTEX_TYPE>
		void UpdateVertices( uint vcount, VERTEX_TYPE const* vertices ) 
		{
			UpdateVertices( vcount, vertices, sizeof(VERTEX_TYPE), VERTEX_TYPE::LAYOUT ); 
		}

	public:
		VertexBuffer* m_vertices;  // like a std::vector<VertexPCU> -> but on GPU
		IndexBuffer* m_indices; 	// like a std::vector<uint> -> but on the GPU

		// optionally
		// std::vector<uint8_t> m_cpuVertexData; 
		// std::Vector<uint> m_cpuIndexData; 
};


