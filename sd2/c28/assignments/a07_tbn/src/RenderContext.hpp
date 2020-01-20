//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      //...

      void BindMaterial( Material *mat );                                  // A07
      //...

   public:
      //...
      Material* GetOrCreateMaterial( std::string const &filename );        // A07

   public:
      //...
      std::map<std::string, Material*> m_materialDatabase;                 // A07
}; 

//------------------------------------------------------------------------
// .cpp
//------------------------------------------------------------------------

void RenderContext::BindMaterial( Material *mat ) 
{
   // Bind the shader
   // ...

   // Bind the texture views
   // ...

   // Bind the samplers
   // ...

   // Bind the material (user) constant buffer if available
   // ...
}

