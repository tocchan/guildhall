Instrumented Profiler
======

## Checklist
- [ ] 25pts: `Allocator`
    - [ ] 02pts: `new` that takes an allocator to use
    - [ ] 02pts: `TrackedAllocator`
    - [ ] 01pts: `UntrackedAllocator`
    - [ ] 15pts: `BlockAllocator` Implemented
    - [ ] 05pts:  `ObjectAllocator` extension of `BlockAllocator` Implemented
- [ ] 40pts: Profiler Implemented
    - [ ] 20pts: Profiler Single-threaded Functionality
    - [ ] 10pts: Profiler Mutli-threaded Functionality 
    - [ ] 03pts: PROFILE_SCOPE( tag )
        - [ ] 02pts: PROFILE_FUNCTION()
    - [ ] 05pts: Profiler/Memory Hookup
- [ ] 20pts: Profiler Reports
    - [ ] 15pts: `profile_report` console command (see below)
    - [ ] 05pts: Also log reports for thread trees that overlap that frame 
- [ ] 15pts: Profiler Visualization (Widget)
    - [ ] 05pts: x-frame, y-time view (frame selector)
        - [ ] 03pts: Clicking the profiler will show the report for that frame; 
    - [ ] 02pts: Can pause/resume by hotkey
    - [ ] 02pts: Report for selected frame
        - [ ] 01pts: Auto pause when selecting a frame
        - [ ] 01pts: Can change report view at run-time
        - [ ] 01pts: Can change sorting at run-time (at least self vs total)

### Extras
- [ ] `ProfileFrame` - Will resume the profiler for a single frame, and autopause profiling when that 
      frame is done.  (if already recording, pause after the *next* complete frame, not current frame)
- [ ] `ProfilerWidget` - timeline view
  - [ ] Also show threads recorded during during this time 
- [ ] `ProfilerWidget` - memory view if verbose memory is enabled
  - [ ] show byte-count at end of frame if verbse - scaled by highwater; 
      - [ ] when selected, show list of new alive allocations w/ callstack in verbose mode
  - [ ] show alloc-count at end of frame if non-verbose, highwater is 'total' allocations
  - [ ] highwater and current count is displayed at end of graph

## Instrumented Profiler
For points - must support at least the following methods; 

```cpp
bool ProfilerSystemInit(); 
void ProfilerSystemDeinit(); 

void ProfilerSetMaxHistoryTime( double seconds ); // cleanup trees that are older than seconds 

void ProfilerPause(); 
void ProfilerResume();

void ProfilePush( char const* tag );      
void ProfilePop();                      

void ProfileBeginFrame( char const* context_name = "frame" );  
void ProfileEndEnd();  
```

The system being only callable from the main thread safely gets you single-threaded points, otherwise you get 
the multi-threaded version. 

### RAII Macros
`PROFILE_SCOPE` and `PROFILE_FUNCTION` are macros that wrap `ProfilePush` and `Pop`.

Example use case; 

```cpp
void Game::Update()
{
    PROFILE_SCOPE( "Game::Update" ); // ProfilePush("Game::Update"); 

    // do stuff in update
    {
        PROFILE_SCOPE("Physics"); // ProfilePush("Physics");  
        Physics::Update(); 
        // ProfilePop() for "Physics" happens here due to leaving scope
    }

    // ProfilePop() for Game::Update happens here due to leaving scope
}

void Game::Render()
{
    PROFILE_FUNCTION(); // Same as PROFILE_SCOPE("Game::Render"); 

    // render...

    // ProfilePop(); happens here due to leaving scope; 
}
```

`PROFILE_FUNCTION` is just `PROFILE_SCOPE`, using the bulit-in macro `__FUNCTION__` to get the name of the function 
or method you are currently in; 


### Thread Safety
Concepts used for this assignment; 

- using `std::thread::id` to identify where work is coming from; 
- A thread-safe `ObjectAllocator` for allocating/freeing nodes; 
- `thread_local` storage qualifier to access the current threads active tree
- `shared_mutex` or reader-writer locks for accessing node history
- reference counting for tracking when a tree can be destroyed
- `atomic` or `Interlocked` operations for tracking references

We will also be using in-place linked lists again to prevent the need to allocating new memory for a node; 

## Reports

