## Prelim
Have a `PROFILE_LOG_SCOPE` macro in your code base before your start this - as will want to get some timings; 


## Current Init State Example
```cpp

// simple - just know when we want to move on
bool InitState::IsLoaded()
{
   // assume bool InitState::m_isLoaded starts false; 
   return m_isLoaded; 
}

// list all resources we want to load, after we're done, mark ourselves as loaded; 
bool InitState::LoadResources()
{
   Texture::GetOrAcquire( "somebigtexture0.png" ); 
   Texture::GetOrAcquire( "somebigtexture1.png" ); 
   Texture::GetOrAcquire( "somebigtexture2.png" ); 
   Texture::GetOrAcquire( "somebigtexture3.png" ); 
   Texture::GetOrAcquire( "somebigtexture4.png" ); 
   Texture::GetOrAcquire( "somebigtexture5.png" ); 
   Texture::GetOrAcquire( "somebigtexture6.png" ); 

   m_isLoaded = true; 
}

void InitState::Begin()
{
   // do nothing - want to load
   m_isLoaded = false; 
   m_renderedFrame = false; 
}

void InitState::Update()
{
   if (m_renderedFrame && !m_isLoaded) {
      LoadResources(); 
   }

   if (m_isLoaded) {
      Game::SetState( GAME_STATE_MAINMENU ); 
   }
}

void InitState::Render()
{
   // Render the loading screen
   // ...

   m_renderedFrame = true; 
}

void InitState::End()
{
   // nothing really
}
```


## Cleanup
- Generalize to make this easier to update?


## Threading it

What is the work involved in loading a texture?  Where does it need to run?
- 
- 
- 

### Prelinary Work
Before we can thread this - best to break up our threaded pieces in a single-threaded environment to make sure they work.  

*Side Note: Would recommend profiling these pieces now.  From what I remember, there are some easy wins here.*
-
- 
- 


### Starting the Threads
**Producer-Consumer Paradigm** - We are the producer, and we're goign to make consumer threads to do our work for us;

*Note: We can break this down into four sub-problems.  All with slightly different algorithms to efficiently communicate with them;  This has to do with the number of threads/nodes producing work vs the number of threads/nodes that are consuming work;*
- **SPSC** :: *Single Producer - Single Consumer* : Pipes/Sockets.  Not common in single machine scenarios.  One such example would be if you have a render thread -vs- update thread split.  In Windows, input is registered on the Render Thread (where the Window lives), but processed on the Update thread, so we will *pipe* information from the render thead to update thread to be used; 
- **SPMC** :: *Single Producer - Multiple Consumer* : Our current problem - main thread wants to do a lot of things, and we spin up many threads to get divy up the work; 
- **MPSC** :: *Multiple Producer - Single Consumer* : A logging thread.  Lots of threads want to log, but only one thread should write to the file so the lines don't get intermingled;  Very common in dispatcher style situations (the point-man for an API - ie, all physics commands may go through a single consumer, who will then dispatch them to their underlying subsystems).
- **MPMC** :: *Multiple Producer - Multiple Consumer* : A generic job system.  Any thread can create a job, and multiple works are available to work on that job.  Our `AsyncQueue` is written in such a way to work for this case (and thus every other case).

- How Many: 
- How do we tell when they're done:
- What are they doing:


## Getting Them Information
`AsyncQueue`

**Atomic Operations** - A piece of non-interruptable (exclusive) work;  

- "...an atomic operation is one which cannot be (or is not) interrupted by concurrent operations..." -[Wikipedia](https://en.wikipedia.org/wiki/Linearizability)


`std::queue`'s interface is inherintly non-atomic;  Why?


## Leaving the State

Knowing the threads are done?
- `join` way
- `counter` way
  - `std::atomic<int>` usage; 

## Extra
- Making our Console thread-safe
- Making a generic threaded loader as part of Engine (persistant consumer)
  - `Semaphore` or similar notification primitive required; 
