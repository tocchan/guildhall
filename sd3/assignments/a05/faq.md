FAQ
======

Q. What are the categories again?
A. Determine where/when the job gets run.  Also referred as **job queues**.  

Q. Why use intergers for categories?
A. Speed!  No string comparisons.  Don't need to hash it; Also konwn at compile time. Instant access if
   you use natural numbers (0, 1, 2, 3, ...), which I highly recommend. 

Q. How could we sleep/signal a queue;
A. Semeaphore or <sad>condition variable</sad>... 
```cpp
class JobCategory
{
	public:
		void Enqueue( Job* job )
		{
			m_pending_queue.enqueue( job ); 
			m_pending_cv.notify_one(); 
		}

		Job* BlockingDequeue()
		{
			Job* ret = nullptr; 
			std::unique_lock lk( m_lock ); 

			// may also need to check that the thread is running
			m_pending_cv.wait( &lk, [&]() { return m_pending_queue.dequeue(&ret); }  );

			return ret; 
		}

		Job* TryDequeue()
		{
			Job* job = nullptr;
			m_pending_queue.dequeue( &job ); 

			return job; 
		}

	public:
		// int m_queueId; 
		AsyncQueue<Job*> m_pending_queue; 
		AsyncQueue<Job*> m_finish_queue; 

		// a lock
		// a conditional variable
};

```

## Examples
- Read a level from a database and load it into a game (from a web server)


### Saving to Screenshot


```cpp
void AsyncSaveScreenshot( std::string const& filename, Texture* textureToSave )
{
	Texture* stagingTexture = new Texture(...); // same as textureToSave
	Copy textureToSave to staging; 

	// create some job data struture to pass data around; 
	job_data* data = new job_data();
	data.image = nullptr; 
	data.staging = stagingTexture; 
	data.filename = filename; 

	JobRun( JobCopyTextureToImage(data) ); 
}

void JobCopyTextureToImage::Execute() 
{
	// allocate space in image for texture
	// map the staging texture
	// copy to image
	// unmap
	// delete staging
	JobRun( new JobSaveImage(data) ); 
}

void JobSaveImage::Execute() 
{
	// save image to file (stbi_write)
	// call finished callback if you want (or have this be the job you return)
}
```