You must support a console command, such as `profile_report` that takes the following parameters; 

- `frame_index`:  Defaults to 0.  Which frame of history to present.  0 means the most recent. 
- `view`:  Can either be `flat` or `tree`, defaulting ot `tree`.  
- `sort`:  Defaults to `total`.  Must support at least `total` and `self`.  Other options are `calls`, `memory`
- `order`:  Defaults `desc` for descending.  Can also support `asc` for ascending. 

All reports must include the following columns;
- `LABEL`:  Label used when pushing
- `CALLS`:  How many in a label group at this level
- `TOTAL%`:  What % of the frame was spent in this label, including children
- `SELF%`:  What % of the frame was spent in this label, but not my children

Optional columns that can be useful; 
- `TOTAL TIME`:  Actual time spent
- `SELF TIME`: Actual time spent, not including children
- `AVG TIME`: Average time per call. 
- `AVG SELF TIME`: Average time per call, not including children. 

If memory tracking is enabled, the following columns are also desired
- `ALLOC COUNT`: number of allocations while this node was active (includes children)
- `FREE COUNT`: number of frees while this node was active (includes children)
- `ALLOC BYTES`: number of bytes allocated while active (including children)
- `FREE BYTES`: number of bytes freed while active (including children) 


### Tree View
A tree view report will clump similar nodes only if they are siblings, and will also show the relationship to the 
parent by indentation.  Useful for figuring out which part of the frame is taking the most time.  

This example is sorted by `self`

```
LABEL                                                CALLS    TOTAL%   TOTAL TIME         SELF%    SELF_TIME       
 Game::run_frame                                     1        100.00%  15.5460 ms         0.18%    28 us          
  Game::render                                       1        99.76%   15.5080 ms         1.75%    271 us         
   RHIOutput::present                                1        20.28%   3.1520 ms          20.28%   3.1520 ms      
   ProfilerView::render                              1        76.38%   11.8740 ms         11.60%   1.8030 ms      
    ImmediateRenderer::draw_text2d                   37       64.78%   10.0700 ms         57.23%   8.8970 ms      
     FontAddMeshForText                              37       7.55%    1173 us            1.85%    287 us         
      FontAddMeshForTextInfo                         37       3.83%    596 us             3.83%    596 us         
      TextStyle::get_text_info                       37       1.87%    290 us             1.87%    290 us         
   DevConsole::render                                1        1.35%    210 us             0.66%    103 us         
    ImmediateRenderer::draw_text2d                   3        0.65%    101 us             0.45%    70 us          
     FontAddMeshForText                              3        0.20%    31 us              0.06%    9 us           
      FontAddMeshForTextInfo                         3        0.09%    13 us              0.09%    13 us          
      TextStyle::get_text_info                       3        0.05%    8 us               0.05%    8 us           
    TextStyle::get_text_info                         1        0.03%    4 us               0.03%    4 us           
  Game::update_sim                                   1        0.06%    9 us               0.06%    8 us           
   Game::process_input                               1        0.00%    0 us               0.00%    0 us    
```

### Flat View
Flat view is used to figure out what specific label is taking the most time.  Useful when a function is called from many
places in code. 

This example is sorted by `self`

```
LABEL                                               CALLS    TOTAL%   TOTAL TIME         SELF%    SELF_TIME       
RHIOutput::present                                  1        60.37%   10.1780 ms         60.37%   10.1780 ms     
ImmediateRenderer::draw_text2d                      58       25.39%   4.2800 ms          12.84%   2.1650 ms      
ProfilerView::render                                1        28.01%   4.7220 ms          10.52%   1.7730 ms      
FontAddMeshForTextInfo                              58       7.17%    1209 us            7.17%    1209 us        
FontAddMeshForText                                  58       12.55%   2.1150 ms          2.81%    473 us         
TextStyle::get_text_info                            59       2.64%    445 us             2.64%    445 us         
Game::render                                        1        99.66%   16.8040 ms         2.47%    416 us         
DevConsole::render                                  1        8.82%    1487 us            0.84%    142 us         
Game::run_frame                                     1        100.00%  16.8610 ms         0.31%    51 us          
Game::update_sim                                    1        0.03%    4 us               0.02%    4 us           
Game::process_input                                 1        0.00%    0 us               0.00%    0 us     
```