//------------------------------------------------------------------------
// GPUMesh.hpp
//------------------------------------------------------------------------

class GPUMesh                            // A04
{
   public:
      // ...
      inline std::string const& GetDefaultMaterialName() const    { return m_defaultMaterial; } // A09

   public: 
      // ...
      std::string m_defaultMaterial;     // A09 - material to use when creating a model
};

