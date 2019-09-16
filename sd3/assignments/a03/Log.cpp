// Engine
void EngineStartup() 
{
	// mem/track/debug init
	// may need to mkdir from code - std::filesystem yay!
	LogSystemInit( "log/current.txt" );

	// other systems 
}

void EngineShutdown()
{
	// shutdown common

	LogSystemShutdown(); 
}

class LogSystem
{
	public:
		std::thread m_thread; 
		std::string m_filename; 

		// other things this need
		// if just async queue
		AsyncQueue<log_object_t*> m_messages; 
		Semaphore m_signal; 

		void wait_for_work() 		{ m_signal.acquire(); }
		void signal_work()			{ m_signal.release(1); }

	public:
		bool m_is_running				= true; 

		bool is_running() const 	{ return m_is_running; }
		void stop() 					{ m_is_running = false; }
};

//------------------------------------------------------------------------
struct log_object_t
{
	uint64_t hpc; 
	std::string filter; 
	std::string line; 
	Callstack callstack; 
}; 

//------------------------------------------------------------------------
static void LogThread() 
{
	// make the file;
	// create a file/overwrite a file
	FILE* file = fopen( gLogSystem->filename.c_str(), "wb" ); 
	if (file == nullptr) {
		// bad juju
		// mark an error, return from thread; 
	}

	// wait for information to write to log while running
	while (gLogSystem->is_running()) {
		// get data to write
		// potentially a dead-lock; 
		gLogSystem->wait_for_work(); 

		log_object_t *log; 
		while (gLogSystem->m_messages.dequeue(&log)) {
			// write it
			// buffer is the c_str of the message
			fwrite( buffer, 1, size, file ); 
			fwrite( '\n', 1, 1, file ); 

			delete log; 
		}
	}

	// flush the file
	// ... make sure queue is empty

	// close the file
	fclose( file ); 
}

//------------------------------------------------------------------------
void LogSystemInit( char const* filename )
{
	gLogSystem = new LogSystem(); 

	gLogSystem->m_filename = filename; 
	gLogSystem->m_signal.create( 0, 1 ); 

	// last thing I do before returning
	gLogSystem->thread = std::thread( LogThread ); 
}

//------------------------------------------------------------------------
void LogSystemShutdown()
{
	gLogSystem->stop(); 

	Logf( "Shutting down log system..." ); 
	gLogSystem->signal_work(); 
	// gLogSystem->m_signal.close(); 
	
	gLogSystem->thread.join(); 

	// potentially other cleanup; 
}


//------------------------------------------------------------------------
void Logf( char const* filter, char const* format, ... ) 
{
	log_object_t* log = new log_object_t(); 

	// fill this
	// ...

	gLogSystem->m_messages.enqueue( log ); 
	gLogSystem->signal_work(); 
}