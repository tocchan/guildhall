

## Topics
- Profiling 
  - Cycles
  - High Performance Counter
    - `GetCurrentTimeHPC();` 
  - "Wall-Time" 
    - `HPCToSeconds()`
  - Sampling Profilers
  - Instrumented Profilers
    - Simple Instrument (Measurement)
      - Time at start - time at finish, log it
      - HPC (why not seconds?)
      - `ProfileLogScope` object
      - `PROFILE_LOG_SCOPE` macro; 

- General Profiling Philosophy
  - Measure twice, cut once.
  - Profile first to identify what is wrong, and proifle after to verify what you did *helped*
    - Don't check in something that made it worse - no matter how much work you put into it (this shoudl go wihtout saying)
  - PCs/OSes have a lot of noise.  Unless measure a large section of code (ie, loading scene), measure something as much as possible.  For example, instead of measuring a sqrt once and timing it, you run the code for 10s, and see how many times you could run sqrt; 
    - That said, you want your profiling to reflect reality.  Unit tests are a good start, but the optimizer may optimize out your test, or optimize thta use case. 
    - Extreme example - no use profiling your GetOrCreateTexture once the texture is already loaded.  It is only the first spike that needs it; 
  - NEVER ASSUME - same skill that can make you a good debugger will make you an excellent profiler
    - Go in assuming as little as possible - let the data tell you where to look
    - Once you make a change, measure to be sure you improved it
    - Minimize optimization while coding something up the first time - you don't need to write optimized code all the time - but you should write optimizable code.  This comes with practice!
      - Again, clean interfaces that ask simple questions help a lot with this!

- Optimization Philosophy
  - Macro/Algorithmic Optimization
    - Deals with the overall approach to a problem
      - Looping over every element -vs- looking at just a subset
      - Searching a sorted vector (looping over the entire array vs divice and conquer)
    - O(n) wins
      - Simplest version is using a more appropraite data structure; 
        `std::map` instead of a `std::vector`
    - Usually requires system refactoring
      - Good interfaces can help with this!
  - Micro Programming
    - Good understanding of hardware/the machine to make the most of it
      - Being cache friendly
      - Pipelining properly
      - Just doing less work; 
    - Usually gives "order of magnitude" or scalar wins
  - Data Oriented Programming (Mike Acton talk is good)
    - Structuring data based on usage & access
    - Can fall under both macro/micro optimization
      - Example: Component System (ECS)
        - Loop over all components and update -vs- looping over only components you care about
  - ex: `Map::GetEntitiesInDisc`
    - First version - loop over all entities
      - Macro Optimization: Using an **acceleration structure** to limit which entities I check.  For example, a quad tree can be used to only check entities that overlap with the circle; 
      - Micro Optimization: Build a list of all entity positions.  Loop and compare just positions, add pointers that pass.  
  - Optimize in Release - unless you're specifically trying to get Debug to run faster
    - An debug optimization will likely improve a release optimization, but it may very well do nothing (ex: release does the same or better optimization than you do), and in some cases worse.  So take your timings in release; 

- Parrallel Programming
  - Threads
    - Why? 
      - use more of the machine
      - can now do more computational intensive work wihtout hindering game experience
        - AI/Pathing ex:  If paths end up taking a long time (even a couple ms), that is time not spent updating your game.  Path results are not critical immediately (you can always just start moving toward your target until you have a path), so you can push paths to a thread and wait for them to come back on later frames. 
    - Hardware Threads -vs- Software Threads
      - Hardware Threads - ???
      - Software Threads - ???
    - If your threads are "busy", it is ideal to have only as many threads as you have hardware cores.  Spinning up a 1000 threads does not make your application 1000x faster.  On the contrary, it will usually end up making your program slower as you are now paying the cost for a thread context switch (a hardware thread loading in a software thread)
  - Race Conditions
    - Deadlock Empire - highly recommend playing with this site to develop an intuition
  - Atomic Operations
  - Locks
    - Amdahl's Law
    - Minimize Lock Time
    - `CRITICAL_SECTION`, `std::mutex`, and **Fair Locks**
    - Other lock types `SpinLock`, `Semaphore`, `ReaderWriterLock` or `std::shared_lock`...
      - `SpinLock` is an unfair lock
      - `Semaphore` supports multiple locks - can be thought of as a shared counter.  useful for signals
      - `Events`, `OverlappedOperations`, `Futures` and `Promises`
        - `Promise` - I will give you this
        - `Future` - this will be something eventually
  - Message Passing Paradigm (GoLang)
    - Starting with a single (yet very powerful) multithreading philosophy
      - I did a large amount of optimization and parallization work - and honestly, message queues tend to solve about 95% of the problems I needed solving.  They are so useful, they are the backbone behind GoLang, a language designed around parallism.  (ErLang is another good example)
      - Example: Doesn't solve shared resource problems (a Resource map for example - many threads may want to add to/remove from a resource database).  We can talk more about these kinds of problems in SD3; 
    - "I give you somtehing, you do work, and then give it back"...
    - If only one person is using it, you don't have to worry about threading;
    - ThreadSafeQueue
      - Problem with `std::queue`
        - Why isn't `STL` thread safe?
      - Atomic Enqueue
      - Atomic Dequeue

