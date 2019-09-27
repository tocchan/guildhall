## Allocator Implementation

### Notes
- Why?  Access and Usage patterns.
  - malloc/new are general purpose allocators
  - solve fragmentation
  - try to be fast - are not guaranteed to be; 
- Allocators are usually given a **pool** of memory.
  - Think `BLOB` or Binary Large OBjects - generally a large buffer
  - Allocators job is to decide how to give pieces of that memory to people
  - Some allocators take a base allocator to suballocate from.  
    - useful when memory usage is less important, but speed is (depends on the type of allocator)
  - So allocator should be able to fully work within that block of memory without asking for more
    - If given a base, try to allocate from base as little as possible; 


### New Concepts
- Alignment and `alignof`
  - What is it?
  - Why is it important?
  - `alloc_aligned`
  - `size` should always be a multiple of `alignment` 
    - why?


## UntrackedAllocator and/or TrackedAllocator
aka: DefaultAllocator

```cpp
class TrackedAllocator : public Allocator 
{
	public:
		void* alloc( size_t size ) final 
		{
			return TrackedAlloc( size ); 
		}

		void free( void* ptr ) final 
		{	
			TrackedFree( ptr ); 
		}

		static TrackedAllocator s_instance; 
};
```

## Linear Allocator
aka: Static Allocator

Not needed for this assignment;  Easiest allocator to talk about; 

```cpp
class LinearAllocator : public Allocator 
{
	public:
		bool init( void* buffer, size_t max_byte_size )
		{
			m_buffer = (byte*)buffer; 
			m_max_size = max_byte_size;
			m_offset = 0; 
		}

		virtual void* alloc( size_t size ) final 
		{	
			size_t new_offset = m_offset + size; 
			if (new_offset > m_max_size) {
				return nullptr; 
			}

			void* ret = m_buffer + m_offset; 
			m_offset = new_offset;  

			return ret; 
		}

		virtual void free( void* ) final
		{
			// do nothing...
		}

		inline void reset() 
		{
			m_offset = 0;   
		}

	private: 
		byte* m_buffer; 
		size_t m_max_size; 
		size_t m_offset; 
};

```

### Use Case
Dev Console System

```cpp
static byte DevConsolePool[1 MB]; 

void DevConsoleStartup() 
{
	m_command_allocator = LinearAllocator( DevConsolePool, sizeof(DevConsolePool) ); 
	//...
}; 

void RegisterCommand() 
{
	Command* cmd = m_command_allocator.create<Command>(); 
	cmd->m_name = m_command_allocator.alloc( name_len ); 
	strcpy_s( cmd->m_name, ... ); 
	// ... finish setting it up; 
	// don't use anything that uses heap/stack/etc... nothing requiring a delete; 
}

// THERE IS NO UNREGISTER
void DevConsoleShutdown()
{
	//... nothing
}

```

### Async Version

```cpp
class AsyncLinearAllocator : public Allocator 
{
	public:
		virtual void* alloc( size_t size ) final 
		{	
			if (m_offset > m_max_size) {
				return nullptr; 
			}

			size_t new_offset = (m_offset += size);  
			if (new_offset > m_max_size) {
				return nullptr; 
			}

			size_t old_offset = new_offset - size; 
			return m_buffer + old_offset; 
		}

		// option 2
		virtual void* alloc( size_t size ) 
		{
			while (true) {
				size_t old_offset = m_offset; 
				size_t new_offset = old_offset + size; 

				if (m_offset.compare_exchange_weak( old_offset, new_offset )) {
					break; 
				}
			}
		}

	public:
		byte* m_buffer; 
		size_t m_max_size; 
		
		std::atomic<size_t> m_offset; 
};


class SpinLock
{
	public:
		void lock()
		{
			m_value.compare_exchange_strong( 0, 1 ); 
		}

		void unlock()
		{
			m_value.exchange( 0 );  
		}

		std::atomic<int> m_value; 
}


// compare_exchange -> Compare and Set, or CAS operator
// YOU DO NOT WRITE THIS!
int CompareAndSet( volatile int* ptr, int comparand, int set_to )
{
	int old_value = *ptr; 
	if (old_value == comparand) {
		*ptr = set_to; 
	}

	return old_value; 
}
```


## Stack Allocator (theory)
aka: n/a

Not needed for this assignment;  Very useful for large temporary (scope based) storage.

```cpp
class StackAllocator
{
	alloc
	free

	push(); // save current head           
	pop();  // restore to last save head
};

// use case
void BlurImage( Image& img ) 
{
	gStackAllocator.push(); 

	byte* img_memory = gStackAllocator.alloc( /* enoug hmemory for the image*/ ); 

	// blur image in place

	// copy result back to passed in image; 

	gStackAllocator.pop(); 
}
```

