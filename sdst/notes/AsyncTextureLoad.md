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

## Goal

```cpp

// AsyncQueue<ImageLoadWork*> InitState::m_load_queue; 
// AsyncQueue<ImageLoadWork*> InitState::m_finish_queue; 

// This is THE thread; 
static void ImageLoadThread( InitState *state )
{
   ImageLoadWork* work; 

   while (!state->IsFinished()) {
      while (state->load_queue->dequeue(&work)) {
         work->image.load_from_file( work->image_name );  
         state->finish_queue.enqueue( work ); 
      }
   }
}

static void PrintMeLots( std::string str, int count )
{
   for (int i = 0; i < count; ++i) {
      printf( str.c_str() ); 
   }
}

struct ImageLoadWork 
{
   std::string image_name; 
   Image image;  
};

void InitState::StartLoadingTexture( InitState* state );
{
   ImageLoadWork* work = new ImageLoadWork(); 
   work->image_name = filename; 

   ++m_objectLoading; 
   m_load_queue->enqueue( work ); 
}

// std::vector<std::thread> InitState::m_threads; 

void InitState::Begin()
{
   StartLoadingTexture( "somebigtexture0.png" ); 
   StartLoadingTexture( "somebigtexture1.png" ); 
   StartLoadingTexture( "somebigtexture2.png" ); 
   StartLoadingTexture( "somebigtexture3.png" ); 
   StartLoadingTexture( "somebigtexture4.png" ); 
   StartLoadingTexture( "somebigtexture5.png" ); 
   StartLoadingTexture( "somebigtexture6.png" ); 

 uint core_count = std::thread::hardware_concurrency(); 
   for (uint i = 0; i < core_count; ++i) {
      // Things we have to do still
      std::thread loadThread = std::thread( ImageLoadThread, this ); 
      // loadThread.detach(); // not bad, will work kinda - rare bug will happen (remind me later)
      m_threads.push_back( loadThread );

      // loadThread.join();   // bad -> I would deadlock
   }
}

void InitState::FinishReadyTextures()
{
   LoadImageWork* work; 
   RenderContext *rc = App::GetRenderContext(); 
   while (m_finish_queue->dequeue( &work )) {
      Texture2D *tex = new Texture( rc, work->image );
      rc::RegisterTexture( work->name, tex ); 
      delete work; 

      --m_objectLoading;       
      ASSERT( m_objectLoading >= 0 ); 
   }
}

void InitState::Update()
{
   FinishReadyTextures(); 
   if (IsFinished()) {
      Game::SetState( GAME_STATE_MAINMENU ); 
   }
}

bool InitState::IsFinished() 
{
   return (m_objectLoading == 0); 
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
- Load File
- Decompress File (PNG/JPG/etc) to Byte Buffer (if you're Colin, to Image... for reasons???)
- Byte Buffer to Texture
- Register in our Database

### Preliminary Work
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
