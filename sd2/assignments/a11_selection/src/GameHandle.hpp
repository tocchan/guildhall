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