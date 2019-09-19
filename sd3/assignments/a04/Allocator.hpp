
// Different systems support allocators different;
// - C++ std uses static classes and basically singleton allocators
// - Some use a simple struct with function pointers
//   - Very common in 3rd party tools
// - Previous companies I've worked use interfaces
//   - This allows using the same pattern with different blocks of memeory
//   - Lets us give systems memory blocks/pools, and letting the system decide how to divvy them up; 
class IAllocator  // or just Allocator if you prefer
{
   public:
      // get memory
      virtual void* alloc( size_t size )     = 0; 

      // free memory; 
      virtual void free( void* ptr )         = 0; 

      // virtual void* realloc( void* ptr, size_t size ) = 0; // if you want, though most allocator types we'll write are unable to do this; 

      // some helpers I tend to like to have as convenience
      template <typename T, typename ...ARGS>
      T* create( ARGS&& ...args )       
      {
         // for a allocator meant for modern C++, the below is a bit simplistic
         // and you may want to "perfect-foward"
         // see: http://cpptruths.blogspot.com/2012/06/perfect-forwarding-of-parameter-groups.html

         void* mem = this->alloc( sizeof(T) ); 
         if (mem != nullptr) {
            // new in place
            new (mem) T( args... ); 
         }

         return (T*) mem; 
      }

      template <typename T> destroy( T* obj ) 
      {
         if (obj != nullptr) {
            obj->~T(); 
            this->free( obj ); 
         }
      }
      
}; 

// note, you do not pay the cost of "virtual" if the object is known not to be inherited from (using an object by value, or the type it is
// at is 'final'.  So good use case for that here; 

// news that take an allocator to use; 
void* operator new( size_t size, IAllocaotr& pool ) { return pool->alloc(size); }
void operator delete( void* ptr, IAllocator& pool ) { pool->free(size); }

// To make an allocator for our tracked memory, it is as easy as this; 
class TrackedAllocator : public IAllocator 
{
   public: 
      virtual void* alloc( size_t size ) final    { return TrackedAlloc( size ); }
      virtual void free( void* ptr ) final         { TrackedFree( ptr ); }

      // TODO: potentially make a singleton, since no real reason to 
      // have multiple of this;  Also, TrackedAlloator is the "default" allocator of the sytem; 
}; 

// similar for UntrackedAllocator 
// ...

      
