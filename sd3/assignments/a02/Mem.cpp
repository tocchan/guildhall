#include "Engine/Mem.hpp"
#include <malloc.h>

//------------------------------------------------------------------------
std::string GetSizeString( size_t byte_count )
{
	// TODO 
}

//------------------------------------------------------------------------
void* UntrackedAlloc( size_t size ) 
{
	// allocate memory off the free-store
	return ::malloc( size ); 
}

//------------------------------------------------------------------------
void UntrackedFree( void* ptr ) 
{
	// return memory to the free-store
	return ::free( ptr ); 
}

//------------------------------------------------------------------------
void* TrackedAlloc( size_t byte_count )
{
	// One suggestion and example on how to break up this function
	// based on build config; 
	#if !defined(MEM_TRACKING)
		return UntrackedAlloc(byte_count); 
	#else
		// TODO

		#if MEM_TRACKING == MEM_TRACK_ALLOC_COUNT
			// track allocation count

		#elif MEM_TRACKING == MEM_TRACK_VERBOSE
			// track allocation count, byte count, and callstack
		#endif

		// return memory pointer to use
	#endif
} 	

//------------------------------------------------------------------------
void TrackedFree( void* ptr )
{
	// TODO - account for cases above; 
}

//------------------------------------------------------------------------
void TrackAllocation( void* allocation, size_t byte_count )
{
	#if defined(MEM_TRACKING)
		// TODO
		// ie, only do work if a MEM_TRACKING mode is defined (may need more conditions
		// based on mode)
	#endif
}

//------------------------------------------------------------------------
void UntrackAllocation( void* allocation )
{
	// TODO
}

//------------------------------------------------------------------------
size_t MemTrackGetLiveAllocationCount()
{
	// TODO
}

//------------------------------------------------------------------------
size_t MemTrackGetLiveByteCount()
{
	// TODO 
}

//------------------------------------------------------------------------
void MemTrackLogLiveAllocations()
{
	// TODO
}


//------------------------------------------------------------------------
// overload new and delete to use tracked allocations
void* operator new( size_t size )
{
   return TrackedAlloc(size); 
}

//------------------------------------------------------------------------
void operator delete( void* ptr )
{
   TrackedFree(ptr); 
}

