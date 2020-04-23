```cpp

void Game::Render()
{
   // shader properties
   rc->BindShaderProgramByName( "data/shaders/lit" ); 
   rc->SetBlendMode( BLEND_OPAQUE ); 
   rc->SetDepthState( COMPARE_LEQUAL, true ); 
   rc->SetCulling( CULL_BACK ); 

   // material properties
   rc->BindDiffuseTexture( "data/textures/crate.png" ); 
   rc->BindNormalTexture( "data/textures/crate_normal.png" ); 
   rc->BindMaterialBuffer( &dissolve_data, sizeof(dissolve_data) ); 

   rc->DrawMesh( theThingToDraw ); 
}
```

We want this to be easier...

```cpp
// intermittent goal
void Game::Render()
{
   rc->BindShaderState( "shaders/lit_opaque" ); 

   // bind materials
   rc->BindDiffuseTexture( "data/textures/crate.png" ); 
   rc->BindNormalTexture( "data/textures/crate_normal.png" ); 
   rc->BindMaterialBuffer( &dissolve_data, sizeof(dissolve_data) ); 

   rc->DrawMesh( theThingToDraw ); 
}
```

Final goal

```cpp
void Game::Render()
{
   rc->BindMaterial( "material/crate" );  
   rc->DrawMesh( theThingToDraw ); 
}
```


What was `Shader` before is the `ShaderProgram` - or executable
`Shader` is a `ShaderProgram` with the relevant pipeline state


## `ShaderState` is defined through data!

```xml
<!-- Example.ShaderState -->
<ShaderState 
   Shader="Path/To/HLSL.hlsl"       
   BlendMode="Opaque"            
   DepthTest="LessThanOrEqual"
   DepthWrite="true"
   Winding="CounterClockwise"
   Culling="Back"
   FillMode="Solid">

   <!-- What can we configure? -->
</ShaderState>
```

## Code
```cpp

class ShaderState
{
   public:
      Shader( XMLElement const& node );

      void SetupFromXML( XMLElement& node ); 

   public:
      Shader* m_shader; 

      eBlendMode m_blendMode           = BLEND_OPAQUE; 
      eCompareOp m_depthTest           = COMPARE_LEQUAL; 
      bool m_writeDepth                = true; 
      eWindingOrder m_windingOrder     = WIND_COUNTERCLOCKWISE; 
      eCullMode m_culling              = CULL_BACK;
      eFillMode m_fillMode             = FILL_SOLID; 
};

bool Shader::SetupFromXML( XMLElement& node ) 
{
   m_blendMode = node.TryGetAttribute( "BlendMode", m_blendMode ); 
   m_depthTest = node.TryGetAttribute( "DepthTest", m_depthTest ); 
   // ... 
}

void RenderContext::BindShaderState( ShaderState* shaderState )
{
   BindShader( shaderState->m_shader ); 
   SetBlendMode( shaderState->m_blendMode ); 
   SetDepthState( shaderState->m_depthTest, shaderState->m_writeDepth ); 
   // ... so on and so forth
}

// Don't forget to add the database of shaders so they can be shared
void RenderContext::GetOrCreateShaderState( char const* filename )
{
   // ... database of shaders
}

```


## Material

```cpp
class Material
{
   public:
      void SetData( void const* data, size_t dataSize ) 
      {
         m_uboCPUData.resize(dataSize); 
         memcpy( &m_uboCPUData[0], data, dataSize ); 
         m_uboIsDirty = true; 
      }

      template <typename UBO_STRUCT_TYPE>
      void SetData( UBO_STRUCT_TYPE const& data )
      {
         SetData( &data, sizeof(UBO_STRUCT_TYPE) ); 
      }

      template <typename UBO_STRUCT_TYPE>
      UBO_STRUCT_TYPE* GetDataAs() 
      {
         m_uboIsDirty = true; 
         if (m_uboCPUData.size() == sizeof(UBO_STRUCT_TYPE)) {
            return (UBO_STRUCT_TYPE*) &m_uboCPUData[0]; 
         } else {
            m_uboCPUData.resize( sizeof(UBO_STRUCT_TYPE) ); 
            UBO_STRUCT_TYPE* retPtr = (UBO_STRUCT_TYPE*) &m_uboCPUData[0]; 
            new (retPtr) UBO_STRUCT_TYPE(); 

            return retPtr; 
         }
      }

      void SetShaderByName( Shader* shader );

   public:
      ShaderState* m_shaderState = nullptr;

      // renderer specific
      rgba m_tint;
      float m_specularFactor;
      float m_specularPower; 
      // if more, add them

      // general data (and maybe diffuse)
      std::vector<Texture*> m_texturePerSlot; 
      std::vector<Sampler*> m_samplersPerSlot; 

      std::vector<unsigned char> m_uboCPUData; 
      RenderBuffer* m_ubo = nullptr; 
}; 


void RenderContext::BindMaterial( Material* mat )
{
   BindShaderState( mat->m_shaderState ); 

   foreach (texture) {
      BindTexture(texture); 
   }

   foreach (sampler) {
      BindSampler(sampler);
   }

   mat->UpdateUBOIfDirty(); 
   BindMaterialBuffer( mat->m_ubo ); 
}


Game::Startup()
{
   m_dissolveMaterial = new Material( "shader/dissolve" ); 
   m_dissovleMaterial->SetDiffuseTexture( "textures/crate.png" ); 
}

Game::Update()
{
   dissolve_data_t* dissolveData = m_dissolveMaterial->GetDataAs<dissolve_data_t>(); 
   dissolveData->amount = 0.4f + 0.25f * Sin(time_seconds); 
   dissolveData->borderWidth = 0.1f; 
   dissolveData->color = rgba::YELLOW; 
}

Game::Render()
{
   rc->SetModelMatrix( m_cubeModel ); 
   rc->BindMaterial( m_dissolveMaterial ); 
   rc->DrawMesh( m_cubeMesh ); 
}
```
