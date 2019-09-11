## Types of Memory

- Static
  - Code - Constant (`constexpr/const/literals`) Globals
  - Data - Editable Globals
- Dynamic
  - Heap - `malloc/free`
  - Free-store - `new/delete`
- Stack

## Tracking

- Third Party Tool 
  - ie: Visual Studio, though there are othe roptions
  - Gives good information
  -
- Overloading `new/delete`
- Hook `malloc/free`
- Create a new `alloc/free` your engine uses
  - This is common for custom engines/third-party libraries
  - You only track what your program uses (usually good enough - you can trust a third-party)


## Tracking Options

Option 1 - Map

```cpp
std::atomic<uint> gAllocCount = 0; 

struct mem_track_info_t 
{
   void* original_pointer; 
   size_t byte_size; 
   Callstack callstack; 
}; 

std::mutex gTrackerLock; 
std::map<void*,      // key
   mem_track_info_t, // value
   std::less<void*>, // less to use for map
   untracked_allocator<std::pair<void* const,mem_track_info_t>> // allocator  
   > gMemTrackers; 

// untracked_allocator is something YOU need to write; 


// takes memory you got back from ::malloc or new
void TrackAllocation( void* ptr, size_t byte_count )
{
   #if defined(MEM_TRACKING)
      // get callstack - I can do that here; 
      #if MEM_TRACKING >= MEM_TRACK_ALLOC_COUNT
         ++gAllocCount; 
      #endif

      #if MEM_TRACKING >= MEM_TRACK_VERBOSE
         mem_track_info_t info; 
         // get callstack
         // from a01...

         // get a hash of the callstack... somehow...
         // windows function will return it if you want to 
         // pass an optional out variable; 

         // save size

         // put this into map
         // gTrackerLock.lock(); 
         {
            std::scoped_lock lock(gTrackerLock); 
            gMemTrackers[ptr] = info; 
         }
         // gTrackerLock.unlock(); 
      #endif
   #endif
} 

void UntrackAllocation( void* ptr )
{
   // assuming correct defines
   // how do I remove from the map?

   // 1. Remove from map (must be thread safe)
   // TODO: Make this step thread safe
   gMemTrackers.erase( ptr ); 

   // 2. Remove from total size;  (thread safe - use an atomic variable)
   // TODO...

   // 3. Remove alloc count;     (thread safe - use an atomic operation)
   // TODO...
}
```

### Option 2

Store as meta data

```cpp
struct alloc_header_t 
{
   Callstack callstack; 
   size_t byte_size; 
   alloc_header_t* prev; 
   alloc_header_t* next; 
}


static alloc_header_t* gAllocList = nullptr; 

void* TrackedAlloc( size_t size )
{
   size_t real_size = size + sizeof(alloc_header_t); 
   void* mem = UntrackedAlloc( real_size ); 

   alloc_header_t* header = (alloc_header_t*) mem; 
   // fill the header
   // ...

   // lock the list
   // insert into list
   // unlock list; 

   return header + 1; 
}
```