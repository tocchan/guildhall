Code Samples
======

## In Class Notes
This naming is non-standard - just how I'll refer to things in the notes; 

**Task**: Some work the user wants to done, made up of jobs.  ie: `AsyncSaveTextureToFile`
**Job**: A single part of a task `LoadFromFile`


What does this system have to do?
```cpp

// what is 'main', 'render', 'io', 'ai, etc... categories...
void JobSystemInit( int genericThreadCount, int categoryCount );

void JobProcessForCategory( int category ); 
void JobProcessForCategoryForMS( int category, uint ms ); 


enum eJobCategories 
{
   JOB_GENERIC = 0, 
   JOB_RENDER, 
   JOB_MAIN, 

   JOB_CORE_CATEGORY_COUNT, 
};

enum eGameJobCategories 
{
   JOB_GAME_CATEGORY0 = JOB_CORE_CATEGORY_COUNT, 
};

// Game.cpp
void Game::BeginFrame()
{
   // consume for UP to 5ms
   // if no work, just return...
   JobProcessForCategoryForMS( JOB_MAIN, 5 );
}


```

### What is a job?
- Category
- Priority
- What does it do? (work - function call/callback/... something)
- The data it needs; 
- Dependencies (what do I depend on)
  - Dependants (what jobs run after me)


### How do this to look?

C way; 
```cpp
void SaveImageToFileJob( Job* job )
{
   // where does path and image come from; 
   job_write_image_data_t* data = (job_write_image_data_t*) job->data; 

   // stbi_write
   stbi_write_image( data->path, data->image.get_bytes() );
   delete data; 
}

void AsyncSaveImageToFile( char const* path, Image const& image )
{
   job_write_image_data_t *data = new job_write_image_data_t(); 
   data->path = path;   // std::string (copy)
   data->image = image; // makes a copy

   RunJob( JOB_GENERIC, PRIORITY_LOW, SaveImageToFileJob, data );
}

```

## OOP Way
```cpp

class Job 
{
   public:
      virtual void Execute() = 0; 
      virtual void GetCategory() = 0; 
      virtual void GetPriority() = 0; //  maybe
}; 

class ImageWriteJob : public Job
{
   public:
      virtual void Execute() { /* method from above*/ }

   public:
      std::string path; 
      Image image; 
}; 

void Foo()
{
   // Job system takes this over; 
   JobRun( new ImageWriteJob( path, image ) ); 
}
```


## What is the system doing to support that?
```cpp
void JobRun( Job* job ) 
{
   /*
   - assign it to a *thread*
   - depeendencies
   - get Execute to be called
   */

   // Find my queue, put myself into it; 
   JobQueue* queue = GetQueueForCategory( job->GetCategory() );
   queue->enqueue( job );  
}

bool JobProcessForCategory( int category ) 
{
   JobQueue* queue = GetQueueForCategory( category ); 
   Job* job; 

   if (queue->try_dequeue( &job )) {
      job->Execute(); 

      // dependencies (todo)
      // finished callback?

      delete job; 
      return true; 
   }

   return false; 
}

// generic work thread
static void GenericThreadWorker() 
{
   while (running) {
      // use a semaphore to wait on - and have generic jobs
      // signal the semaphore

      while (JobProcessForCategory(JOB_GENERIC));
      Sleep(0); 
   }
}

// threadCount can be < 0
// if threadCount is less than zero, do max cores + threadCount
// otherwise do threadCount
void JobSystemInit( int threadCount, int categoryCount )
{
   // create your category queues
   // ...

   // figure out thread count
   // always spin up at LEAST 1.
   // spawn those threads
   // ...

   // ThreadAffinity if you're feeling fancy
   // likely not going to do you much good in Windows

}

// JobSystemInit( -1, CATEGORY_COUNT );   // 
```


## Dependencies & Chaining
So just single instance jobs are already very powerful.  


### My Job Spawns Other Jobs
This one is free.  Over the course of a job, if there is a second step (say a load, then a copy to texture), 
you spawn the first job (load) and at the end of the job, it creates and runs the next job.

Note when doing this, that job can run immediately as soon as `JobRun` is called, not when the calling job finishes.  So be
sure the job has all the data it needs to run at this time.  

For example;
```
StreamChunkJob
   while (read 4096B of data)
      create decompress job for that 4096B
```      

So as work is ready, we can kick off a job to process that work immediately.  Once you get chaining, you could also 
chain this new job to a "final" job to


### My Job Re-enqueues
I have a single job, that needs to run different areas.  It may look like the following; 

