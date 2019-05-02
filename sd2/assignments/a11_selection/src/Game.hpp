class Game
{
   public:
      void EnqueueCommand( RTSCommand *command ); 
      void ProcessCommands(); // process and free up memory 
      void ClearCommands();   // just free up memory 

   private:
      queue<RTSCommand*> m_commandQueue; 
}; 