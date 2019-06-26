//------------------------------------------------------------------------
//------------------------------------------------------------------------
template <typename TYPE>
class AsyncQueue
{
   public:
      void queue( TYPE const &v ); 

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
};


//------------------------------------------------------------------------
template <typename TYPE>
void AsyncQueue<TYPE>::queue( TYPE const& v )
{
   // TODO
}

//------------------------------------------------------------------------
template <typename TYPE>
bool AsyncQueue<TYPE>::dequeue( TYPE *out )
{
   // TODO
   return false; 
}