```cpp
class SomeJob
{
   eSomeJobState m_state; 

   void Execute()
   {
      switch (m_state) {
         case GENERIC_STATE_0:
            DoGeneric0();

            m_state = RENDER_STATE_0;  
            JobEnqueue( this, JOB_RENDER ); 

         case RENDER_STATE_0: 
            DoRender0(); 
            m_state = MAIN_STATE_0; 
            JobEnquuee( this, JOB_RENDER ); 

         case MAIN_STATE_0: 
            FinalMainThreadWork(); 
            break; 
      }
   }
}
```

The underlying structure for this working is interesting from a co-routine implementation standpoint (ie, a job being
put back into a work queue, and then re-starting).  Without co-routines, the structure like above is more likely.

Since the job already has to be split into parts, the benefit isn't very strong over just spawning a second job for 
the second state, while requiring a little extra complexity in the finish code; 

```
JobFinish
   if (have a dispatch count) {
      re-dispatch
   } eles {
      normal job finish code
   }
```

Notice no chaining will happen until the job is fully done (not re-enqueued).  

*Note: I do not support this in my engine - as I prefer splititng it to multiple jobs per place/break point*.


### Pre-setup Dependencies
This is my preferred - mainly due to visibility.  There is one piece of code that you can see all
the pieces being setup as well as the chains, helping you find the individual parts.  

On the downside, the initial job function gets a little busier.  

```cpp
// Discisson on how this work
// How does this change how the system takes values
```


## Checking for "Finished"
If we kick off an async task or job, we likely want to know when it is finished and we can use
the result.  For example, if we ask for an AI path from a pathfinder, we probaly want to use it,
and therefore need to be told when it is ready. 

Two common ways of approaching this are **Callbacks** and **Polling**.  

**Callbacks** we should be familiar with.  This is a callable object (function, funtor, method, etc...) 
that will be invoked upon the job complete.

**Polling** means being able to ask the task/job itself if it is finished.  We can ask or wait on an
operation to check for it being completely (either error or success).   


### Finished Callback
Some people prefer callbacks for when something is finished, but this begs the question of
*where* should the callback be called, and how?

Two common paradigms in engines where callbacks are the notification choice...
1. Callbacks are called from thread the job runs on, and thread safety is left to the caller (less popular)
2. A hook function is provided that must be called, where all events/callbacks will be invoked (more popular)

The reason two tends to be more popular is that one of the major goals of most multi-threaded frameworks is to 
minimize the amount the user has to worry about it being multi-threaded.  A soft rule such as "All jobs are kicked
off from the main thread, and can only work on data provided" would make the entire system "safe" to use, as the 
sync points are both in the main thread of execution. 

```cpp
// At Job Creation
Job* job = new AsyncFileReadJob(); 
job-set_finish_callback( OnReadFinished );

// rest of setup
// ...

JobRun( job ); 
```

```cpp
// Somewhere during your frame on the main therad; 
JobCallFinishedCallbacks(); // hook for callbacks 
```

As stated above - this paradigm fits really well if you are planning on always kicking off jobs from the main thread
only (or at least, only jobs that require callbacks).  We can simply also extend the system with a small modification
to make it more general. 

By allowing a `finish_category`, in which you can enqueue which job group the finish 
callback is meant for, and having a `JobCallFinishedCallbacksForGroup` style function for invoking them.  This works
very similar to the initila job queues, but now each queue can also have a 'finish' queue for when the job was given 
a callback. 



### Polling (Overlapped/Async Operations)
**Polling** means to just *pole* or ask if the job is done - usually the system providing an object in which to ask.  

For Win32 Async calls, they use an `OVERLAPPED` structure.  This is just a structure you provide to the operation, that
you can then ask for when it is finished;  This memory has to be valid for the lifetime of the call. 

In Modern C++ the structures for supporting this are the `std::promise` and the `std::future`.  A promise is something 
needed to be filled (ie, the job itself) and the `std::future` is the thing you can wait on (what you return to the caller).
https://www.modernescpp.com/index.php/promise-and-future

And in our system, we could even just use the `Job` itself as a way to ask if it is done.

```cpp
// in this scheme - the job system no longer is given the job to delete, but
// instead a reference count scheme has to be used to account for the caller holding onto the object; 
Job* job = JobCreate( JOB_GENERIC, AsyncFileReadWork );
JobDispatch( job ); // marks the job as ready to be run; 
```

```cpp
// At some point, somewhere, in the future
if (JobIsDone(job)) {
   // do finish work
   // ...

   JobRelease(job); // release my old on it; 
}
```


## Theory
### Work -vs- Data
My implementation puts the work of the job with the data of the job.  Meaning you reconstruct the job
graph every time you need an instance of that task.  

Toying with the idea of splitting it up so that spinning up a complex job would be faster, though currently
have no strong arguments for it, which is in itself a strong argument against it.  

Think it warrants farther discussion - hopefully get some fresh input.  Probably after implementing the system. 