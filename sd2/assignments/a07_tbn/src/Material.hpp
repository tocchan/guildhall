//------------------------------------------------------------------------
// Material.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Material
{
   public:
      Material(); 
      Material( XMLNode *xml ); 
      ~Material();

      void SetShader( Shader *shader ); 
      void SetShader( char const *shader_name ); 

      void SetTextureView( uint slot, TextureView *view ); 
      void SetTextureView( uint slot, char const *name );

      void SetSampler( uint slot, Sampler *sampler ); 

      TextureView* GetTextureView( uint slot ) const; 
      Sampler* GetSampler( uint slot ) const; 

      // Convenience Helpers - meaning it can pick a sane default when set to nullptr;
      void SetDiffuseMap( TextureView *view );  // default to "white" on nullptr;
      void SetNormalMap( TextureView *view );   // default to "flat" on nullptr;
      void SetSpecularMap( TextureView *view ); // default to "white" on nullptr;
      void SetEmissiveMap( textureView *view ); // default to "black" on nullptr; 

      void SetUniforms( void const *data, size_t const size );
      void SetUniforms( UniformBuffer *ubo ); 
      UniformBuffer* GetUniformBuffer() const;

      // templated SetUniforms could be useful as well
      // ...

   public:
      Shader *m_shader; 
      std::vector<TextureView*> m_textures; 
      std::vector<Sampler*> m_samplers; 
};

#endif 
