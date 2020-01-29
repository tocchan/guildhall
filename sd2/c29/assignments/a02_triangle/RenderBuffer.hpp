
enum eRenderBufferUsageBit 
{

};

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
		void update( void const* data, size_t size ); 
};