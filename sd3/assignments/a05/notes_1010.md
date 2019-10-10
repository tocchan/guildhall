Chaining - Spin up job

```cpp
void JobLoadTexture()
{
	Image *image = new image(); 
	image->load( filename ); 

	JobRun( new JobCopyImageToTexture( image, texture ) ); 
}
```


```cpp

// imlementing a graph
class Job
{
	std::vector<Job*> m_successors; 
	std::atomic<int> m_num_predecessors; 

	Job() 
	{
		m_num_predecessors = 1; 		
	}

	void finish()
	{
		foreach (successor) {
			successor->try_to_start(); 
		}

		if (m_callback != nullptr) {
			JobSystem::EnqueueFinished( this ); 
		} else {
			delete this; 
		}
	}

	bool try_to_start()
	{
		int new_value = --m_num_predecessors;
		ASSERT( new_value >= 0 ); 

		if (0 == new_value) {
			enqueue(); 
			return true; 
		}

		return false; 
	}

	void add_successor( Job* job )
	{
		job->add_predecessor( job ); 
	}

	void add_predecessor( Job* job )
	{
		m_num_predecessors++; 

		{
			lock it;
			job->m_successors.push_back( this ); 
		}
	}
}; 


void JobRun( Job* job ) 
{
	job->try_to_start(); 
}

// somewhere in the main thread - where it is save to modify the state; 
// BeginFrame
void JobSystemCallFinishedCallbacks()
{
	foreach (finished) {
		// call callback
		// delete job; 
	}
}

// Adding Children
void MergeSort( T* array, size_t size )
{
	size_t half_size = size / 2; 
	SortJob* s0 = new SortJob( array, half_size ); 
	SortJob* s1 = new SortJob( array + half_size, size - half_size ); 

	MergeJob* merge = new MergeJob( array, half_size, 
		array + half_size, size - half_size ); 

	// second option
	merge->add_predecessor( s0 ); 
	merge->add_predecessor( s1 ); 
	JobRun( s0 ); 
	JobRun( s1 ); 

	// Setup chains BEFORE running jobs; 
	JobRun( merge ); 

	// JobRun( s0, s1, merge ); 
}

template <typename ...ARGS>
void JobRun( Job* job0, Job* job1, ARGS ...args )
{
	JobRun( job0 ); 
	JobRun( job1, args... ); 
}

class Entity 
{
	void Path()
	{
		PathJob* job = Pather::CreatePathJob(); 
		job->set_callback( [=]() { this->apply_path( job->path ); } ); 

		JobToRun( job ); 
	}

	void apply_path( Path* path ) 
	{
		// copy the path to my entity (it will be desroyed otherwise)
		// ...
	}

}

void Game::BeginFrame()
{
	JobSystem::ProcessFinishedQueue(); 

	JobSystem::ProcessFinishedQueue( JOB_MAIN ); 

}


void Game::DoAJob( bool* opt_finished )
{
	while (!finished) {
		WaitForJob( job, JOB_MAIN ); 
	}	
}

void JobWait( Job* job, int categoryOfCallingThread )
{
	while (!JobIsDone(job)) {
		JobProcessPendingQueue( categoryOfCallingThread ); 
		JobProcessFinishedQueue( categoryOfCallingThread ); 
	}
}
```