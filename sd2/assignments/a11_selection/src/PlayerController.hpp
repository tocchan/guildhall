class PlayerController
{
   public:
      void UpdateInput()
      {
         Game *camera = Game::GetCamera(); 
         Ray3 ray = camera->ScreenPointToWorldRay( GetMousePosition() ); 

         Map *map = Game::GetMap(); 

         float entityTime; 
         float mapTime; 

         Entity *ent = map->RaycastEntity( &entityTime, ray ); 
         if (map->RaycastTerrain( &mapTime, ray)) {
            if (mapTime < entityTime) {
               ent = nullptr; 
            }
         }

         Entity *selected = map->FindEntity( m_selectionHandle ); 
         if (selected == nullptr) {
            m_selectionHandle = GameHandle::INVALID; 
         }

         if (ent != nullptr) {
            ent->SetHoveredThisFrame(); 
            if (mouse->WasButtonPressed(0)) {
               m_selectionHandle = ent->GetHandle(); 
               ent->SetSelectedThisFrame(); 
               selected = ent; 
            }
         }

         if (selected != nullptr) {
            if (mouse->WasButtonPressed(1)) {
               MoveCommand *cmd = new MoveCommand(); 
               cmd->m_unit = m_selectionHandle;
               cmd->m_target = ray->evaluate(mapTime).xy();

               // g_theGame->EnqueueCommand( cmd ); 
               Game::GetInstance()->EnqueueCommand( cmd ); 
            }
         }
      }

   public:
      GameHandle m_selectionHandle; // std::vector<GameHandle> m_selection; 
}; 
