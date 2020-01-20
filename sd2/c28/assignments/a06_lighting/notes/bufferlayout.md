## Places We've Touched

- VertexMaster
- VertexPCU
- Mesh Construction 
- GPUMesh

- VertexLit
  - VertexPCUN

- Shader itself

- InputLayout
  - Wheere does it live?  Shader
  - When is it created?  First time you draw with this shader; 

## Buffer Layouts

```cpp

struct vertex_pcu
{
	vec3 position; 
	rgba color; 
	vec2 uv; 
};

struct vertex_lit
{
	vec3 position;
	vec3 normal; 
	rgba color; 
	vec2 uv; 

	static buffer_attribute_t[] LAYOUT; 
	static void CopyFromMaster( void *dst, vertex_master const *src, uint count ); 
}

struct vertex_master
{
	vec3 position;
	vec3 normal; 		// add normal - always the superset of things we can construct;  
	rgba color; 
	vec2 uv; 
};



enum eRenderDataFormat
{
	RDF_NULL, 

	RDF_FLOAT, 
	RDF_VEC2, 
	RDF_VEC3, 
	RDF_RGBA32, 
};

struct buffer_attribute_t
{
	std::string name; 
	eRenderDataFormat format; 
	size_t member_offset; 

	inline bool is_null() const { return format == RDF_NULL; }
};


//------------------------------------------------------------------------
// vertex_lit
//------------------------------------------------------------------------
//------------------------------------------------------------------------
STATIC buffer_attribute_t const vertex_lit::LAYOUT[] = {
   buffer_attribute_t( "POSITION",  RDF_VEC3,      offsetof(vertex_lit, position) ), 
   buffer_attribute_t( "COLOR",     RDF_RGBA32,    offsetof(vertex_lit, color) ), 
   buffer_attribute_t( "UV",        RDF_VEC2,      offsetof(vertex_lit, uv) ), 
   buffer_attribute_t( "NORMAL",    RDF_VEC3,      offsetof(vertex_lit, normal) ), 
   buffer_attribute_t( "TANGENT",   RDF_VEC4,      offsetof(vertex_lit, tangent) ), 
   buffer_attribute_t() // end
};

STATIC void vertex_lit::CopyFromMaster( void *buffer, vertex_master_t const *src, uint count ) 
{
	vertex_lit *dst = (vertex_lit*)buffer; 

	for (uint i = 0; i < count; ++i) {
		dst[i] = src[i]; // not actually easy, but copy elements that matter to lit; 
	}
}

byte_t *bytes = (byte_t*) myVertices; // vertex_lit myVertices[6]; 

```

## GPUMesh

```cpp

class CPUMesh
{
	public:
		std::vector<vertex_master> m_vertices; 
		std::vector<uint> m_indices;
		// nothing else...  
};

class GPUMesh
{
	public:
		// Lives in CPP (non-template)
		// Pass a buffer in the correct layout;
		bool create_from_cpu_data( void const *buffer, buffer_attribute_t const *layout, CPUMesh const &mesh )
		{
			// vertex count
			m_vbo = ...; // create from buffer and vertex count on mesh;
			m_vbo->m_layout = layout; 

			// indices still
			m_ibo = ...; // same as before; 
		}

		// have to be implemented in the header (h/hpp)
		template <typename VERTEX_TYPE>
		bool create_from_cpu_mesh( CPUMesh const &mesh )
		{
			// size of the vertex is the last element plus the size of that element
			size_t gpu_buffer_size = ComputeGPUBuffer( VERTEX_TYPE::LAYOUT, mesh.get_vertex_count() ); 
			void *buffer = malloc( gpu_buffer_size ); 

			VERTEX_TYPE::CopyFromMaster( buffer, mesh->get_vertices() ); 
			create_from_cpu_data( buffer, VERTEX_TYPE::LAYOUT, mesh ); 

			// Don't Forget to Free
			free( buffer ); 
		}

	public:
		VertexBuffer *m_vbo; 
		IndexBuffer *m_ibo; 
};

// Usage
Game::Startup()
{
	CPUMesh mesh; 
	MeshAddCube( &mesh, ... ); 

	RenderContext *rc = App::GetRenderContext();
	gpu_mesh = new GPUMesh( rc ); 
	gpu_mesh->create_from_cpu_mesh<vertex_lit>( mesh ); 
}

class VertexBuffer
{
	public:
		// ...
		buffer_attribute_t const *m_layout; 
};
```


## Create Input Layout for Buffer Layout