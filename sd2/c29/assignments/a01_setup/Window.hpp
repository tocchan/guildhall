class Window
{
   public:
      Window();
      ~Window(); 

      // SD2.A01
      void SetInputSystem( InputSystem* inputSystem )

      // SD2.A01
      void Open( std::string const& title, float aspect, float percentOfHeight );
      void Close(); 

      // SD2.A01
      void BeginFrame(); // process window messages

   private:
      void* m_hwnd                  = nullptr; 

      // input system the window feeds into
      InputSystem* m_inputSystem    = nullptr; 

      // You may need to add more to make things work; 
};