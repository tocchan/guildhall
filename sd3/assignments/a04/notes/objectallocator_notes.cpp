void Game::Update()
{
    PROFILE_SCOPE( "Game::Update" ); // ProfilePush("Game::Update"); 
    // PROFILE_SCOPE( __FUNCTION__ ); 
    // PROFILE_FUNCTION(); 

    // do stuff in update
    {
        PROFILE_SCOPE("Physics"); // ProfilePush("Physics");  
        Physics::Update(); 
        // ProfilePop() for "Physics" happens here due to leaving scope
    }
    // ProfilePop() for Game::Update happens here due to leaving scope
}

void Physics::Update()
{
  {
    PROFILE_SCOPE("Rigidbody::Update"); 
    Rigidbody::Update();
  }

  {
    PROFILE_SCOPE_("Collide2D::Update"); 
    Collide2D::Update();
  }

  Rigidbody::Update();
}

vec2 operator+( vec2 const& lh, vec2 const& rh ) { ... }; 


#define PROFILING_ENABLED

#if defined(PROFILING_ENABLED)
  #define PROFILE_SCOPE(tag) ... 
#else
  #define PROFILE_SCOPE(...) 
#endif

class ProfileScope
{

};


template <typename OBJ_TYPE>
class ObjectAllocator : private BlockAllocator 
{
   public:
      void init( Allocator* parent, uint blocks_per_chunk )
      {
         return BlockAllocator::init( parent, sizeof(OBJ_TYPE), alignof(OBJ_TYPE), blocks_per_chunk ); 
      } 

      void deinit() 
      {
         BlockAllocator::deinit(); 
      }

      virtual void* alloc( size_t size ) { return BlockAllocator::alloc( size ); }
      virtual void free( size_t size ) { BlockAllocator::free( size ); }

      template <typename ...ARGS>
      OBJ_TYPE* create( ARGS&& ...args ) // std::vector<int>&& thing; 
      {
         void* mem = alloc( sizeof(OBJ_TYPE) ); 
         if (mem != nullptr) {
            return new (mem) OBJ_TYPE( std::forward(args)... ); 
         } else {
            return nullptr; 
         }
      }

      void destroy( OBJ_TYPE* obj )
      {
         free( obj ); 
      }
};

size_t align_to( size_t value, size_t alignment ) 
{
   // is_pow_2
   // return (alignment & alignment - 1) == 0; 

   size_t result = alignment; 
   while (result < value) {
      result += alignment; 
   }

   return result; 
}

alignas(16)
struct cache_line
{
   int a, b, c, d; 
}; 

sizeof(obj) = n * alignof(obj); 

// pool
template <typename OBJ_TYPE>
class ObjectPool 
{
   struct node_t
   {
      node_t* next; 
   };

   public:
      void init( uint blocks_per_chunk )
      {
         m_node_header_size = align_to( sizeof(node_t), alignof(OBJ_TYPE) );  
         m_nodes.init( sizeof(OBJ_TYPE) + node_header_size );
      }

      OBJ_TYPE* acquire()
      {
         // get from free list
         if (have thing) {
            return thing; 
         } else {
            byte* buf = m_nodes.alloc();
            buf += m_node_header_size; 
            new (buf) OBJ_TYPE(); 

            return thing;
         }
      }

      void release( OBJ_TYPE* obj )
      {
         byte* buf = (byte*)obj; 
         buf -= m_node_header_size; 

         node_t* node = (node_t*)buf; 
         node->next = m_free_list; 
         m_free_list = node; 
      } 

   private:
      BlockAllocator m_nodes; 
      node_t* m_free_list; 
};


void Foo( std::vector<int>&& list )
{
   std::vector<int> otherList = std::move(list);
}

void Foo( std::vector<int>& list )
{
   std::vector<int> otherList = std::move(list); 
   list.clear();
}

void DoThing()
{
   std::vector<int> primes = GetPrimes(); 

   gPool.create( std::move(primes) ); 

   Foo( primes ); 
   // primes is now empty... unspecified
}

