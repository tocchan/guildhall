```cpp

//------------------------------------------------------------------------
// Interface we're going to give out Entity
//------------------------------------------------------------------------
class Unit 
{
   public:
      void PathTo( vec2 position );    // path to a specific location
      void PathTo( Entity* target );   // path to a particular target (building/tree)

};


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

//------------------------------------------------------------------------
// Pathing 
//------------------------------------------------------------------------

// you can do this algorithm without
// these state variables, but they're helpful 
// for debugging
// (for example, you can make sure you don't re-visit a finished node due to a bad hueristic)
enum ePathState
{
   PATH_STATE_UNVISITED = 0, 
   PATH_STATE_VISITED, 
   PATH_STATE_FINISHED,
};

struct path_info_t
{
   float cost     = INFINITY;  // how much did it cost to reach this point
   ivec2 parent   = ivec2(-1); // who was I visisted by (root/seed points have no parent) 
                               // useful for computing a path (though you can also "fall downhill" without this - just involves more checks)
   ePathState state = PATH_STATE_UNVISITED; 
}; 

// some common types we'll be using for this
typedef Arrray2D<float> TileCosts; 
typedef Array2D<path_info_t> PathInfo; 
typedef std::vector<ivec2> Path; 

// The game facing object; 
class Pather
{
   public:
      void init( ivec2 size, float initial_cost ); 

      // start of frame, will clear costs out...
      void set_all( float cost ); 

      // ...and add costs for all objects
      void set_cost( ivec2 tile, float cost ); 
      void add_cost( ivec2 tile, float cost ); 

      // ask for paths (to single destination -or- multiple destinations)
      // Like the pather being in charge of path memory here as it gives
      // us more options for threading/optimizing later; 
      Path* alloc_path( ivec2 start, ivec2 end ); 
      Path* alloc_path( ivec2 start, std::vector<ivec2> const& ends ); 
      void free_path( Path* path ); 


   public:
      TileCosts m_costs; 
}; 

// Not necessary, but can help for debugging and cleaning up the code; 
// This class uses a pather (or pather data) to solve a specific path or 
// distance field calculation.  Can be stepped
class PathSolver
{
   public:
      // distance field work; 
      void start_distance_field( Pather* pather ); 
      void add_end( ivec2 tile );                    // a seed (or destination) is where we'll flood fill from
      void add_start( ivec2 tile ); 

      bool step();  // useful for debugging if you want to step the solver and see what it is doing; 
      void solve(); // run all steps; 

      // returns a path for a given start point to the destination
      // (will fail if the distance field didn't reach this tile)
      bool get_path( Path* out, uvec2 tile ); 

   private:
      // visit method if using distance field solver; 
      void visit_dijkstra( ivec2 cell, ivec2 parent, float parent_cost ); 

   private: 
      Pather* m_pather; 
      std::vector<ivec2> m_visited; 
      std::vector<ivec2> m_termination_points; // used for ending early if you have a start point in mind; 

      PathInfo m_path_info;   // will effectively contain a distance field - what this solver is filling; 
};
```

```cpp
// Pather Map::m_pather; 
void Map::Update()
{
   // prepare path info for frame to use
   m_pather.init( get_size(), 1.0f ); 

   // for each object that is a blocker (your static objects), mark those are unable to get into
   foreach (static_unit in m_units) {
      foreach (ivec2 tile : tiles unit takes up) {
         m_pather.set( tile, SOME_BIG_VALUE ); // some big value being like, 1000 - something that makes the pather avoid the tile due to cost; 
         // we don't use INFINITY since if we are pathing to a blocked tile, we want the unit to still approach it, even if it can't get into it; 
      }
   }
   // done preparing pather
}
```
