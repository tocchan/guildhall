#pragma once

#include "Types.hpp"
#include <tuple>

struct JobImpl; 

class Job
{
	public:
		Job() : m_job(nullptr) {}
		Job( std::nullptr_t ) : m_job(nullptr) {}
		Job( JobImpl* impl );
		~Job(); 

		Job( Job const& copy ); 
		Job( Job&& move ); 

		Job& operator=( Job const& copy ); 
		Job& operator=( Job&& move ); 

		void add_dependant( Job& other ); 
		void depends_on( Job& other ); 

		void* alloc_data( size_t size ); 
		void* get_data( size_t* out_size = nullptr ); 

	public:
		JobImpl* m_job;  
}; 

typedef void (*job_work_cb)( Job& job ); 


//-----------------------------------------------------------------------------------------------
void JobSystemStartup();
void JobSystemShutdown();
bool JobConsumeOne(); 
int JobConsumeAll();

Job JobCreate( job_work_cb work );

// Runs the job, but holds onto the reference
void JobDispatch( Job& job ); 
bool JobIsDone( Job& job ); 

// *frees* the job - join will wait until the job is finished; 
void JobJoin( Job& job ); 

//-----------------------------------------------------------------------------------------------
// Templated Helpers
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
template <typename OBJTYPE, typename METHOD, typename ...ARGS>
void MethodForwardJob( Job& job )
{
	constexpr size_t pointer_size = sizeof(OBJTYPE*); 
	constexpr size_t method_pointer_size = sizeof(METHOD); 

	byte* data = (byte*) job.get_data();
	OBJTYPE* obj = *(OBJTYPE**)data; 
	data += pointer_size;

	METHOD method = *(METHOD*)data; 
	data += method_pointer_size; 

	std::tuple<ARGS...>* tuple = (std::tuple<ARGS...>*)data; 

	auto f = [&]( ARGS& ...args ) { (obj->*method)( args... ); }; 
	std::apply( f, *tuple ); 
}

//-----------------------------------------------------------------------------------------------
template <typename OBJTYPE, typename METHOD, typename ...ARGS>
Job JobCreateMethod( OBJTYPE* object, METHOD method, ARGS& ...args ) 
{
	constexpr size_t pointer_size = sizeof(OBJTYPE*); 
	constexpr size_t method_pointer_size = sizeof(METHOD); 
	constexpr size_t tuple_size = sizeof(std::tuple<ARGS...>); 
	constexpr size_t total_size = pointer_size + method_pointer_size + tuple_size; 

	// create the job
	Job job = JobCreate( MethodForwardJob<OBJTYPE,METHOD,ARGS...> ); 

	// put data into the job; 
	byte* data = (byte*) job.alloc_data( total_size );
	OBJTYPE** obj_storage = (OBJTYPE**)data; 
	*obj_storage = object; 
	data += pointer_size;

	METHOD* method_storage = (METHOD*)data; 
	*method_storage = method; 
	data += method_pointer_size; 

	std::tuple<ARGS...> *tuple = new (data) std::tuple<ARGS...>(args...); 

	// run the job
	return job; 
}

//-----------------------------------------------------------------------------------------------
template <typename OBJTYPE, typename METHOD, typename ...ARGS>
Job JobRunMethod( OBJTYPE* object, METHOD method, ARGS& ...args ) 
{
	Job job = JobCreatemethod( object, method, args... ); 
	JobDispatch( job ); 

	return job; 
}

//-----------------------------------------------------------------------------------------------
template <typename OBJTYPE, typename METHOD, typename ...ARGS>
Job JobRunMethodBefore( Job& dependant, OBJTYPE* object, METHOD method, ARGS& ...args ) 
{
	Job job = JobCreateMethod( object, method, args... ); 
	job.add_dependant( dependant ); 
	JobDispatch( job ); 

	return job; 
}



