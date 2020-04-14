class Shader
{
   public:
      ShaderProgram* m_program;

      eBlendMode m_blend_mode;
      eCullMode m_culling;
      eWindOrder m_winding;
      eFillMode m_fill_mode; 

      bool m_depth_enable
      eCompareOp m_depth_test; 
};

// Let you have...

void RenderContext::BindShader( Shader* shader )
{
   BindShaderProgram( shader->GetProgram() ); 
   SetCullMode( shader->GetCullMode() ); 
   // so on and so forth...
}


class Material
{
   public:
      Shader* m_shader; 

      RenderBuffer* m_data; // may be null if material doesn't need it
      std::vector<Texture*> m_textures;
};

// Let you have...
void RenderContext::BindMaterial( Material* mat )
{
   BindShader( mat->GetShader() ); 

   // bind ubo if we have one
   // ... 

   // bind all textures
   // ...
}