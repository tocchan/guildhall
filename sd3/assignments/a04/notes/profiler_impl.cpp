struct profiler_node_t
{
   // parent
   profiler_node_t* parent; 

   // children
   profiler_node_t* last_child; 
   profiler_node_t* prev_sibling; // first_child, prev_sibling, next_sibling 

   char label[32]; //

   // timing
   uint64_t start_time; 
   uint64_t end_time; 

   // these are head specific - could have a seperate
   // type
   std::thread::id thread_id; 
   uint ref_count       = 0; 

   // memory
   // alloc_count, byte_count

   void add_child( profile_node_t* child ) 
   {
      child->parent = this; 
      child->prev_sibling = last_child; 
      last_child = child; 
   }
};


//----------------------------------------------------------
static thread_local profile_node_t* tActiveNode = nullptr; 

static std::vector<profile_node_t*> gHistory; 
static std::shared_mutex gHistoryLock; 

//----------------------------------------------------------
// see objectpool_impl.cpp
// for CORRECT version
//----------------------------------------------------------
static profile_node_t* AllocNode()
{
   profile_node_t* node = new profile_node_t(); 
   node->ref_count = 1; 
}

//----------------------------------------------------------
static void FreeNode( profile_node_t* node ) 
{
   delete node; 
}

//----------------------------------------------------------
void ProfileReleaseTree( profile_node_t* node ) 
{
   uint new_count = ::InterlockedDecrement( &node->ref_count ); 
   if (new_count == 0) {
      FreeTree( node ); 
   }
}


//----------------------------------------------------------
void ProfilePush( char const* tag )
{
   profile_node_t* new_node = AllocNode(); 

   new_node->parent = tActiveNode; 
   new_node->thread_id = std::this_thread::get_id(); 

   // setup now
   new_node->start_time = GetCurrentTimeHPC(); 
   new_node->set_name( tag ); 

   if (tActiveNode != nullptr) {
      tActiveNode->add_child( new_node ); 
   }   
   
   // this is now my active node
   tActiveNode = new_node; 
}

//----------------------------------------------------------
static void FreeTree( profile_node_t* root ) 
{
   // free children
   // ... 

   std::thread t = std::thread( Example, &tActiveNode );  

   // free myself
   FreeNode( root ); 
}

//----------------------------------------------------------
void ProfilePop()
{
   if (tActiveNode == nullptr) {
      // oh-oh
      // ... ?
      return; 
   }

   // finalize
   tActiveNode->end_time = GetCurrentTimeHPC(); 

   // this is where I am now
   profile_node_t* old_active = tActiveNode;
   tActiveNode = tActiveNode->parent; 
   if (tActiveNode == nullptr) {
      std::scoped_lock<std::shared_mutex> lk( gHistoryLock ); 
      gHistory.push_back( old_active ); 
   }
}

//----------------------------------------------------------
// example
//----------------------------------------------------------
void App::BeginFrame()
{
   ProfileBeginFrame(); 

   // engine stuff
   // game stuff
}

void App::EndFrame()
{
   // game stuff
   // engine stuff

   ProfileEndFrame(); 
}
//----------------------------------------------------------
// end example
//----------------------------------------------------------

//----------------------------------------------------------
void ProfileBeginFrame( char const* label = "frame" )
{
   ASSERT( tActiveNode == nullptr ); 

   ProfilePush( label ); 
}

//----------------------------------------------------------
void ProfileEndFrame()
{
   ProfilePop(); 

   ASSERT( tActiveNode == nullptr ); 
}

//----------------------------------------------------------
// example
void Game::Update()
{
   PROFILE_SCOPE("Game::Update");  // Push and Pop

   // profiles the entire scope
   // ...

   // pop would happen here...
}

//----------------------------------------------------------
profile_node_t* ProfilerAcquirePreviousTree( std::thread::id id, uint history = 0 )
{
   std::shared_lock lk( gHistoryLock ); 

   // loop through gHistory
   // count up to history all trees from thread::id id
   // return that tree or null if there wasn't enough;  
}