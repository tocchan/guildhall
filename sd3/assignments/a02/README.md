SD3.a02
======

## Grading


## Unit Test

```cpp

#define TRACK_MEMORY 		 	// Just logs total number of "live" allocations
#define TRACK_MEMORY_VERBOSE	// If TRACK_MEMORY is also true, this will also track callstacks/bytes for live allocations

// "allocator" that doesn't track
void* UntrackedAlloc( size_t size ) { return ::malloc(size); }
void UntrackedFree( void* ptr ) 	{ ::free(ptr); }

void TrackAllocation( void* ptr, size_t size ); 
void UntrackAllocation( void* ptr ); 

static void AllocTest()
{

}

UNITTEST("A02", nullptr, 0) 
{
	uint live_allocations = MemoryTrackingGetLiveAllocations(); 

	uint core_count = std::thread::hardware_concurrency(); 

	while (core_count > 0) {
		::Sleep(10); 
	} 
}

//
```