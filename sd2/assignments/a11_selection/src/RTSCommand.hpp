//------------------------------------------------------------------------
// hpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RTSCommand 
{
   public:
      virtual ~RTSCommand() {}
      virtual void Execute() = 0; 
};

//------------------------------------------------------------------------
class CreateEntityCommand 
{
   public:
      virtual void Execute(); 

   public:
      vec2 m_createPosition; 
}; 

//------------------------------------------------------------------------
class MoveCommand 
{
   public:
      virtual void Execute(); 

   public:
      GameHandle m_unit; 
      vec2 m_position; 
};

//------------------------------------------------------------------------
// cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
VIRTUAL void CreateEntityCommand::Execute()
{
   Map *map = Game::GetMap();
   map->CreateEntity( m_createPosition ); 
}

//------------------------------------------------------------------------
VIRTUAL void MoveCommand::Execute()
{
   Map *map = Game::GetMap();
   Entity *ent = map->FindEntity( m_unit ); 
   if (ent != nullptr) {
      ent->MoveTo( m_position ); 
   }
}