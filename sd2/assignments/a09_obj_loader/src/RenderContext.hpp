//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      // ...
      // Any Additional Helper Drawing Methods
      void DrawModel( Model *model ); 

   public:
      // ...
      GPUMesh* GetOrCreateMesh( std::string const &filename );                 // A09

   public:
      // ...
      std::map<std::string, GPUMesh*> m_meshDatabase;              // A09
}; 


