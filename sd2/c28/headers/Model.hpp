//------------------------------------------------------------------------
// Model.hpp
//------------------------------------------------------------------------
class Model          // A09
{
   public: 
      Model::Model( RenderContext *ctx, char const *meshName ); 

   public:
      GPUMesh *m_mesh; 
      Material *m_material; 
      mat44 m_modelMatrix;  
};

//------------------------------------------------------------------------
// Model.cpp
//------------------------------------------------------------------------
Model::Model( RenderContext *ctx, char const *meshName )
{
   m_mesh = ctx->GetOrCreateMesh( meshName ); 
   if (m_mesh != nullptr) {
      m_material = ctx->GetOrCreateMaterial( m_mesh->GetDefaultMaterialName() ); 
   }

   m_modelMatrix = mat44::IDENTITY; 
}