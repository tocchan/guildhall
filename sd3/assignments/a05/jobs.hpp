class Job; 


class JobCategory
{
	public:
		void Enqueue( Job* job ); 
		Job* TryDequeue(); // return nullptr if not job is ready
		Job* Dequeue(); // returns nullptr only if system is shutdown, otherwise block until a job is ready

		void EnqueueFinished( Job* job ); 
		Job* TryDequeueFinished(); 

	public:
		// int m_queueId; 
		AsyncQueue<Job*> m_pendingQueue; 
		AsyncQueue<Job*> m_finishQueue; 
};


enum eJobCategoryType : int
{
	JOB_GENERIC	= 0, 
	JOB_MAIN,
	JOB_RENDER,

	JOB_CATEGORY_CORE_COUNT, 
};

class Job
{
	public:
		using finish_cb = std::function<void(Job*)>; 

		virtual void Execute() = 0; 

		void AddSuccessor( Job* job ); 
		void AddPredecessor( Job* job );

		// support at least one; 
		void SetFinishCallback( finish_cb cb );
		void SetFinishEvent( std::string const& evt ); 


	private:
		bool TryStart(); 
		void FinishJob(); 

		int 				m_category = JOB_GENERIC; 

		std::vector<Job*> 	m_successors; 
	 	std::atomic<int> 	m_predecessorCount; 

	 	// Options - support at least one callback version
	 	finish_cb 			m_finishCallback; 
	 	std::string 		m_event; 

};

class JobSystem
{
	public:
		// negative means as many as you can MINUS the value (8 cores, -1 is 7... -2 would be 6.  -20 would be 1)
		// MINIMUM 1 unless explicitly saying 0; 
		void Startup( int numGenericThreads = -1, int numCategories = JOB_CATEGORY_CORE_COUNT ); 
		void Shutdown();

		void Run( Job* job ); 

		int ProcessCategoryForMS( int category, uint ms ); // process until no more jobs, or until 'ms' has passed 
		int ProcessCategory( int category ); // process until no more jobs, return number of jobs executed
		void FinishJobsForCategory( int category ); 

	private: // probably internal to the CPP (here for reference)
		std::vector<JobCategory> m_categories; 
		std::vector<std::thread> m_genericThreads; 

		bool m_isRunning; 
};

extern JobSystem* g_theJobSystem;  