### Async Version
- No.  By its nature it is designed for stack based work, which means using the same thread.  

## Stereo and Mailbox Buffers/Allocators
Variants of the Linear Allocator;  Not used in this assignment, but are fairly common in multi-threaded system design; 


## Block Allocator
- Block allocators will only grow.
- Are very fast (mostly)
  - When allocating a chunk, at the mercy of the parent allocator, but this should be a rare occurance
  - Once saturated, all frees and allocations amount to a couple of pointer assignments; 
- Predictable internal fragmentation and no external fragmentation
- Bad cache-coherency (initially blocks will allocate linearly, but soon get shuffled by usage)
- Fairly simple to make thread-safe, even lockless

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

		// AsyncBlockAllocator
		std::mutex m_chunk_lock; // when allocating chunks 
		std::mutex m_block_lock; // when allocating a single block
}
```

```cpp
bool BlockAllocator::init( Allocator* base, 
	size_t block_size, 
	size_t alignment, 
	uint blocks_per_chunk )
{
	m_base = base; 
	m_block_size = block_size; 
	m_alignment = alignment; 
	m_blocks_per_chunk = blocks_per_chunk; 

	m_free_blocks = nullptr; 
	m_chunk_list = nullptr; 

	alloc_chunk();  // up to you to do this here or not; 

	return true ;
}

bool BlockAllocator::init( void* buffer, 
	size_t buffer_size,  
	size_t block_size, 
	size_t alignment )
{
	// infer class members based on parameters
	m_blocks_per_chunk = buffer_size / block_size; 
	m_buffer_size = buffer_size; 
	m_block_size = block_size; 

	m_base = nullptr; 
	m_free_blocks = nullptr; 

	// allocating blocks from a chunk
	// may move this to a different method later; 
	break_up_chunk( buffer ); 
}

void BlockAllocator::break_up_chunk( void* chunk ) 
{
	byte* buf = (byte*)chunk; 
	block_t* first = (block_t*)buf;
	block_t* head = nullptr; 

	for (uint i = 0; i < m_blocks_per_chunk; ++i) {
		block_t* node = (block_t*)buf; 
		buf += m_block_size; 

		node->next = head; 
		head = node; 
	}

	{
		// scope lock list_lock
		first->next = m_free_blocks; 
		m_free_blocks = head; 
	}
}

void BlockAllocator::deinit() 
{
	// if we have a base pool, free all chunks
	if (m_base) {
		// free chunks
		while (m_chunk_list != nullptr) {
			list = m_chunk_list; 
			m_chunk_list = m_chunk_list->next; 			
			m_base->free( list ); 
		}
	} // else - normal cleanup; 

	// reset
	m_base = nullptr; 
	m_free_blocks = nullptr;
	m_block_size = 0U; 
	m_blocks_per_chunk = 0U;  
}

void* BlockAllocator::alloc_block()
{
	block_t* block = pop_free_block(); 
	while (block == nullptr) {
		if (!alloc_chunk()) {
			return nullptr; 
		}

		block = pop_free_block(); 
	}

	return block; 
}

block_t* BlockAllocator::pop_free_block() 
{
	// scope lock - list_lock
	block_t* head = m_free_blocks; 
	if (head != nullptr) {
		m_free_blocks = head->next; 
	}

	return head; 
}

BlockAllocator::free_block( void* ptr ) 
{

	block_t* block = (block_t*)ptr;
	push_free_block( block );  
}

void BlockAllocator::push_free_block( block_t* block ) 
{
	// scope lock - list_lock
	block->next = m_free_blocks;
	m_free_blocks = block; 
}

bool BlockAllocator::alloc_chunk()
{
	if (m_base == nullptr) {
		return false; 
	}

	if (m_chunk_lock.try_lock()) {
		// allocate a chunk of memory from base if able

		size_t chunk_size = m_blocks_per_chunk * m_block_size + sizeof(block_t); 

		block_t* chunk = (block_t*)m_base->alloc( chunk_size ); 
		if (chunk == nullptr) {
			return false; 
		}

		// track this chunk so we can free it later
		chunk->next = m_chunk_list; 
		m_chunk_list = chunk; 

		// break chunk 
		break_up_chunk( chunk + 1 ); 
	}

	return true; 
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

### ObjectAllocator and ObjectPool Variants
...

## Sign Post Allocator (theory only)
...

## Buddy Block Allocator (theory only)
...