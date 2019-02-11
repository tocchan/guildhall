//-------------------------------------------------------------------
class DevConsole
{
   public:

      // handle arrows, enter, escape, backspace, delete, etc...
      void HandleKeyUp( uint vkKeyCode );          // WM_KEYUP
      void HandleKeyDown( uint vkKeyCode );        // WM_KEYDOWN

      // insert into string; 
      void HandleCharacter( uint charCode );       // WM_CHAR

   public:
      std::string m_currentInput;  // "clear"
      int m_carotPosition; 
}; 

//-------------------------------------------------------------------
bool DevConsole::HandleKeyDown( uint vkKeyCode )
{
   if (IsClosed()) {
      // may want to open if open key was pressed; 
      if (vkKeyCode == OPEN_KEY) {
         Open(); 
      }

      return IsOpen(); // 
   }

   if (vkKeyCode == VK_RETURN) {
      if (!IsEmpty(m_currentInput)) {
         RunCommand( m_currentInput ); 
         ClearInput(); 
      }
   } else if...
}

//-------------------------------------------------------------------
bool DevConsole::HandleCharacter( uint charCode )
{
   // bulk of code goes here; 
}