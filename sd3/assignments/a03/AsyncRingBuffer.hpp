class AsyncRingBuffer
{
   public:
      AsyncRingBuffer(); 
      ~AsyncRingBuffer(); 

      bool init( size_t size_in_bytes ); 
      void deinit();    // called from deconstructor

      // try will always return immediately
      // where a non-try version will block until there is 
      // room (it may still fail if the allocated was deinit'd
      // or the size requested was larger than the ring buffers capability)
      void* try_lock_write( size_t size ); 
      void* lock_write( size_t size ); 
      void unlock_write( void* ptr ); 

      void* try_lock_read( size_t* out_size ); 
      void* lock_read( size_t* out_size );      
      void unlock_read( void* ptr ); 

      // helpers - optional
      bool write( void const* data, size_t byte_size ); 
      bool try_write( void const* data, size_t byte_size ); 

      // return 0 on failure, otherwise number of bytes read; 
      // be sure buffer is large enough.  Non-async version could do a peek; 
      size_t read( void* out_data ); 
      size_t try_read( void* out_data ); 

   private: 
      // data we need to support this; 
      // ...
};