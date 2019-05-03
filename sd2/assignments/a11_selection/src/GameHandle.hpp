class GameHandle
{
   public:
      GameHandle(); 
      explicit GameHandle( uint id );
      GameHandle( uint cyclicID, uint index ); 

      uint GetIndex() const; 

      bool operator==( GameHandle const &other ) const; 
      bool operator!=( GameHandle const &other ) const; 
      
   public: // STATICS
      static GameHandle INVALID; // = GameHandle(0);  So be sure never to use the cyclicID 0.

   private:
      uint m_data = 0; 
};

GameHandle::GameHandle( uint cyclicID, uint index ) 
{
   ASSERT( index <= 0x0000ffff ); 
   ASSERT( cyclicID <= 0x0000ffff ); 

   // So I want cyclicID to be in high-word (high 16 bits)
   // I want index in the lo-word (low 16 bits); 
   uint hiword = cyclicID << 16; 
   m_data = hiword | index; 
}

