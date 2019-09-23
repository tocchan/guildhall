
Profiling System
- Instrumentation
- Design

Views
- Cost Graph
- Timeline View

Recording: Theory
- end goal views
  - usage
- data structure that makes sense
- don't want to slow down our game *too much*. 

Reports: Theory
- can take more time
- help use find our where we should work


Recording: Practice
- What do we have to record
- Error Checking
- Grabbing old trees
  - Ref Counting
- Cleaning up old trees
- Pausing and Resuming

Interlocked Operations
- relation to std::atomic
- why I prefer them (especially in this case)

Allocators
- Why
- Basic Structure
- `LinearAllocator` (not used in assignment): aka `StaticAllocator`
  - Lockless Linear Allocator: `AsyncLinearAllocator` if you want variety
- `BlockAllocator`: aka, `PageAllocator` or `ObjectAllocator`
  - Why?
  - Our implementation

Reports: Practice
- Slower than recording, do on demand
- Tree View -vs- Flat View
  - Fancy: Timeline View
- Compare Operator
  - 

Handling Threads
- Thread Local Storage

Handling Memory Allocations
- Extra data on a node (depends on memory allocation mode)

Visualizing
- Frame-v-Time Graph
- Timeline View
- Memory Allocations (use memory system, not profiler system)
  - Profiler system is just to quickly see (where) an allocation happens


