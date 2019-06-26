
//------------------------------------------------------------------------
// Engine/Async/CriticalSection.hpp
//------------------------------------------------------------------------
// Optional - you can just use std::mutex if you prefer
// This is a fair. recursive lock
class CriticalSection
{
   public:
      CriticalSection(); 
      ~CriticalSection(); 

      // keeping with these names
      // since I boht like them, and
      // it allows us to work with std::lock_guard
      void lock();
      bool try_lock(); 
      void unlock(); 

   private:
      // you could "hide" this
      // by just using a byte_buffer that is the same size and casting
      // it if you want to avoid having to include Windows.h
      CRITICAL_SECTION m_cs; // from 'Windows.h'

};

//------------------------------------------------------------------------
// Engine/Async/CriticalSection.cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
CriticalSection::CriticalSection()
{
   ::InitializeCriticalSection( &m_cs ); 
}

//------------------------------------------------------------------------
CriticalSection::~CriticalSection()
{
   ::DeleteCriticalSection( &m_cs ); 
}

//------------------------------------------------------------------------
void CriticalSection::lock()
{
   ::EnterCriticalSection( m_cs ); 
}

//------------------------------------------------------------------------
bool CriticalSection::try_lock()
{
   return ::TryEnterCriticalSection( m_cs ); 
}

//------------------------------------------------------------------------
void CriticalSection::unlock()
{
   ::LeaveCriticalSection( &m_cs ); 
}
