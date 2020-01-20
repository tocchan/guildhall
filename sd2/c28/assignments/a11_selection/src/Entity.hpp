
enum eEntityFlagBit : uint
{
   ENTITY_DESTROYED_BIT    = BIT_FLAG(0),    // object is marked for destruction
   ENTITY_SELECTABLE_BIT   = BIT_FLAG(1),    // entity is selectable by the player
}; 
typedef uint eEntityFlags; 

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Entity
{
   public:
      void Update(); 

      void Destroy(); 
      void SetSelectable( bool isSelectable ); 

      bool IsDestroyed() const; 
      bool IsSelectable() const; 

      void SetPosition( vec2 pos ); 
      void MoveTo( vec2 target ); 
      vec2 GetPosition() const; 

      // return just one result if it hits - return the closest result
      // dependingon its physics collision
      bool Raycast( float *out, ray3 ray ) const; 

   public:
      GameHandle m_handle; 
      eEntityFlags m_flags; 

      // info
      vec2 m_position; 
      vec2 m_targetPosition; 

      // stats
      float m_speed;

      // UI collision
      float m_height; 
      float m_radius; 
}; 