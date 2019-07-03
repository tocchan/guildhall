PATHING
======

## Overview Steps: 
- Goal
- Dijkstra Example 
  - Why no A*?
- Data Structures
- Feeding the Beast
- Having a unit follow a path; 
- Shortcutting
  - Grid Raycasting

- Enemy Commander
  - Seeding the map
  - Some basic behaviour

- Array2D (if time)
  - `Array2D` iterator
  - `irect` iterator;  
- A* (if time)


## End Goal
```cpp
//------------------------------------------------------------------------
// Interface we're going to give our Unit
//------------------------------------------------------------------------
class Unit 
{
   public:
      void PathTo( vec2 position );    // path to a specific location
      void PathTo( Unit* target );     // path to a particular target (building/tree)

};
```


## Dijkstra Example
-- in class --

### Why no A\*?
Turns out - Dijkstra for multiple-target pathing just works out better, and felt more intuitive to understand.  The other point - it jus didn't end up being slow enough to matter.

The other nice part of Dijkstra is the results can be shared - which for a lot of the common paths (go gather/return to base), can be a big win; 

If you want to add a A\* solver to the `PathSolver`, is is a very minimal step addition.   


## Data Structures
```cpp
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
// see: Array2D.hpp for interface for that
// Not a necessary add - could easily be done with std::vector; 
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

## Feeding the Beast
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

## Following a Path
So, my intended use case is my tasks will each frame likely want to `PathTo` a target, so I'm going to make the `PathTo` be kind to when I'm repathing to a new target; 

```cpp
bool Unit::PathTo( vec2 target ) 
{
   // 1. Determine if I have a path that works for this target; 
   // 2. If not, clear the current path if I have one
   // 3. If no path, create path for the target
   // 4. If path...
   //    4a. Follow path
   //    5b. return true; 
   // 5. else if no path...
   //    5a. Just move toward the target (like before)
   //    5b. Return false (could not get there)
} 
```

Most of the above should be fairly straight foward, outside of `FollowPath`, so let's look into that...

```cpp
// Should be private - as it should only be used internally;
void Unit::FollowPath()
{
   ASSERT( m_path != nullptr ); // if we're here, we better have had a path

   // if I have reached the next node on the path...
   //    if more nodes... advance target node in path
   //    if no more nodes... move to exact target within node
   // else 
   //    move toward target node
   //    if I end up being blocked - clear the path (causing repath next frame)
}
```

*Note: If you can move REALLY quickly, and can pass multiple nodes in a frame, you may want call this and keep track of how much movement was used.  Move until you reach the destination or you run out of distance for this frame*

**TODO** 
So what members does our Unit need to follow this path?
- ...


`PathTo( Entity* )` is similar, though when first calcultaing the path, you path to all tiles the building occupies, and pick the best one. 


## Path Shortcutting
The above paths will look fairly unnatural, but should work;

Since we're a realspace game, and are allowed to move any direction - what we want to do is try to skip forward along the path as far as possible as long as we can take a direct line unobstructed to that target; 

One way to accomplish this is that every time we're about to advance our target ndoe forward, we instead raycast to our end-goal and see if we can go there directly.  If that files, we step back along out path one node and repeat.  Keep going until we are at our next node or we find a shortcut.

Mark the shortcut node as our next node - which will cause our unit to move directly there. 

Every frame, recommend repeating the process, so see if a new path opens up (start at the end, and move back to our next target). 

If you're worried about speed, you can do one step of this each frame, keeping track of the last shortcut attempted.  

**TODO: Grid Raycast**
```cpp
```

## Enemy Commander
You have a `PlayerCommander` or `PlayerController` currently.

This task is make another `AIController` or `AICommander`, and give them a common base.  When the game starts up, assign an `AICommander` to the other team and give him some starting units/buildings.  

Be sure he only communicates with the game through commands.  

Some refactoring of what you should move from `PlayerCommander` to the base `Commander` so that `AICommander` can leverage will need to happen.  In my experience, it was very minimal (team, ability to issue commands, and an update), but your designs may vary; 

The AI presented in the assignment should be fairly short to implement.  If you find it taking longer - take a look at what sort of helper methods you can add to `Game`/`Map`/`Commander` etc to make it go quicker; 