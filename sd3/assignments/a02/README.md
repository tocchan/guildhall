SD3.a02
======

## Grading

- [ ] 02%:`UntrackedAlloc` and `UntrackedFree`
- [ ] 08%:`TrackedAlloc` and `TrackedFree`
- [ ] 02%: Overload `new` to use `TrackedAlloc` and `TrackedFree`
- [ ] Memory tracking is a config that allows for...
  - [ ] 04%: Display current mode in dev console
  - [ ] 02%: `nothing`: Disabled Completely (`Tracked` behaves like `Untracked`), no additional info is shown in dev-console
  - [ ] `#define MEM_TRACK_ALLOC_COUNT 0` - Track live allocation count 
      - [ ] 02%: Allocation Count is displayed in the dev console
  - [ ] `#define MEM_TRACK_VERBOSE 1` - Track live byte/callstack count 
      - [ ] 06%: Display byte count in dev console (see notes on format)
      - [ ] 04%: Dev console command to log live callstacks
      - [ ] 05%: Mem tracking uses only `Untracked` allocation under the hood (so as to not infinite loop)
- [ ] 05%: `MemTrackGetLiveAllocationCount()` implemented (return 0 if memtracking is disabled)
- [ ] 10%: `MemTrackGetLiveByteCount()` implemented (return 0 if byte counting is disabled)
- [ ] 20%: `MemTrackLogLiveAllocations()` implemented (does nothing if not tracking)
  - [ ] 05%: When logging, group by callstack (hashing may help this, but could also use `memcmp`)
  - [ ] 10%: When logging, sort by largest to smallest group
