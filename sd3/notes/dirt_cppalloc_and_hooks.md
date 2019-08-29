## C++ Allocators

```cpp
// An allocator will exist on a container by value - ie, each
// container has its own instance of this allocator - but usually 
// we just treat this as a namespace/static and don't use much internal state; 
template <typename T>
struct custom_allocator 
{
   custom_allocator() = default; 

   template <class U> 
   constexpr custom_allocator( custom_allocator<U> const& ) noexcept {}

   // allocator needs to define these types;
   // the "type" is not as important as the name
   // the stdlib is expecting these names - remember templates are a form of duck-typing
   // these three are fairly standard
   typedef T               value_type; 
   typedef size_t          size_type; 
   typedef std::ptrdiff_t  difference_type; 

   // give hints on how the allocator is implemented so that containers can optmize around it 
   typedef std::true_type  propagate_on_container_move_assignment;   // when moving - does the allocator local state move with it?
   typedef std::true_type  is_always_equal;                          // can optimize some containers (allocator of this type is always equal to others of its type)                         

   // 
   T* allocate( size_t byte_size )              { return (T*) ::malloc( sizeof(T) ); }
   void deallocate( T* ptr, size_t byte_size )  { ::free(ptr); }
};

template<typename T, class U>
bool operator==( custom_allocator<T> const&, custom_allocator<U> const& ) { return true; }

template<typename T, class U>
bool operator!=( custom_allocator<T> const&, custom_allocator<U> const& ) { return false; }
```

```cpp
static std::vector<int, untracked_allocator<int>> gItems; 
```

```cpp
struct System::Allocator 
{
	void* (*fbx_alloc)( size_t ); 
	void (*fbx_free)( void* ptr ); 
}
```

## Executable Data (NEVER EVER DO THIS)
So a function is just a pointer to data that is marked as executable.  

So I can get valid byte-code - I can makr that buffer as executable, and run it as if it were a function; 

```cpp
UNITTEST( "badness", nullptr, 0 )
{
   // this just happens to be valid x64 code; 
   static byte opcodes[] = { 
      0x89, 0x54, 0x24, 0x10, 0x89, 0x4c, 0x24, 0x08, 
      0x57, 0x48, 0x83, 0xec, 0x20, 0x48, 0x8b, 0xfc, 
      0xb9, 0x08, 0x00, 0x00, 0x00, 0xb8, 0xcc, 0xcc, 
      0xcc, 0xcc, 0xf3, 0xab, 0x8b, 0x4c, 0x24, 0x30, 
      0x48, 0x8d, 0x0d, 0xd3, 0xcf, 0x04, 0x00, 0x90, 
      0x90, 0x90, 0x90, 0x90, 0x8b, 0x44, 0x24, 0x38, 
      0x8b, 0x4c, 0x24, 0x30, 0x03, 0xc8, 0x8b, 0xc1, 
      0x48, 0x83, 0xc4, 0x20, 0x5f, 0xc3 
   }; 

   // by default, data memory isn't executable for security reasons,
   // but we can change that; 
   DWORD old_rights; 
   ::VirtualProtect( opcodes, sizeof(opcodes), PAGE_EXECUTE_WRITECOPY, &old_rights ); 

   // then, we just case that memory address to a function
   // compatible signature and call it;    
   op_cb op = (op_cb)(void*)opcodes; 
   int res = op( 7, 10 ); 
   ASSERT( res == 17 ); 

   // clean up (mark as non-executable)
   DWORD unused; 
   ::VirtualProtect( opcodes, sizeof(opcodes), old_rights, &unused ); 
   return (val == 5); 
}
```

The above bytecode was the following function compiled in `DEBUG` with 
some operations **NOP**d (**N**o-**OP**eration, `0x90`) out. 
```cpp
   static int Op( int a, int b )
   {
      return a + b; 
   }
```


## Function Hooking & Trampolines

### Hooking a Function
Overwriting executable code so that when another function is called, it
will instead call my function; 

```cpp 
// Example
void OutputStringA( char const* );  // windows function - I dont own the source
void CustomLog( char const* str ) 	// function I want to call instead that I own
{
	printf( str ); 
}

void Startup()
{
	Hook( OutputStringA, CustomLog );

	OutputStringA( "Hello World" ); // now calls CustomLog instead 
}
```

This works by overwriting the first few instructions of the code at OutputStringA with a jump
to our function.  I recommend using a 3rd party library, like `MinHook` if you want to do this, but here is some example code for how it works; 

```cpp


#include "core/platform/win32.h"
#include <malloc.h>

class FunctionHook 
{
   public:
      // these must have the same signature
      bool set( void* original, void* hook_func ) 
      {
      	// save off address of the function I'm overwriting; 
         m_original_function = original;

         // Where do I want to go?
         m_jump_address = (intptr_t)hook_func; 
         
         // Do an initial hook (make original now call hook_func)
         return hook(); 
      }

      bool hook()
      {
         // Windows - Make the code memory writable so I can change it; 
         DWORD old_rights; 
         bool can_edit = ::VirtualProtect( m_original_function, 64, PAGE_EXECUTE_WRITECOPY, &old_rights );  
         if (!can_edit) {
            return false; 
         }

         // save off what the code was so I can return to it in ::unhook
         MemCopy( m_original_code, m_original_function, sizeof(m_original_code) ); 


         // Do a long jump - in x64 assembly 
         // the assembly will look like this; 
         //   UINT8  opcode0;     // FF25 00000000: JMP [+6]
         //   UINT8  opcode1;
         //   UINT32 dummy;       
         //   UINT64 address;     // Absolute destination address
         byte* bc = (byte*)m_original_function; 
         bc[0] = 0xff; 
         bc[1] = 0x25; 
         memset( bc + 2, 0, sizeof(uint) ); 
         memcpy( bc + 6, &m_jump_address, sizeof(m_jump_address) ); 

         // return the code to un-writable; 
         DWORD unused; 
         VirtualProtect( m_original_function, 64, old_rights, &unused ); 
         
         return true; 
      }

      void unhook()
      {
         // make as writable
         DWORD old_rights; 
         ::VirtualProtect( m_original_function, 64, PAGE_EXECUTE_WRITECOPY, &old_rights );  
         
         // return to what it was
         MemCopy( m_original_function, m_original_code, sizeof(m_original_code) );
         
         // return to previous rights
         DWORD unused; 
         VirtualProtect( m_original_function, 64, old_rights  , &unused ); 
      }

   public: 
      void* m_original_function; 
      byte m_original_code[32];
      int64_t m_jump_address; 
}; 

UNITTEST("hooktest", nullptr, 0) 
{
   FunctionHook hook; 
   hook.set( OutputStringA, CustomLog );

   OutputStringA( "Test" ); 

   gMallocHook.unhook(); 
   return true; 
}
```

### Trampoline
A trampoline is a form of hook that will...

1. Hook the function so that it calls the desired function
2. Unhook and call the original function
3. Rehook
4. Return result of original function (possibly modified)

Name comes from bouncing out, back into, and back out before returning to the original function. 


