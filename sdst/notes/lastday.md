Overloading New/Delete

```cpp

static size_t gTotalAllocations = 0U; 
static size_t gTotalBytesAllocated = 0U; 
static size_t gAllocationsThisFrame = 0U; 
static size_t gBytesAllocatedThisFrame = 0U;

App::BeginFrame()
{
	// print previous frame; 
	Logf( "Allocated: %u, %u", gAllocationsThisFrame,
		gBytesAllocatedThisFrame ); 

	gAllocationsThisFrame = 0U; 
	gBytesAllocatedThisFrame = 0U; 
}

void* operator new( size_t size ) 
{
	++gTotalAllocations;
	++gAllocationsThisFrame; 
	gBytesAllocatedThisFrame += size; 

	return ::malloc( size ); 
}

void operator delete( void* ptr )
{
	--gTotalAllocations;

	::free(ptr); 
}

void* operator new( size_t byte_size, void* buf )
{
	return buf; 
}

// byte buf[128]; 
// Entity *ptr = new (buffer + 8) Entity(); 
// ptr->~Entity(); 

class Entity 
{
	public:
		void* operator new( size_t size )
		{
			// return ::malloc(size); 
			return ::new Entity(); 
		}
};

Entity *ent = new Entity(); 

void operator delete( void* ptr, void* buf ) 
{

}

// Entity *ptr; 
// delete (buf) ptr; 
// ptr->~Entity(); 


// IAllocator 
void operator new( size_t size, IAllocator& allocator )
{
	return allocator.alloc( size ); 
}

void operator delete( void* ptr, IAllocaotr& allocator )
{
	allocator.free(ptr); 
}
```


- Don't do one without the other
- Tracking Allocations (size/count) 
- Overloading new per object; 
- Overloading new with parameters
  - In-Place New
- Overloading delete with parameters (uncallable, but required)
  - Used when there is an exception in the constructor

- Hey, how much memory are we using a frame?
- How many allocations, and how much?

Memory
- Pools/Blocks
- Allocators
- Fragmentation
  - Internal Fragmentation
  - External Fragmentation
- Linear Allocator
- Stack Allocator
- Signpost Allocator
- Buddy Allocator
- Block Allocator
  - ObjectAllocator
  - ObjectPool

Writing a Profiler
- Samples/Measurements
- Building the trees
- Running reports
- Visualizing

- Speeding up recordings
- Speeding up the tree
  - In-Place Linked List 
  - In-Place Circular Linked List

- Tie in Memory tracking; 
