## Allocator Implementation

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


## Linear Allocator
aka: Static Allocator

### Async Version

## Static Allocator

### Async Version

## Block Allocator

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
		virtual void* alloc( size_t byte_size ) final; // works as long as size <= block_size
		virtual void free( void* ptr ) final; 

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

### Async Version

What isn't safe?
- ...

How can we make it safe?
- Simple Lock Way

Lockless Version (optional)
- Lockless -vs- Waitless (fun topic for a DFS)
- Basic algorithm
- CAS operator
- ABA problem
- BE SURE TO TEST THIS THOROUGHLY
  - Have all your cores spinning, either all the same test, or a random assortment of these tests; 
    - Allocate and free a single node over and over again
    - Randomly choose between allocating and free a node.  When picking a node to free, pick randomly amongst currently allocated nodes from your own thread.
    - Same as above, allow all cores to add or remove from the same shared bucket
    - Allocate a random number of nodes, then free a random number of nodes, repeat...

If there is a bug in your lock-less algorithm, it will likely be very rare.  So if a unit test every fails or crashes, be sure to look into it.  These algorithms have a tendency to be very finicky, and compilier optimizations, different processors, etc, can sometimes change behaviour.  

Plus the benefit of lockless is usually minimal outside of very high contention cases (multiple threads hitting the same resources at the same time over a long period of time).  Very few systems you are likley to write are going to have this level of contention outside of *maybe* the job system, and only if you use it religiously for micro jobs.


### ABA Problem
ABA is a common problem in lock-less algorithms, stemming from object resuse.  The basic idea is;

1. We are trying to compare an object A and change it to some other value C.  This only works if during the change, A is still A.
2. While we save off C, someone else changes A to B.  Other things happen, and A is put back to the top of the list, but now with the next potential value needing to be D.
3. The original CAS runs with A vs A, but setting C instead of the now desired D

Ie, a value changes and then changes back, but some non-thread-safe data was not captured. 

Solutions to this problem personally encounted fall into two basic camps;
1. Store a unique nounce that is changed with A (usually making A larger, and depends on your processor supporting a larger CAS operator).  Usually called an 'aba' or 'nounce' member.  This doesn't solve the ABA problem, but makes it very *very* unlikely to happen (odds you wrap the nounce counter AND it gets assigned back to the original value are very unlikely)

2. Preventing things from being recycled until you have a safe 'sync' point.  Early on most lockless algorithsm were proved out in Java, C#, or some other managed language.  Since garbage collection is a hard lock on the entire system.  In C++, we can do this by saving off freed nodes into a secondary list, and then adding the list back to the main list if our list becomes empty.  



## Sign Post Allocator (theory only)
...

## Buddy Block Allocator (theory only)
...