- Joining a thread
- Detaching a thread
- Signaling a Thread
- One thread running indefinintely vs a thread per piece of work.
- Thread Affinity

- Optimizing the slow parts
  - Measure the individual parts of the load - is it IO, is it decompression, or is it copying ot texture?
    - Measure each invidually to figure out where your time is going!
      - If it is disk reads - better/different compression would help overall
      - If it is decompression - loading a raw format may help, or using a different format
      - if it is transferring to CPU->GPU, perhaps try a compressed texture format, or generating mips on the fly
    - Example above - for meshes, it is the decoding of the file that will likely be slow.  Creating a custom binary format could speed this up (a bake step)

- Optimization is tricky (what works on my machine may make your machine worse)
  - Example: Disk Reads
    - If using an HDD or other physical media (bluray), the more threads I have accessing the disk, the worse the performance gets, as I'm constantly having to seek to a new read head and I end up "thrashing" the disk.  If I instead serialize all access to the disk through one thread, I'll end up getting much better performance. 
    - If using an SSD, this may be counter productive.  SSDs actually benefit from having multiple IO threads making requests since the SSD itself may be internally parallel


- Game Example : Threading our Resource Loader
  - Verifying it works.  Add something "animated" to your loader
  - Be able to register a loaded resource to your database
  - Do this just for time consuming loads
    - Textures (#1)
    - Meshes (#2)
    - Shaders (maybe)
  - Theoritical BEST case is we get 8x savings
  



```cpp
// Note:  I don't usually use std::thread, but instead use windows or pthread functions.  
// So be aware there might be some problems; 

// Timing Functions
uint64_t TimeGetPerformanceCounter()
{
   uint64_t hpc;
   ::QueryPerformanceCounter((LARGE_INTEGER*)&hpc);
   return hpc;
}

// Profiling Functions
double HPCToSeconds( uint64_t hpc ) 
{
   // sometime during an init
   // static gSecondsPerHPC;
   // uint64_t hpc_per_second; 
   // ::QueryPerformanceFrequency((LARGE_INTEGER*)&hpc_per_second);
   // gSecondsPerHPC = 1.0 / (doublehpc_per_second; 

   return (double)hpc * gSecondsPerHPC;
}

// Talk about local static initialization - and why *not* to do it here; 


// Thread Functions
void ThreadWork( char const *str )
{
   // by the time I get here, I have almost NO guarantees about where
   // my calling thread will be in execuation; 

   // I'm running on a thread!!!
   DebuggerPrintf( "")
}

void ThreadTest()
{
   // creates a thread
   std::thread t0( ThreadWork, "Hello World" ); 

   // There are then 3 possible ways to handle the thread object
   // - join
   // - detach
   // - terminate (don't do this unless you have a VERY good reason)

   // I will not pass this line until the thread is finished
   // I should not use t0 after this line; 
   t0.join();


   // I do not wait (I effectively disown the thread - t0 should not be used after this point) 
   t0.detach(); 

   // by default, if you do not join/detach a thread, the thread will "terimate" when the thread goes out of scope; 
   // This should be considered an error - so always be sure to detach or join; 

}

// Mutex Functions

```