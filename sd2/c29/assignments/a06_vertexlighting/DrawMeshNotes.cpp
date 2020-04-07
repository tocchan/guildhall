
//------------------------------------------------------------------------
struct VertexPCT
{
   static buffer_attribute_t const LAYOUT[]; 
}


//------------------------------------------------------------------------
class VertexBuffer
{
   public:
      ID3D11Buffer* m_handle; 

      buffer_attribute_t const* m_layout; 
      size_t m_element_stride; 
}; 

//------------------------------------------------------------------------
void RenderContext::SetVertexBuffer( VertexBuffer* vbo ) 
{
   buffer_attribute_t const* layout = vbo->GetLayout(); 
   if (m_last_bound_layout != layout) {
      layout has changed(); 
   }

   m_last_bound_vbo = vbo; 
   uint stride = (uint) vbo->GetStride(); 

   // bind vbo 
   // ...

}

//------------------------------------------------------------------------
// similar are MRU - most recently cache of size 1
ID3D11InputLayout* Shader::GetOrCreateInputLayout( buffer_attribute_t const* layout ) 
{
   if (m_last_used_layout != layout) {
      free old d3d11 layout; 

      create new d3d11 layout

      m_last_used_layout = layout; 
   }

   return m_d3d11_input_layout; 
}

//------------------------------------------------------------------------
void RenderContext::FinalizeState()
{
   if ((shader has changed) || (vertex layout changed)) {
      ID3D11InputLayout* layout = shader->GetOrCreateInputLayout( m_last_bound_vbo->GetLayout() );
      m_context->IASetInputLayout( layout );  
   }
}

//------------------------------------------------------------------------
void RenderContext::Draw( uint start, uint count )
{
   FinalizeState(); 
   m_context->Draw( start, count ); 
}

//------------------------------------------------------------------------
void RenderContext::DrawIndexed( uint start, uint count, uint vertex_offset ) 
{
   FinalizeState(); 
   m_context->DrawIndexed( start, count, vertex_offset ); 
}

//------------------------------------------------------------------------
void RenderContext::DrawMesh( GPUMesh* mesh ) 
{
   // bind a bad stride
   // 24 byte stride
   // 36 byte
   BindVertexBuffer( mesh->GetVertexBuffer() ); 

   if (mesh->GetUsesIndices()) {
      Draw( 0, vertexCount ); 
   } else {
      BindIndexBuffer( mesh->GetIndexBuffer() ); 
      DrawIndexed( 0, indexCount ); 
   }
}

//------------------------------------------------------------------------
// This is a "identity" TBN, or "tangent - bitangent - normal" space.
// tangent -> (1, 0, 0)
// bitangent -> (0, 1, 0)
// normal -> (0, 0, 1)


// normal to color
vec3 color = (normal + vec3(1.0f)) * 0.5f; 

(0, 0, 1) -> (.5, .5, 1)












//------------------------------------------------------------------------
void RenderContext::FinalizeState()
{
   if ((shader has changed) || (vertex layout changed)) {
      ID3D11InputLayout* layout = shader->GetOrCreateInputLayout( m_last_bound_vbo->GetLayout() );
      m_context->IASetInputLayout( layout );  
   }

   if (light info has changed) {
      m_light_ubo->UpdateFromCPU( &m_light_data, sizeof(m_light_data) ); 
      mark light info as clean; 
   }
}

//------------------------------------------------------------------------
class RenderContext 
{
   public:
      void set_ambient( rgba color, float intensity ); 
      void set_ambient_intensity( float intensity ); 

   private: 
      void set_dirty( eRenderContextDirtyBit bit );
      bool is_dirty( eRenderContextDirtyBit bit ) const

   public:
      RenderBuffer* m_light_ubo;  
      light_data_t m_light_data;    // cpu version of my lights

      // dirty flags get set at begin camera - everything is dirty after begin camera
      bool m_light_data_dirty = true; 
};

//------------------------------------------------------------------------
void RenderContext::set_ambient_intensity( float intensity )
{
   m_light_data.ambient_intensity = intensity; 
   mark light as dirty(); 
}
