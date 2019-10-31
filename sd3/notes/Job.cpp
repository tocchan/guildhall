#pragma once

#include "Job.hpp"
#include "AsyncQueue.hpp"
#include "ObjectAllocator.hpp"

#include <atomic>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
static constexpr uint MAX_JOB_DEPENDANTS = 4; 

enum eJobState : byte
{
	JOB_STATE_INITIALIZING = 0, 
	JOB_STATE_WAITING_ON_DEPENDANCIES,
	JOB_STATE_ENQUEUED, 
	JOB_STATE_RUNNING, 
	JOB_STATE_FINISHING, 
	JOB_STATE_FINISHED,
};

//-----------------------------------------------------------------------------------------------
struct JobImpl
{
	public:
		uint16_t ref_count; 
		uint16_t disptach_count; 		
		// 4 bytes

		eJobState state; 
		uint8_t data_size;				// 0xff if using large storage (in which case size is stored in first part of data)
		uint8_t next_job_count;			// 
		// 7 bytes

		uint8_t pad0[1]; // pad to 8 bytes

		job_work_cb work; // 16 bytes

		// children are stored at the start - data is stored
		// at the end (allocating a heap allocation if more than the local buffer is needed)
		byte data[112]; // data section - 128 bytes

	public:
		void add_child( JobImpl* child );
		JobImpl* get_child( int index ) const; 

		void* get_or_alloc_data( size_t size ); 
		size_t get_data_size() const; 
		void* get_data( size_t *out_size ) const; 
		void free_data(); 
}; 

//-----------------------------------------------------------------------------------------------
class JobCategory
{
	public:
		void enqueue( JobImpl* impl );

		bool consume_one();
		int consume_all();

	public:
		AsyncQueue<JobImpl*> m_work_queue; 
};

//-----------------------------------------------------------------------------------------------
class JobSystem
{
	public:
		JobSystem()
		{

		}

	public:
		ObjectAllocator<JobImpl> m_job_pool;
		JobCategory m_generic_category; 
};

//-----------------------------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------------------------
static JobSystem* gJobSystem = nullptr; 


//-----------------------------------------------------------------------------------------------
// Internal Impl
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
static inline uint16_t AtomicIncrement( volatile uint16_t* val )
{
	return InterlockedIncrement16( (volatile short*)val ); 
}

//-----------------------------------------------------------------------------------------------
static inline uint16_t AtomicDecrement( volatile uint16_t* val )
{
	return InterlockedDecrement16( (volatile short*)val ); 
}

//-----------------------------------------------------------------------------------------------
static JobImpl* JobAlloc()
{
	JobImpl* job = gJobSystem->m_job_pool.alloc(); 

	job->ref_count = 1; 
	job->disptach_count = 1; 
	
	job->work = nullptr; 
	
	job->state = JOB_STATE_INITIALIZING; 
	job->next_job_count = 0; 
	job->data_size = 0; 

	return job; 
}

//-----------------------------------------------------------------------------------------------
static void JobFree( JobImpl* job )
{
	job->free_data(); 
	gJobSystem->m_job_pool.free( job ); 
}

//-----------------------------------------------------------------------------------------------
static void JobAcquire( JobImpl* job ) 
{
	AtomicIncrement( &job->ref_count ); 
}

//-----------------------------------------------------------------------------------------------
static bool JobRelease( JobImpl* job )
{
	if (AtomicDecrement( &job->ref_count ) == 0) {
		JobFree( job ); 
		return true; 
	}

	return false; 
}

//-----------------------------------------------------------------------------------------------
static void JobDispatch( JobImpl* job )
{
	if (AtomicDecrement( &job->disptach_count ) == 0) {
		job->state = JOB_STATE_ENQUEUED; 
		gJobSystem->m_generic_category.enqueue( job ); 
	}
}

//-----------------------------------------------------------------------------------------------
void JobCategory::enqueue( JobImpl* impl )
{
	JobAcquire( impl ); 
	m_work_queue.enqueue( impl ); 
}

//-----------------------------------------------------------------------------------------------
bool JobCategory::consume_one()
{
	JobImpl* job;
	if (m_work_queue.try_dequeue( &job )) {

		job->state = JOB_STATE_RUNNING;

		// run the work if there is any (otherwise this is just used to tell when work is done)
		if (job->work != nullptr) {
			Job j(job); // just for passing it forward
			job->work( j );
		}

		job->state = JOB_STATE_FINISHING; 

		// dispatch children
		for (uint8_t i = 0; i < job->next_job_count; ++i) {
			JobImpl* child = job->get_child(i); 
			JobDispatch( child );
			JobRelease( child ); 
		}

		// done - release myself
		job->state = JOB_STATE_FINISHED; 
		JobRelease( job ); 
		return true; 
	}

	return false; 
}

//-----------------------------------------------------------------------------------------------
int JobCategory::consume_all()
{
	int count = 0; 
	while (consume_one()) {
		++count; 
	}
	
	return count; 
}


//-----------------------------------------------------------------------------------------------
// Interface
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Job
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
Job::Job( JobImpl* impl )
{
	m_job = impl;
	if (m_job != nullptr) {
		JobAcquire( m_job ); 
	}
}

//-----------------------------------------------------------------------------------------------
Job::~Job()
{
	if (m_job != nullptr) {
		JobRelease( m_job ); 
		m_job = nullptr; 
	}
}

