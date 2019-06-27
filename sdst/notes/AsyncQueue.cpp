//------------------------------------------------------------------------
// Engine/Async/AsyncQueue.hpp
//------------------------------------------------------------------------

template <typename TYPE>
class AsyncQueue
{
   public:
      void enqueue( TYPE const &v );

      // I prefer my default dequeue to be
      // non-blocking (do not wait)
      // hence it will return 'true' if we successfully got something
      bool dequeue( TYPE* out );
      // May be useful - especially for a persistant consumer model
      // bool blocking_dequeue( TYPE* out ); 

      // THAT IS IT - anything else you add is likely purely for debug purposes, and would not be useful in a practical
      // situation...
      //
      // For example...
      // uint get_count() const;
      // bool is_empty() const; 
      //
      // By the time you get the count, process and or try to use it, the count may have changed
      // rendering the information useless.  Find if you just want to log it as a metric, but not useful
      // in any critical logic; 

   private:
      // TODO
      std::queue<T> m_queue; 
      std::mutex m_mutex;  // fair and non-recursive;
      // std::recursive_mutex m_mutex; // fair and recursive
};


//------------------------------------------------------------------------
template <typename TYPE>
void AsyncQueue<TYPE>::enqueue( TYPE const& v )
{
   std::scoped_lock<std::mutex> locky(m_mutex); 
   m_queue.push( v );
}

//------------------------------------------------------------------------
template <typename TYPE>
bool AsyncQueue<TYPE>::dequeue( TYPE *out )
{
   std::scoped_lock<std::mutex> locky(m_mutex); 
   if (is_empty()) {
      return false; 
   } else {
      *out = m_queue.front(); 
      m_queue.pop();
      return true;
   } 
} 

