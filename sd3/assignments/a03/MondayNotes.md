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
// What the processor is doing - NOT CODE YOU WRITE
struct semaphore_t 
{
   uint count; 
   uint max_count; 
};

void SemaphoreAcquire( semaphore_t &obj )
{
   while (obj.count == 0) {
      // yield; 
   }

   --obj.count; 
}

void ReleaseSemaphore( semaphore_t &obj, uint count ) 
{
   obj.count = min( obj.count + count, obj.max_count ); 
}

```

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
Goal of this is to prevent use of additional memory of the system, ie, the log system is self contained.  Ring buffers are great for FIFO (first in-first out) use cases.  

Intended use; 
```cpp
//------------------------------------------------------------------------
void Logf( char const* tag, char const* format, ... )
{
   // do not work if the message is currently filtered; 
   // you could alternatively do this on the thread if you're
   // allowing for seperate filters for console/files/etc...
   if (IsFiltered(tag)) {
      return; 
   }

   // lock enough space to write log information; 
   // look into `vsnprintf` for determining buffer size needs
   // for the format/arguments...
   size_t bytes_needed = 0; 
   // need space for the log_message_t, the format and tag; 
   // for callstack version - also enough room for a callstack
   // ...

   // allocate it using a thread-safe ring-buffer
   byte* buf = (byte*) gMessageBuffer->lock_write( bytes_needed ); 

   // sub allocate and finalize the message
   // ...

   // get callstack; 
   // ...

   // mark us as finished writing - so worker thread can pick it up
   gMessageBuffer->unlock_write( buf ); 
}
```

Impl;
```cpp
// Mutiple-Producer, Single-Consumer Ring Buffer
// with notes for an MPMC ring-buffer
// There are faster ways to write this - but going for understandability!
typedef uint8_t byte; 

struct ringbuffer_header_t 
{
   uint size      : 31;   
   uint ready     : 1; 
}; 

class MPSCRingBuffer
{
   public:
      // members needed
      byte* m_buffer       = nullptr; 
      size_t m_byte_size   = 0; 

      uint m_write_head    = 0; 
      uint m_read_head     = 0; 

      std::mutex m_lock; 


      ~MPSCRingBuffer()
      {
         deinit(); 
      }

      // methods
      bool init( size_t byte_size )
      {
         // todo: make sure deinit'd; 
         // handle it being init'd

         m_buffer = (byte*) malloc( byte_size ); 
         m_byte_size = byte_size; 
      } 

      void deinit()
      {
         // frees memory
         if (m_buffer != nullptr) {
            free( m_buffer ); 
            m_buffer = nullptr; 
            m_byte_size = 0U; 
         }
      }

      //------------------------------------------------------------------------
      // for now - lets do the blocking calls as just attempting the trys until 
      // they succeed (can do better by waiting on a semaphore if they fail the first time)
      // creating a block lock
      void* lock_write( size_t size )
      {
         // really want to be safe and allow REALLY large allocations

         void* ptr = try_lock_write(size); 
         while (ptr == nullptr) {
            std::this_thread::yield(); 
            ptr = try_lock_write(size); 
         }

         return ptr; 
      }

      //------------------------------------------------------------------------
      size_t get_writable_space() const 
      {
         // size_t remaining = (m_read_head - m_write_head - 1) % m_size;  
         size_t remaining = 0; 
         if (m_write_head >= m_read_head) {
            remaining = m_size - m_write_head; 
            remaining += m_read_head; 
         } else {
            remaining = m_read_head - m_write_head; 
         }

         return remaining; 
      }

      //------------------------------------------------------------------------
      // return nullptr if failed
      // otherwise valid pointer of at least size
      void* try_lock_write( size_t size )
      {
         ASSERT( size < (1 << 31) );
         size_t header_size = sizeof(ringbuffer_header_t); 
         size_t total_size = 2 * header_size + size; 

         std::scoped_lock lk( &m_lock ); 
         if (get_writable_space() < total_size) {
            return nullptr; 
         }

         uint new_head = m_write_head + total_size; 
         if (new_head > m_size) {
            // need to wrap;
            ringbuffer_header_t* skip_header = (ringbuffer_header_t*)(m_buffer + m_write_head); 
            skip_header->size = 0;  // 0 means skip; 
            skip_header->ready = 1; 

            m_write_head = 0; 
         }

         if (get_writable_space() < total_size) {
            return nullptr; 
         }

         // my usable buffer
         byte* cur_buf = m_buffer + used_head; 

         ringbuffer_header_t* head = (ringbuffer_header_t*)cur_buf; 
         head->size = (uint)size; 
         head->ready = 0; 

         uint used_head = m_write_head; 
         m_write_head += total_size; 

         return head + 1; 
      }

      //------------------------------------------------------------------------
      void unlock_write( void* ptr )
      {
         ringbuffer_header_t* write_head = (ringbuffer_header_t*) ptr; 
         --write_head; 

         write_head->ready = 1; 
      }

      //------------------------------------------------------------------------
      void* try_lock_read( size_t* out_size )
      {
         std::scoped_lock lk( &m_lock ); 

         while (true) {
            if (m_read_head == m_write_head) {
               return nullptr; 
            }

            ringbuffer_header_t* read_header = (ringbuffer_header_t*)(m_buffer + m_read_head); 
            if (read_header->ready) {
               if (read_header->size == 0) {
                  // skip case
                  m_read_head = 0; 
               } else {
                  // valid case
                  *out_size = read_header->size; 

                  // SINGLE CONSUMER CASE - nothing else happens
                  void* ret_buf = read_header + 1; 
                  return ret_buf;
               } 
            } else {
               // only one consumer - this shouldn't happen
               return nullptr;
            }
         }
      }

      //------------------------------------------------------------------------
      void unlock_read( void* ptr )
      {
         std::scoped_lock lk( &m_lock ); 

         ringbuffer_header_t* read_head = (ringbuffer_header_t*)ptr;
         read_head--; 

         ASSERT( (m_buffer + m_read_head) == read_head ); 

         m_read_head += sizeof(ringbuffer_header_t) + read_head->size; 
      }
};
```

