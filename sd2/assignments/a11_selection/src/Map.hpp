//------------------------------------------------------------------------
// hpp
//------------------------------------------------------------------------
class Map
{
   public:
      void Think(); // Update all entities
      void Render(); // render map and entities; 

      // terrain methods
      // ...

      // entity methods
      Entity* CreateEntity( vec2 pos ); 
      Entity* FindEntity( GameHandle handle ) const; 

      // Pick
      Entity* RaycastEntity( float *hitTime, Ray3 ray, float maxDistance = INFINITY ); 
      bool RaycastTerrain( float *hitTime, Ray3 ray, float maxDistance = INFINITY ); 


   private:
      void PurgeDestroyedEntities();   // cleanup destroyed entities, freeing up the slots; 

      uint GetFreeEntityIndex() const; // return a free entity slot
      uint GetNextCyclicID() const     // gets the next hi-word to use, skipping '0'

   private:
      // map terrain data
      // ...

      // map entity data
      std::vector<Entity*> m_entities; 
      uint16 m_cyclicID; // used for generating the GameHandle
}; 


//------------------------------------------------------------------------
// cpp
//------------------------------------------------------------------------
//------------------------------------------------------------------------
Entity* Map::CreateEntity()
{
   uint slot = GetFreeEntityIndex(); 
   uint cyclicID = GetNextCyclicID(); 

   GameHandle handle = GameHandle( cyclicID, slot ); 
   Entity *ent = new Entity();
   ent->m_handle = handle; 

   // setup
   // ...

   // you may have to grow this vector...
   m_entities[slot] = ent; 
   return ent; 
}

//------------------------------------------------------------------------
Entity* Map::FindEntity( GameHandle handle ) 
{
   uint slot = handle.GetIndex(); 
   Entity *ent = m_entities[slot]; 

   // we only return the entity if it matches the handle
   if ((ent != nullptr) && (ent->GetHandle() == handle)) {
      return ent; 
   } else {
      return nullptr; 
   }
}

//------------------------------------------------------------------------
Entity* Map::RaycastEntity( float *hitTime, Ray3 ray, float maxDistance = INFINITY )
{
   Entity *bestEntity = nullptr; 
   float bestTime = INFINITY; 

   foreach (Entity *ent in mEntities) {
      float time; 
      if (ent->IsSelectable() && ent->Raycast( &time, ray )) {
         if ((time >= 0.0f) && (time <= maxDistance) && (time < bestHitTime)) {
            bestEntity = ent; 
            bestHitTime = time; 
         }
      }
   }

   *hitTime = bestHitTime; 
   return bestEntity; 
}



