#define MEM_TRACK_ALLOC_COUNT 	0
#define MEM_TRACK_VERBOSE		1

// Have MEM_TRACKING undefined means no tracking happens
#if defined(_DEBUG)
	#define MEM_TRACKING MEM_TRACK_VERBOSE
#elif defined(_RELEASE)
	// #define MEM_TRACKING MEM_TRACK_ALLOC_COUNT
#endif


// take a byte count and conver to a more human-parseable
// string 
// ex...
// 123 B would return "123 B"
// 1234 B would return "1.234 KB"
// 123456 B would return "123.456 KB"
// 12345678 B would return "12.346 MB"
std::string GetSizeString( size_t byte_count ); 

// allocations that mem tracking will ignore
// thin wrapper around ::malloc and ::free
void* UntrackedAlloc( size_t byte_count ); 
void UntrackedFree( void* ptr ); 

// allocations will be tracked (if the config is set)
// use this instead of "malloc" and "free" 
void* TrackedAlloc( size_t byte_count ); 
void TrackedFree( void* ptr ); 

// tracking - work is conditional based on config
void TrackAllocation( void* allocation, size_t byte_count ); 
void UntrackAllocation( void* allocation ); 

// report methods
size_t MemTrackGetLiveAllocationCount(); 
size_t MemTrackGetLiveByteCount(); 
void MemTrackLogLiveAllocations();


