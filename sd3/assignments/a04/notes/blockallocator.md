
## BlockAllocator Implementatio

### Notes
- Allocators are usually given a **pool** of memory.
  - Think `BLOB` or Binary Large OBjects - generally a large buffer
  - So allocator should be able to fully work within that block of memory without asking for more
    - If given a base, try to allocate from base as little as possible; 
- Block allocators will only grow.
- Are very fast (mostly)
  - When allocating a chunk, at the mercy of the parent allocator, but this should be a rare occurance
  - Once saturated, all frees and allocations amount to a couple of pointer assignments; 
- Predictable internal fragmentation and no external fragmentation
- Bad cache-coherency (initially blocks will allocate linearly, but soon get shuffled by usage)
- Fairly simple to make thread-safe, even lockless

### New Concepts
- Alignment and `alignof`
  - What is it?
  - Why is it important?
  - `alloc_aligned`
  - `size` should always be a multiple of `alignment` 
    - why?



```cpp
struct block_t
{
	block_t* next; 
};

struct chunk_t 
{
	chunk_t* next; 
};

// Single-Threaded Block Allocator
class BlockAllocator : public Allocator
{
	public:
		// different ways to initialize the pool
		// one takes a base allocator to sub-allocate out of 
		// (which means it can grow as long as the base can allocate)
		bool init( Allocator* base, 
			size_t block_size, 
			size_t alignment, 
			uint blocks_per_chunk );

		// takes a static buffer, and is not allowed to grow; 
		bool init( void* buffer, 
			size_t buffer_size,  
			size_t block_size, 
			size_t alignment );

		void deinit(); 

		// interface implementation
		virtual void* allocate( size_t byte_size ) override; // works as long as size <= block_size
		virtual void free( void* ptr ) override; 

		// unique to a block allocator
		// allocates and frees a single block 
		void* alloc_block();
		void free_block( void* ptr );  

	private:
		// allocates a single chunk of memory
		// that is divided into blocks - will fail
		// if no base allocator is provided
		bool allocate_chunk(); 

	private: // members - implementation specific
		Allocator* m_base		= nullptr; // Allocator we're sub-allocating from

		block_t* m_free_blocks			= nullptr;	// keep track of free blocks 
		chunk_t* m_chunk_list		    = nullptr;  // some way to keep track of chunks if using a base allocator
													// challenge: try to track this wihout a specialized "chunk" header

		size_t m_alignment; 
		size_t m_block_size; 
		size_t m_blocks_per_chunk; 	// may prefer chunk_size?
}
```

```cpp
bool BlockAllocator::init( Allocator* base, 
	size_t block_size, 
	size_t alignment, 
	uint blocks_per_chunk )

bool BlockAllocator::init( void* buffer, 
	size_t buffer_size,  
	size_t block_size, 
	size_t alignment )
{
	// infer class members based on parameters
}

void BlockAllocator::deinit() 
{
	// if we have a base pool, free all chunks

	// reset to an invalid state
}

void* BlockAllocator::alloc_block()
{
	Block* block = get_free_block(); 
	if (block == nullptr) {
		if (!alloc_chunk()) {
			return nullptr; 
		}

		block = get_free_block(); 
	}

	return block; 
}

BlockAllocator::free_block( void* ptr ) 
{
	Block* block = (Block*)ptr;
	push_free_block( block );  
}

bool BlockAllocator::alloc_chunk()
{
	// allocate a chunk of memory from base if able

	// track this chunk so we can free it later

	// break chunk 
}

```

## Async Version

What isn't safe?
- ...


### ABA Problem