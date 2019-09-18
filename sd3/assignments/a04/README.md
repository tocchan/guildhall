Instrumented Profiler
======

## Checklist
- [ ] 05pts: `Allocator` base class
    - [ ] `new` that takes an allocator to use
    - [ ] `TrackedAllocator`
    - [ ] `UntrackedAllocator`
- [ ] 20pts: `BlockAllocator` Implemented
    - [ ]: 10pts `ObjectAllocator` Implemented
- [ ] 40pts: Profiler Implemented
    - [ ] Profiler Processing Thread
    - [ ] Profiler Single-threaded Functionality
    - [ ] Profiler Mutli-threaded Functionality 
    - [ ] PROFILE_SCOPE( tag )
        - [ ] PROFILE_FUNCTION()
    - [ ] Profiler/Memory Hookup
- [ ] 20pts: Profiler Reports
- [ ] 15pts: Profiler Visualization (Widget)

## Allocator
So we'll treat allocators as objects we'll pass around, with a specific kind of allocator being the algorithm for giving out or breaking up a given block; 

## Block Allocator

### ObjectAllocator
This is a specialization of a `BlockAllocator` that allocates only one type of object.  It is just an excercise in templates. 

## Instrumented Profiler



### The Consumer

### The Producers

### Threaded Version
 
### Add Measurements for Memory