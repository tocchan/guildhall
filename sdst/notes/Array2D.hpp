//------------------------------------------------------------------------
// NOT NECESSARY - but very helpful for us working with grids; 
// as we'll be accessing using 2d coordinates, and this can do 
// smoe additional bounds checking for us; 
//------------------------------------------------------------------------
template <typename TYPE>
class Array2D
{
   public:
      Array2D(); 
      Array2D( ivec2 size, T const& value ); 

      void init( ivec2 size, T const& value ); 

      void set( ivec2 cell, T const& value ); 
      void set_all( T const& value ); 

      TYPE& get( ivec2 cell ); 
      TYPE try_get( ivec2 cell, T const& def_value ); 
      TYPE const& get( ivec2 cell ) const; 

      // helper accessors
      int get_width() const; 
      int get_height() const; 
      ivec2 get_size() const; 
      bool contains_cell( ivec2 cell ) const; // good for debugging

      inline TYPE& operator[]( ivec2 cell )              { return get(cell); }
      inline TYPE const& operator[]( ivec2 cell ) const  { return get(cell); }

   public:
      uint get_index( ivec2 cell ) const; 

   private: 
      ivec2 m_size; 
      std::vector<DATA> m_data; // if you want more of an excercise - use DATA* m_data and manage the memory yourself (including copy/move/etc...)

};
