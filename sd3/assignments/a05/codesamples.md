Code Samples
======

## In Class Notes


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