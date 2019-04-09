struct MapTile
{
   // empty for now; 
   int m_placeholder; 
}; 


/** For a 2x2 map, the vertices will look like (each + is a unique vertex)
    This will give us more painting fidelity, and visual fidelity if we choose to futz with 
    height

  +---+---+---+---+
  | / | / | / | / |
1 +---+---+---+---+
  | / | / | / | / |
  +---+---+---+---+
  | / | / | / | / |
0 +---+---+---+---+
   | / | / | / | / |
^ +---+---+---+---+
y     0       1
  x->


 The map will be setup such that X -> Right, Y -> Up.  So that we can keep with a left handed basis and not deal 
 with the change of basis going into the rendering system, we'll just assume Z -> down (instead of the more intuitive Z -> up)

 Each tile is 1 unit length in the world (so each quad is half length)
 Each tile is one UV of texture (so each quad is half a uv) - be sure to use a wrapping sampler for terrain for this; 


**/
  

class Map
{
   public:
      bool Load( char const *filename );              // just call into crate with 64x64 for now; 
      bool Create( int tileWidth, int tileHeight ); 

      void Update(); // nothing yet;
      void Render(); // assumes a camera is already bound

      public RenderTerrain( Material *matOverride = nullptr ); // render the terrain, be able to provide a custom material for debug purposes

      // Accessors
      aabb2 GetXYBounds() const; // used for constraining the camera's focal point

   public: // internal helpers
      void GenerateTerrainMesh(); // creates the mesh and material from the tiles; 

   public: 
      ivec2 m_tileDimensions; // how many tiles X and Y
      ivec2 m_vertDimensions; // how many verts X and Y

      // use a std::vector if you prefer for these containers; 
      MapTile *m_tiles        = nullptr;  // logical representation
      VertexLit *m_vertices   = nullptr;  // visual represenation

      GPUMesh *m_terrainMesh; 
      Material *m_terrainMaterial; 
}; 