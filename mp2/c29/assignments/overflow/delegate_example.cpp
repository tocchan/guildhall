#include <vector>
#include <functional>

//------------------------------------------------------------------------
// ...ARGS -> "List Of" Args
template <typename ...ARGS>
class Delegate
{
   public:
      using function_t = std::function<void(ARGS...)>;  
      using c_callback_t = void (*)(ARGS...);  

      struct sub_t // subscription_t 
      {
         void const* obj_id      = nullptr; 
         void const* func_id     = nullptr; 
         function_t callable; 

         inline bool operator==( sub_t const& other ) const     { return (obj_id == other.obj_id) && (func_id == other.func_id); } 
      }; 

   public:
      //------------------------------------------------------------------------
      void subscribe( c_callback_t const& cb )
      {
         sub_t sub; 
         sub.func_id = cb;  // cb = &cb = &&cb = *cb = **cb = ***cb = ...
         sub.callable = cb; 

         subscribe(sub); 
      }

      //------------------------------------------------------------------------
      void unsubscribe( c_callback_t const& cb ) 
      {
         sub_t sub; 
         sub.func_id = cb; 

         unsubscribe( sub ); 
      }

      //------------------------------------------------------------------------
      template <typename OBJ_TYPE>
      void subscribe_method( OBJ_TYPE* obj, void (OBJ_TYPE::*mcb)( ARGS... ) )
      {
         sub_t sub; 
         sub.obj_id = obj; 
         sub.func_id = *(void const**) &mcb; 

         // std::bind
         // capture list
         sub.callable = [=]( ARGS ...args ) { (obj->*mcb)( args... ); };

         subscribe( sub ); 
      }

      //------------------------------------------------------------------------
      template <typename OBJ_TYPE>
      void unsubscribe_method( OBJ_TYPE* obj, void (OBJ_TYPE::*mcb)( ARGS... ) )
      {
         sub_t sub; 
         sub.obj_id = obj; 
         sub.func_id = *(void const**) &mcb; 

         unsubscribe( sub ); 
      }

      // void unsubscribe_object( void const* obj_ptr ); 
      // void unsubscribe_all(); 

      //------------------------------------------------------------------------
      // int args, int args
      void invoke( ARGS const& ...args )
      {
         for (sub_t& sub : m_subscriptions) {
            sub.callable( args... ); 
         }
      }

      void operator() ( ARGS const& ...args ) {  invoke( args... ); }

   private:
      //------------------------------------------------------------------------
      void subscribe( sub_t const& sub )       
      {
         m_subscriptions.push_back( sub ); 
      }

      //------------------------------------------------------------------------
      void unsubscribe( sub_t const& sub ) 
      {
         for (uint i = 0; i < m_subscriptions.size(); ++i) {
            if (m_subscriptions[i] == sub) {
               m_subscriptions.erase( m_subscriptions.begin() + i );
               return; 
            }
         }
      }

      std::vector<sub_t> m_subscriptions; 
};

//------------------------------------------------------------------------
void SubscribeTest( float t ) 
{
   Logf( "Test: %.2f", t ); 
}

//------------------------------------------------------------------------
void AddTest( int a, int b )
{
   int c = a + b; 
   Logf( "Test: %i + %i = %i", a, b, c ); 
}

class TestClass
{
   public:
      TestClass( int v ) 
         : m_value(v)
      {}

      void SomeMethod( int a, int b )
      {
         int c = a + b + m_value; 
         Logf( "Method: %i + %i + %i = %i", a, b, m_value, c ); 
      }

   private: 
      int m_value; 
};


//------------------------------------------------------------------------
UNITTEST( "delegate", "", TEST_ALWAYS ) 
{
   Delegate<float> update;
   Delegate<int, int> add;

   TestClass obj( 7 ); 

   update.subscribe( SubscribeTest ); 
   update.subscribe( SubscribeTest ); 
   update.unsubscribe( SubscribeTest ); 

   add.subscribe( AddTest ); 
   add.subscribe_method( &obj, &TestClass::SomeMethod ); 

   update( 3.14f ); 
   add( 4, 6 ); 

   Logf( "Test" ); 
   return true; 
}
