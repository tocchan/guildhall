Order of Work
======

## Header & Basic Flow

## Log Thread - Simple
- Implement using `AsyncQueue`
- Spin and Check 

## Sleeping/Signaling

### Semaphore
Semaphore is a thread-safe counter than can be decremented (`acquire`) down to 0, or incremented (`release`) up to max count.

If it is unable to `acquire`, it will sleep until it can.  If it is unable to `release`, nothing happens, and the count stays at max; 

Semaphores are ideal for consumer/producer problems, as the producer can `release` to signal that there is work to be done, and the consumer can `acquire` to do that work;  You can release multiple at a time to try to wake up multiple consumers as well.  

But since they are not part of the standard, we'll have to make our own class that is platform
specific.  This is an implementation for windows.  See http://man7.org/linux/man-pages/man7/sem_overview.7.html for Linux equivalent functionality; 


```cpp
class Semaphore 
{
   HANDLE m_semaphore; // HANDLE is a void*, if you want to use that in your header; 

   void create( uint initial_count, uint max_count ) 
   {
      // Creating/Initializing
      m_semaphore = ::CreateSemaphore( nullptr,  // security attributes - ignore 
         initial_count,                      // count this starts at
         max_count,                          // max count this semaphore can reach
         nullptr );                          // name, if used across processes
   }

   // note: should probably call from deconstructor
   void destroy() 
   {
      if (m_semaphore != nullptr) {
         ::CloseHandle( m_semaphore ); 
         m_semaphore = nullptr; 
      }
   }

   // Acquire a Seamphore
   // this will block until the semaphore becomes invalid (destroyed)
   // or succeeds
   void acquire() 
   {
      ::WaitForSingleObject( m_semaphore, // object to wait on
         INFINITE );                      // time to wait in milliseconds
   }

   // may or may not succeed
   // if returns true, the counter was decremented
   // if returns false, the counter was 0 and unable to be decremented
   bool try_acquire() 
   {
      DWORD result = ::WaitForSingleObject( m_semaphore, 0 ); 
      return (res == WAIT_OBJECT_0); // we successfully waited on the first object (m_semaphroe)
   }

   // releases teh seamphore - ie, adds to the counter up to max
   void release( uint count = 1 ) 
   {
      ::ReleaseSemaphore( m_semaphore, 
         count,      // how many to add to the semaphore
         nullptr );  // out for previous count
   }

   // to make this work like a normal scope lock; 
   inline void lock()      { acquire(); }
   inline bool try_lock()  { return try_acquire(); }; 
   inline void unlock()    { release(1); }
}
```

So a critical section/mutex can be made with a semaphore by creating a semaphore with an
initial count of 1, and a max count of 1.  

We can use this as a signal for the log thread by creating it with a max count of 1, but a initial count of 1.  Every time we log, we release() (adding potentially one to the count, up to max of 1).  The log thread sleeps until this happens, *consumes* it, and then does its work. 


### Condition Variables 
Condition variables are meant as a way to signal/wait on a specific event, and allow for you to wake only one, or all threads waiting on it. 

For the problem we're solving, semaphore is my preferred solution, but it is not currently part of the standard.  If you prefer to keep only to standard C++, condition variables can be used.

In this case, we just want to signal, so there is no shared resource;

```cpp
std::condition_variable gSignal; 

void LogThread()
{
   // setup
   // ...
   std::mutex mutex; 

   // locks/enters this mutex automatically by default
   // condition variables guarantee we have a lock when 
   // returned, so we are providing one to use; 
   std::unique_lock<std::mutex> local_lock(mutex); 

   // wait for work
   while (running) {
      // wait for a signal, no real condition to check
      // hence returning true (allow it to wakeup when signaled)
      gSignal.wait( local_lock, []() { return true; } ); 

      while (has work) {
         // do work
         //
      }
   }
}

void Logf(...)
{
   // prepare work 
   // ...

   // Let the thread know it has work; 
   gSignal.notify_one(); 
}
```

## RingBuffer Implemention
Prevent use of additional memory of the system (outside of potentially hooks)

