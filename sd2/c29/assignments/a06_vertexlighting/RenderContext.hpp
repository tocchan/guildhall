
// new structs for lighting (internal or external)
// this should match GPU light_t (if you update one, update the other)
struct light_t
{
   vec3 position; 
   float pad00; 
   vec3 color;
   float intensity; 
};


// DEFAULT shader - IS NOT A LIT SHADER

class RenderContext
{
   public:
      void SetDiffuseTexture( Texture* tex ); // if nullptr, what are you doing?  You bind white!
      void SetNormalTexture( Texture* tex ); // if nullptr, what do you think we should bind?  (.5, .5, 1) - "flat"

      // Lighting Methods
      void SetAmbientColor( RGBA color ); 
      void SetAmbientIntensity( float intensity ); 
      void SetAmbientLight( RGBA color, float intensity ); 
      // disabling Ambient is the same as doing...  SetAmbient( WHITE, 1.0f ); 

      // for now, assume idx is 0, we only have one light
      void EnableLight( uint idx, light_t const& lightInfo ); 
      // void EnablePointLight( uint idx, vec3 position, rgba color, float intensity, vec3 attenuation ); 
      void DisableLight( uint idx ); 
      // disabling a light is the same as just saying the light has 0 intensity


   private:
      // new data for lighting
};


//---------------------------------------------------------------------------
// End Goal
//---------------------------------------------------------------------------
m_renderer->BeginCamera( m_game_camera ); 

// setup for rendering opaque boecjts
m_renderer->SetCullMode( CULL_BACK, WIND_COUNTER_CLOCKWISE ); 
m_renderer->SetDepthMode( COMPARE_GREATER_EQUAL, true ); 

// setup lights for the scene
m_renderer->DisableAllLights(); 
m_renderer->SetAmbientLight( m_ambient_light, m_ambient_light.a ); 
m_renderer->EnableLight( 0, m_point_light ); 

// set to use our lit shader
m_renderer->BindShaderByName( "shaders/lit" ); 

// bind our textures for the object
m_renderer->BindSamplerByMode( 0, SAMPLE_LINEAR ); 
m_renderer->BindDiffuseTextureByName( m_diffuse_map.c_str() ); 

// bind the model matrix & draw
m_renderer->SetModelMatrix( m_plane_model ); 
m_renderer->DrawMesh( m_mesh_plane ); 

m_renderer->EndCamera(); 


class GameTile
{
   public:
      void Render()
      {
         m_renderer->SetSpecularConstants( m_spec_factor, m_spec_power ); 
         m_renderer->SetModelMatrix( m_transform.GetAsMatrix() ); 

         // bind all textures
         // ...

         m_renderer->DrawMesh( m_mesh ); 
      }

   public:
      // rendering information
      Transform3D m_transform; // model matrix
      GPUMesh* m_mesh; 

      Texture* m_diffuse;
      Texture* m_normal;
      Texture* m_specular; 

      float m_spec_factor;
      float m_spec_power;
}; 