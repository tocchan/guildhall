#pragma once

#include <stdint.h>
#include <string>

#include "Engine/Math/IntVec2.hpp"

enum eMouseMode
{
   MOUSE_MODE_ABSOLUTE, 
   MOUSE_MODE_RELATIVE, 
};


typedef unsigned int uint; 
typedef bool (*windows_proc_cb)( void*, uint msg, uintptr_t wparam, uintptr_t lparam ); 


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

      // Utility
      bool HasFocus() const; 
      inline bool IsOpen() const             { return m_hwnd != nullptr; }
      
      IntVec2 GetClientCenter() const; 
      
      // Mouse Position Information 
      IntVec2 GetClientMousePosition(); 
      void SetClientMousePosition( IntVec2 &clientPos ); 
      
      IntVec2 GetClientMouseRelativeMovement(); 

      // Contorlling mouse optins; 
      void LockMouse(); 
      void UnlockMouse(); 
      
      void ShowMouse(); 
      void HideMouse();

      inline bool IsMouseLocked() const      { return m_mouseLockCount > 0; }

      void SetMouseMode( eMouseMode mode ); 

   public:
      void *m_hwnd; // void* if you're avoiding Windows.h
      windows_proc_cb m_gameCB;  

      eMouseMode m_mouseMode     = MOUSE_MODE_ABSOLUTE; 
      IntVec2 m_lastFrameMousePosition; 
      IntVec2 m_currentMousePosition; 
      int m_mouseLockCount       = 0; 
};

