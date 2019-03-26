```cpp

// signature is not of great importance - just know we have a signature we have to "match"
typedef void (*event_cb)( std::string const &arg ); 

// We can subscribe something to it like this - both subscribe and unsubcribe
void EventSystemSubscribe( std::string const &event, event_cb cb )
void EventSystemUnsubscribe( std::string const &event, event_cb cb ); 

```

## std::function is not comparable

Easiest solution if we want to subscribe methods is to switch to using `std::function` instead of a raw function pointer - allowing us to capture methods, lambdas, and C style functions.  

**The problem** though is that `std::function` is not comparble, so we can't remove subscriptions by pointer.  For more reasons as to why, see Boost's explanation on why they didnt' implement it: https://www.boost.org/doc/libs/1_55_0/doc/html/function/faq.html#idp97012536   

*Note: Boost isn't std::function, but the rational is probably very similar.*


## Zen

"If you're willing to restrict the flexibility of your approach, you can almost always do something better"- Carmack

So if the official C++ guys can't do it, why in our hubris do we think we can?  Well - because we're not solving the general problem, we're going to solve a very specific problem - that is, we'll always assume we're only comparing two things; 

- An Object Pointer
- A function/method pointer; 

Pointers are comparable, and unique, thus allowing us to use them for our comparison; 


## Our `Function`

We're going to implement a `Function` type that we can register and call like a std::function, but that is comparable - so our needs are as follows; 

```cpp
// Note - this is a great use case for a templated class, so you can do this with 
// any kind of argument list, but for now let us keep it simple; 
class EventFunction 
{
   public:
      EventFunction( event_cb callback ); // works for C functions
      EventFunction( /* methods? */ );    // works for methods; 

      // comparable
      bool operator==( EventFunction const &other ) const; 
      bool operator!=( EventFunction const &other ) const; 

      // can be used like a normal std::function
      void operator()( std::string const &arg ) const; 
}
```

### Method Pointers
So method pointers are similar to object points - but the synatx is a little funky since methods must be called on an object, and the signature takes into the account the object they're being called on (to support virtual methods and the like)

So for example...

```cpp
class ObjType
{
   public:
      void SomeMethod( std::string const &arg ); 
};

void Foo()
{
   ObjType *obj_ptr = new ObjType(); 

   obj_ptr->SomeMethod( "test" ); // calling a method normally

   // creating a method pointer named `method_pointer`
   // notice the similarity between how we would define a function pointer and this
   // and the slight difference in having to include the ObjType)
   void (ObjType::*method_pointer)( std::string const& ); 

   // we can now set it like we would an object pointer
   method_pointer = &ObjType::SomeMethod; 

   // method_pointer( "test" ); // ERROR - DOES NOT WORK
                                // can't call a method pointer without an object

   // So to call it, we need the object to call it *on*
   // (notice the * before method_pointer)
   obj_ptr->*method_pointer( "test" ); // this will now call SomeMethod on obj_ptr
}
```


### Registering Methods && Comparing

So first, our interface for registering methods - and since to call a method, we both need the object and the method, we will need to store those two pieces of information with our class; 

And we'll try to go for an interface like this - limiting ourself to only thinking about pointers; 


```cpp
void Foo()
{
   ObjectType someObject; 
   EventFunction func = EventFunction( &someObject, &ObjectType::MethodOnObject ); 

   // call it!
   func( "test" ); 
}
```

Now, this poses a problem - in that the method pointer type is different depending on the object type.  This is a very C++ problem, so it has a very C++ solution; TEMPLATES!  You can create a templated constructor as follows; 

```cpp
class EventFunction 
{
   public:
      //... 
      template <typename T>
      EventFunction( T *obj, void (T::*method_ptr)( std::string const& ) )
      {
         // so to call method_ptr, I would need to do...
         // obj->*method_ptr( "arguments" );  

         // welll, I can guess I'll need to store pointers
         // to these things so call them later, so let's do that for now;
         m_object_pointer = obj; 
         m_function_pointer = &method_ptr; 

         // if this is just a normal C function - what changes?
      }

   private:
      // objects and methods are just addresses in 
      // memory - so all types can eventually case downt thos this;
      void *m_object_pointer;          
      void const *m_function_pointer; 
}; 

// Them being void* allows for an easy comparison
bool EventFunction::operator==( EventFunction const &other ) 
{
   return (m_object_pointer == other.m_object_pointer)
       && (m_function_pointer == other.m_function_pointer); 
}
```

## How In the World do we Call it?

### Quick Version (using `std::function` as a backer)
Quickest way to just get something working is to leverage the STL to do the heavy lifting after we get the information we need to do the comparison; 

We'll be using a lamda to store a method in the function, and just invoke that std::function when I need to; 

```cpp
class EventFunction 
{
   public:
      //... 
      template <typename T>
      EventFunction( T *obj, void (T::*method_ptr)( std::string const& ) )
      {
         // save data so we can compare
         m_object_pointer = obj; 
         m_function_pointer = &method_ptr; 

         // we will use a lambda with capture
         // to allow us to call this method in the future; 
         m_func = [=]( std::string const &args ) { obj->*method_ptr(args); }; 

         // Question: If this was for the normal C function, what would I set m_func to?
         // (it is much simpler)
      }

      //...
      // calling it
      void operator()( std::string const &arg ) const 
      {
         m_func( arg ); // call the function
      }

   private:
      // ...
      std::function<void(std::string const&)> m_func; // a function matching our event signature
};  
```


### Learning Version (tricks with templates)
So the difficult problem ahead of us is to figure out how to use the raw memory pointers we have to call the appropriate function/callback; 

So a nice trick of templates is to use the function signature itself to carry some extra information;  For example;

```cpp
template <typename SOME_TYPE>
int Add( int a, int b )
{
   return a + b; 
}
// notice that above class doesn't use or reference SOME_TYPE

void Foo()
{
   // these do the same thing but they are uniquely different functions; 
   int s0 = Add<int>(1, 2);    
   int s1 = Add<double>( 1, 2 ); 

   // &Add<int> != &Add<double>
}
```

This allows us to have a fixed type function but still have some type information for us to use - this is going to be the crutch we'll be using for this problem; 

// ... more to come - need to head into work...


