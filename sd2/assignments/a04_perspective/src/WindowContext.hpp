#pragma once

#include <stdint.h>
#include <string>


typedef unsigned int uint; 
typedef bool (*windows_proc_cb)( void*, uint msg, uint wparam, uint lparam ); 


//---------------------------------------------------------------
//---------------------------------------------------------------
class WindowContext
{
   public:
      WindowContext();
      ~WindowContext(); 

      void Create( std::string const &name, float aspect, float percent, windows_proc_cb cb ); 
      void Close(); 

      void BeginFrame(); 

   public:
      void *m_hwnd; // void* if you're avoiding Windows.h
      windows_proc_cb m_gameCB;  
};