- [ ] 15%: Memory tracking is thread-safe (the below test doesn't error over multiple runs)

### Extra
Extra is not worth any points - just here as potentially useful metrics/tools

- [ ] Keep track of memory high-water mark (max memory used), and be able to report this
- [ ] Be able to tag memory (default to nullptr) to help identify which system is taking the memory (tracked)
    - [ ] Supply an overloaded new that takes this tag as a parameter
    - [ ] Update report functions to take an optional tag. 

## Notes

### Printing Values
```cpp
// Recommend a function signature such as...
std::string GetSizeString( size_t byte_count ); 
```

When printing numbers, use the largest suffix that allows a full number, 
and three points past the decimal point.  

- *b: Bit - not used in this assignment
- *B*: Byte (8 bits)

- *KiB*: Kibibyte - 1024 B (2^10 B)
- *MiB*: Mebibyte - 1024 KiB (1024 * 1024 B or 2^20 B)
- *GiB*: Gibibyte - 1024 MiB (1024 * 1024 * 1024 B or 2^30 B)

- *KB*: Kilobyte - 10^3 B
- *MB*: Megabyte.  10^6 B (or 1000 KB)
- *GB*: Gigabyte.  10^9 B (or 1000 MB)

*Note: Windows will use KB to refer to a KiB, MB to mean MiB, and so on. 
Generally just being very confusing.*

I don't care which suffix style you use, just be sure to use the largets
available; 

For example; 
- 233 B would print as `233 B`
- 2492 B would print as `2.492 KB`
- 321930 B would print as `321.930 KB`
- 123456789 B would print as `123.347 MB`
- and so on...

### Links



**Hashing** 
- https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
  - First response comparing hash algorithms, mostly there as a place to look into other hash functions if you're curious.  
- CRC-32 Implementations: https://create.stephan-brumme.com/crc32/
  - You can use the callstack returned hash, or hash it yourself using any hash function.  CRC-32 is typically fast and appendable; 


## Grading & Unit Test

The below unit test should work; 

On top of that I will be asking you to...
- Disable MEM_TRACKING, compile, and show me that no work is being done (all methods report 0)
- Fast tracking (`ALLOC_COUNT`) configuration works (allocation count is shown, but all other values report 0, no real slowdown)
- `VERBOSE` mode enabled - potentially a drop in performance;  `A02 Test` should definitely be running slower.  All numbers look correct with no errors.



```cpp
// Engine/Config.hpp or Engine/Mem.hpp

// ... 
#define MEM_TRACK_ALLOC_COUNT 0  // Just logs total number of "live" allocations
#define MEM_TRACK_VERBOSE     1  // If TRACK_MEMORY is also true, this will also track callstacks/bytes for live allocations

// Which mode do I want - comment out if nothing
#define MEM_TRACKING MEM_TRACK_VERBOSE
```

```cpp

// in your config file - can define one or both of these
// "allocator" that doesn't track
void* UntrackedAlloc( size_t size ) { return ::malloc(size); }
void UntrackedFree( void* ptr )  { ::free(ptr); }

// todo: allocator that will track if set to; 
void* TrackedAlloc( size_t size )   { /*IMPLEMENT ME*/ return nullptr; }
void TrackedFree( void* ptr )       { /*IMPLEMENT ME*/ }

// todo: custom C++ "untracked" allocator
// so we can make an untracked map (or however you 
// want to track verbose data)
void TrackAllocation( void* ptr, size_t size ); 
void UntrackAllocation( void* ptr ); 

#if defined(MEM_TRACKING)
   #define MEMTEST_ITER_PER_THREAD 1'000'000
   #define MEMTEST_ALLOC_BYTE_SIZE 128

   static void AllocTest( ThreadSafeQueue<void*>& mem_queue, std::atomic<uint>& running_count )
   {
      for (uint i = 0; i < MEMTEST_ITER_PER_THREAD; ++i) {
         // (Random01() > .5f) or however your random functions look
         if (RandomCoinFlip()) {
            byte* ptr = (byte*) TrackedAlloc( MEMTEST_ALLOC_BYTE_SIZE ); 

            // just doing this to slow it down
            // (and later, to confirm memory didn't get currupted)
            for (uint i = 0; i < MEMTEST_ALLOC_BYTE_SIZE; ++i) {
               ptr[i] = (byte)i; 
            }

            mem_queue.enqueue( ptr ); 
         } else {
            void* ptr; 
            if (mem_queue.dequeue(&ptr)) {
               TrackedFree(ptr); 
            }
         }
      }

      // we're done; 
      --running_count; 
   }

   // This test will only work if memory tracking is enabled
   // otherwise the memory tracking just return 0;
   UNITTEST("A02", nullptr, 0) 
   {
      // unittest assumes 
      uint pre_allocations = MemTrackGetLiveAllocationCount();

      {
         PROFILE_LOG_SCOPE("A02 Test"); 
         // scope so queue goes out of scope and we
         // get those allocations back; 
         ThreadSafeQueue<void*> mem_queue; 
         std::atomic<uint> live_count = core_count;   

         // wpin up that many threads; 
         uint core_count = std::thread::hardware_concurrency(); 
         for (uint i = 0; i < core_count; ++i) {
            std::thread test_thread( AllocTest, mem_queue, count );  
         } 

         while (live_count.load() > 0) {
            // "ms" is a C++ custom literal equivalent 
            // for std::chrono::milliseconds(100)
            // https://en.cppreference.com/w/cpp/chrono/operator%22%22ms
            std::this_thread::sleep_for(100ms); 
         }

         void* ptr; 
         while (mem_queue.dequeue(&ptr)) {
            TrackedFree(ptr); 
         }
      }

      // check we're back to where we started; 
      uint post_allocations = MemTrackGetLiveAllocationCount();

      // if done right, allocations at the start
      // should be allocations at the end; 
      return (pre_allocations == post_allocations); 
   }
#endif

//
```

### Log Live Allocation Example

When you call `MemTrackLogLiveAllocations`, the output depends on the mode you're in;

1. None:  `Memory tracking is not enabled.`
2. Alloc Count:  `%u live allocation(s).`
3. Verbose...

```
=== BEGIN MEMORY LOG === 
2.430 MiB live from 16 allocation(s)...

- 1.400 MiB from 7 allocation(s)
  path/to/file0.cpp(123): Foo
  path/to/file0.cpp(100): PathToTarget
  path/to/main.cpp(40): WinMain

- 800.000 KiB from 2 allocation(s)
  path/to/game.cpp(111): CreateBullet
  path/to/entity.cpp(10): Entity::TryFireProjectile
  path/to/game.cpp(493): Game::UpdateEntities
  path/to/main.cpp(40): WinMain

- 200.000 KiB from 1 allocation(s)
  path/to/game.cpp(80): CreateEntity
  path/to/game.cpp(10): Game::SpawnPlayer
  path/to/game.cpp(120): Game::Startup
  path/to/main.cpp(40): WinMain

- 30.000 KiB from 6 allocations(s)
  path/to/game.cpp(824): SpawnExplosion
  path/to/game.cpp(320): Entity::TakeDamage
  path/to/game.cpp(120): Game::Update
  path/to/main.cpp(40): WinMain

=== END MEMORY LOG ===

```

Import part is they're grouped by similar callstack, and ordered largest to smallest.  
