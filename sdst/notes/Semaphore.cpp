
//------------------------------------------------------------------------
// Engine/Async/Semaphore.hpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Semaphore
{
   public:
      Semaphore(); 
      Semaphore( uint max_count, uint initial_count ); 
      Semaphore( Semaphore const& copy ) = delete; // NO COPYING - these are unique!
      ~Semaphore(); 

      Semaphore( Semaphore const& copy ) = delete; 

      void reinit( uint max_count, uint initial_count = ~0U );  // will default to full count

      void acquire(); 
      bool try_acquire( uint ms ); 
      bool release( uint count = 1U ); 

      // alias methods to help with scope lock;
      inline void lock()      { acquire(); } 
      inline bool try_lock()  { try_acquire(0U); }
      inline void unlock()    { release(1); }

   private:
      // feel free to void* instead of HANDLE if you want to avoid including Windows.h
      HANDLE m_handle = nullptr;  // HANDLE from Windows.h
}; 


//------------------------------------------------------------------------
// Engine/Async/Semaphore.cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
Semaphore::Semaphore()
{
   // by default, behaves a lot like a CriticalSection
   // starting in an "unlocked" state, and allowing only one lock at a time
   m_handle = CreateSemaphore( nullptr,   // security rules
      1,                                  // initial count of the semaphore
      1,                                  // max count of the semaphore
      nullptr );                          // name if shared; 
   ASSERT( m_handle != nullptr ); 
}

//------------------------------------------------------------------------
Semaphore::Semaphore( uint max_count, uint initial_count )
{
   // just giving us a little more control on how it is constructed
   m_handle = CreateSemaphore( nullptr, initial_count, max_count, NULL ); 
   ASSERT( m_handle != nullptr ); 
}

//------------------------------------------------------------------------
Semaphore::~Semaphore()
{
   ::CloseHandle( m_handle ); 
   m_handle = nullptr; 
}

//------------------------------------------------------------------------
void Semaphore::reinit( uint max_count, uint initial_count )
{
   // be sure no-one is using this semaphore if calling this method
   // as we destroy the old one and recreate with our new options
   ::CloseHandle( m_handle ); 
   m_handle = CreateSemaphore( nullptr, initial_count, max_count, NULL ); 
}

//------------------------------------------------------------------------
void Semaphore::acquire()
{
   // an atomic version of...
   // while (m_count == 0) {
   //   // do nothing
   // }
   // --m_count; 
}

   ::WaitForSingleObject( m_handle, INFINITE ); 
}

//------------------------------------------------------------------------
bool Semaphore::try_acquire( uint ms )
{
   // Same as above - but can time out and fail.  If it succeeds
   // WaitForSingleObject will tell you the object that it "locked", which
   // it the first item (WAIT_OBJECT_0).  (Naming makes more sense knowing
   // WaitForMultipleObjects exists)
   DWORD res = ::WaitForSingleObject( m_handle, ms ); 
   return (res == WAIT_OBJECT_0); 
}

//------------------------------------------------------------------------
bool Semaphore::release( uint count )
{
   // Adds to the semaphores count
   // can read as an atomic: 
   // m_count = Min( m_count + count, m_max_count ); 
   return ::ReleaseSemaphore( m_handle, 
      count,     // amount to add
      nullptr ); // out variable if you want to know the previous count 
}



/************************************************************************/
/*                                                                      */
/* COMMANDS                                                             */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* UNIT TESTS                                                           */
/*                                                                      */
/************************************************************************/
