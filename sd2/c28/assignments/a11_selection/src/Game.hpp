//------------------------------------------------------------------------
class Game
{
   public:
      void EnqueueCommand( RTSCommand *command ); 
      void ProcessCommands(); // process and free up memory 
      void ClearCommands();   // just free up memory 

   private:
      std::queue<RTSCommand*> m_commandQueue; 
}; 

//------------------------------------------------------------------------
void Game::ProcessCommands()
{
   RTSCommand *cmd;
   while (m_commandQueue.dequeue( &cmd )) {
      cmd->Execute(); 
      delete cmd; 
   }
}