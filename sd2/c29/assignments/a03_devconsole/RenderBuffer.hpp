
// BIT_FLAG is a custom macro, sets the bit 
enum eRenderBufferUsageBit : uint
{
	VERTEX_BUFFER_BIT 		= BIT_FLAG(0),		// A02: can be used to store vertices
	INDEX_BUFFER_BIT		= BIT_FLAG(1),  	// we will discuss this later
	UNIFORM_BUFFER_BIT		= BIT_FLAG(2),		// A03: used to store constants
};
typedef uint eRenderBufferUsage;  

enum eRenderMemoryHint 
{
	RENDER_MEMORY_GPU,		// memory updated and used only from GPU side (if created from CPU, must be initialization data)
	RENDER_MEMORY_DYNAMIC, 	// something updated often from CPU side, used by GPU side 
	RENDER_MEMORY_STAGING,  // memory that can be read/written from CPU, and copied to a GPU buffer
};

// Manages some GPU memory
class RenderBuffer
{
	public:
		RenderBuffer( RenderContext* owner, eRenderBufferUsage usage, eRenderMemoryHint memoryHint ); 
		~RenderBuffer(); 

		void update( void const* data, size_t totalByteSize, size_t elemByteSize = 1 ); 

	public:
		RenderContext* m_owner; 
		ID3D11Buffer* m_handle; 
		eRenderBufferUsage m_usage; 
		eRenderMemoryHint m_memoryUsage; 

		// stats
		size_t m_totalByteSize;
		size_t m_elementByteSize; // used for vertex buffer  
};