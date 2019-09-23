// You should be able to enable/disable this with an EngineConfig.hpp
// so you don't pay the cost of profiling if you aren't using it; 

bool ProfilerSystemInit(); 
void ProfilerSystemDeinit(); 

void ProfilerSetMaxHistoryTime( double seconds ); // cleanup trees that are older than seconds 

void ProfilerPause();  // stop recording (disables creation of new trees, but current trees are allowed to finish)
void ProfilerResume(); // resume recording (new trees can be created)

// RECORDING
// Recording again should allocate no heap memory, and try to be fairly quick;
// While active, profiler is always recording, and wants to do its best to not
// slow down the calling thread; 
void ProfilePush( char const* label );	// pushes a new child node, and marks it as the active node.  
void ProfilePop(); 						// pops active node in the tree, or errors if no node is present

// memory tracking fed to the profiler system 
// tracks on active node (does nothing if no node)
// see: InterlockedAdd, Subtract, Increment, and Decrement
void ProfileAllocation( size_t byte_size = 0 ); // 
void ProfileFree( size_t byte_size = 0 ); 

// A context can be thought of as a frame of work
// optional in the system, but can add some additional
// error checking to make sure people are using it correctly
void ProfileBeginFrame( char const* label = "frame" );  // 
void ProfileEndFrame();  

// EXTRA -> more important to the job system for tracking timing across threads
// manually construct a tree without relying on thread_local storage; 
// profile_handle_t ProfilePush( char const* tag, profile_handle_t parent = nullptr ); // attaches, starts, and returns a new node to parent
// profile_handle_t ProfilePop( profile_andle_t active ); // will complete active, and return active's parent (or null if no parent)

// We can only really 'view' a complete tree
// these functions return the most recently finished tree
// use a shared_mutex for accessing trees from the system (as it will try to destroy old ones constantly)
// `history` is how many frames back we should try to get
profile_node_t* ProfilerAcquirePreviousTree( std::thread::id id, uint history = 0 );
profile_node_t* ProfilerAcquirePreviousTreeForCallingThread( uint history = 0 ); 
void ProfilerReleaseTree( profile_node_t* node );

// if you prefer to use std::shared_ptr, you can use the above two to create...
// shared_ptr<profiler_node_t> ProfilerGetPreviousTree( std::thread::id id ); 

// if doing the graphic visualizer, you may want some variant of
// std::vector<std::shared_ptr<profile_node_t*>> ProfilerAqcuirePreviousFrames( std::thread::id id, uint count );  

// REPORTING
// A report takes a tree and compiles it into more human-readable
// form.  Making a report is costly in comparison to making the recordings, 
// so should only be done if the user has requested it; 
class ReportNode;

class ProfilerReport
{
	public:
		void append_flat_view( profile_node_t* root ); 
		void append_tree_view( profile_node_t* root ); 

		ReportNode* get_root() const;

	public:
		// whatever you need to make this work;
		// ... 
};

class ReportNode
{
	public:
		// sorts this report using a sorting function
		// compare returns <0 if a should appear before b
		//         returns >0 if a should appear after b
		//         returns =0 if it doens't matter
		typedef int compare_op( ReportNode const* a, ReportNode const* b ); 
		void sort( compare_op op ); 

	public:
		ReportNode* m_parent = nullptr; // parent in tree view, root node in flat view
		std::vector<ReportNode*> m_children; 

		std::string m_name; 
		uint m_call_count; 

		uint64_t m_total_hpc; 			// total time spent at this node
		uint64_t m_self_hpc; 			// time spent here not accounted for by children

		// memory tracking
		uint m_alloc_count; 			// non-verbose & verbose
		uint m_free_count; 				// non-verbose & verbose
		uint64_t m_bytes_allocated; 	// only in verbose mode
		uint64_t m_bytes_freed; 		// only in verbose mode
};