//-----------------------------------------------------------------------------------------------
Job::Job( Job const& copy )
{
	m_job = copy.m_job; 
	if (m_job != nullptr) {
		JobAcquire( m_job ); 
	}
}

//-----------------------------------------------------------------------------------------------
Job::Job( Job&& move )
{
	m_job = move.m_job;
	move.m_job = nullptr; 
}

//-----------------------------------------------------------------------------------------------
Job& Job::operator=(Job const& copy)
{
	m_job = copy.m_job; 
	if (m_job != nullptr) {
		JobAcquire( m_job ); 
	}
	return *this; 
}

//-----------------------------------------------------------------------------------------------
Job& Job::operator=(Job&& move)
{
	m_job = move.m_job;
	move.m_job = nullptr; 
	return *this; 
}

//-----------------------------------------------------------------------------------------------
void Job::add_dependant( Job& other )
{
	m_job->add_child( other.m_job ); 
}

//-----------------------------------------------------------------------------------------------
void Job::depends_on( Job& other )
{
	other.add_dependant(*this); 
}

//-----------------------------------------------------------------------------------------------
void* Job::alloc_data( size_t size )
{
	if (m_job == nullptr) {
		return nullptr; 
	} else {
		return m_job->get_or_alloc_data(size); 
	}
}

//-----------------------------------------------------------------------------------------------
void* Job::get_data( size_t* out_size )
{
	if (m_job == nullptr) {
		if (out_size != nullptr) {
			*out_size = 0; 
		}
		return nullptr;
	} else {
		return m_job->get_data( out_size );
	}
}


//-----------------------------------------------------------------------------------------------
// JobImpl
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// children are added to beginning of data block
void JobImpl::add_child( JobImpl* child )
{
	JobImpl** start = (JobImpl**)(data); 
	start += (next_job_count); // move to that jobs place from the end of the data section
	++next_job_count; 

	*start = child; 

	AtomicIncrement( &child->disptach_count ); 
	AtomicIncrement( &child->ref_count ); 
}

//-----------------------------------------------------------------------------------------------
JobImpl* JobImpl::get_child(int index) const
{
	JobImpl** children = (JobImpl**)data; 
	return children[index];  
}

//-----------------------------------------------------------------------------------------------
void* JobImpl::get_or_alloc_data(size_t size)
{
	if (get_data_size() == size) {
		return get_data( nullptr ); 
	}

	byte* end = (byte*)(this + 1); 

	free_data(); 
	if (size >= 0xff) {
		void* buffer = ::malloc(size); 
		data_size = 0xff; 

		size_t* size_storage = (size_t*)end; 
		size_storage--; 

		*size_storage = size; 
		void** ptr_storage = (void**)(size_storage - 1); 
		*ptr_storage = buffer; 

		return buffer;

	} else {
		data_size = (byte) size; 
		return end - size; 
	}
}

//-----------------------------------------------------------------------------------------------
size_t JobImpl::get_data_size() const
{
	if (data_size == 0xff) {
		size_t* end = (size_t*)(this + 1); 
		--end; 
		return *end; 
	} else {
		return data_size; 
	}
}


//-----------------------------------------------------------------------------------------------
void* JobImpl::get_data( size_t* out_size ) const
{
	byte* end = (byte*)(this + 1); 
	if (data_size == 0xff) {
		void** pointer = (void**)end; 
		if (out_size != nullptr) {
			*out_size = *(size_t*)(pointer - 1); 
		}
		return *(pointer - 2); 
	} else {
		if (out_size != nullptr) {
			*out_size = data_size; 
		}
		return end - data_size; 
	}
}

//-----------------------------------------------------------------------------------------------
void JobImpl::free_data()
{
	if (data_size == 0xff) {
		void* ptr = get_data( nullptr );
		free(ptr); 
	}

	data_size = 0; 
}


//-----------------------------------------------------------------------------------------------
// C Functions
//-----------------------------------------------------------------------------------------------
void JobSystemStartup()
{
	gJobSystem = new JobSystem(); 
}

//-----------------------------------------------------------------------------------------------
void JobSystemShutdown()
{
	JobConsumeAll(); 
	delete gJobSystem; 
	gJobSystem = nullptr; 
}

//-----------------------------------------------------------------------------------------------
bool JobConsumeOne()
{
	return gJobSystem->m_generic_category.consume_one(); 
}

//-----------------------------------------------------------------------------------------------
int JobConsumeAll()
{
	return gJobSystem->m_generic_category.consume_all(); 
}

//-----------------------------------------------------------------------------------------------
Job JobCreate( job_work_cb work )
{
	JobImpl* job = JobAlloc(); 
	job->work = work; 

	// create a "refcounted" job for it
	// to acquire the job
	Job ret(job); 

	// relase my functions hold on it
	JobRelease(job); 

	// return it to the user (if they don't use it, ref will go to zero)
	return job; 
}

//-----------------------------------------------------------------------------------------------
void JobDispatch( Job& job )
{
	job.m_job->state = JOB_STATE_WAITING_ON_DEPENDANCIES; 
	JobDispatch( job.m_job ); 
}

//-----------------------------------------------------------------------------------------------
bool JobIsDone( Job& job )
{
	return (job.m_job == nullptr) || (job.m_job->state == JOB_STATE_FINISHED); 
}

//-----------------------------------------------------------------------------------------------
void JobJoin( Job& job )
{
	while (!JobIsDone(job)) {
		JobConsumeOne(); 
	}
}


