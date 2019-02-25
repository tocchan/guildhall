//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class RenderContext
{
   public:
      // APP CONTROL
      void Startup();                  // A01, A02, A03, A04
      void Shutdown();                 // A02, A02, A03

      // ...
      DepthStencilTargetView* GetFrameDepthStencilTarget(); // A04

      // Be sure to set the depth view and model buffer; 
      void BeginCamera( Camera* );     // A01, A02, A03, A04
      void EndCamera();                // A01

      void ClearColorTargets( rgba const &color );                              // A01
      void ClearDepthStencilTarget( float depth = 1.0f, uint8_t stencil = 0U ); // A04

      //...

      // Be able to bind index buffers; 
      void BindIndexStream( IndexBuffer *ibo );    // A04

      // Resource Binding
      // ...

      // Be able to set a model matrix (updates the uniform buffer; 
      void SetModelMatrix( mat44 const &modelMatrix );            // A04

      // Drawing
      // Update DrawVertexArrays to use DrawMesh
      // Implement DrawMesh; 
      void Draw( uint vertexCount, uint byteOffset = 0U );                 // A01
      void DrawIndexed( uint indexCount );                                 // A04

      void DrawVertexArrays( VertexPCU const *vertices, uint count );      // A02
      void DrawMesh( Mesh *mesh );                                         // A04

      // Any Additional Helper Drawing Methods
      // ... 

   public:
      Shader* GetOrCreateShader( std::string const &filename );                // A01
      TextureView2D* GetOrCreateTextureView2D( std::string const &filename );  // A03

   public:
      // ...

      // new members
      DepthStencilTargetView *m_defaultDepthStencilView; // A04
      UniformBuffer *m_modelBuffer;                      // A04

      // m_immediateMesh replaces m_immediateVBO
      // VertexBuffer *m_immediateVBO;                   // A02 
      GPUMesh *m_immediateMesh;              

      // ...
}; 


//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void RenderContext::Startup()
{
   // ...

   model_buffer_t buffer; 
   buffer.model = mat44::IDENTITY; 

   m_modelBuffer = new UniformBuffer( this ); 
   m_modelBuffer->CopyCPUToGPU( &buffer, sizeof(buffer) ); 
}

//------------------------------------------------------------------------
void RenderContext::BeginCamera( Camera *cam )
{
   // ...

   // bind targets
   ColorTargetView *view = cam->m_color_target; 
   ID3D11RenderTargetView *dx_rtv = nullptr; 
   uint colorCount = 0U; 
   if (view != nullptr) {
      dx_srv = view->GetHandle(); 
      colorCount = 1U; 
   }
   
   // if we have a depth target, bind that as well; 
   DepthStencilTargetView *dsv = cam->GetDepthStencilTargetView(); 
   ID3D11DepthStencilView *dx_dsv = nullptr; 
   if (dsv != nullptr) {
      dx_dsv = dsv->GetHandle(); 
   }

   // Bind this as our output (this method takes an array, so 
   // this is binding an array of one)
   m_context->OMSetRenderTargets( colorCount, &dx_rtv, dx_dsv );

   // ... other stuff?

   // Bind model matrix; 
   BindModelMatrix( mat44::IDENTITY ); 
   BindUniformBuffer( 3, m_modelBuffer ); 
}

//------------------------------------------------------------------------
void RenderContext::ClearDepthStencilTarget( float depth = 1.0f, uint8_t stencil = 0U )
{
   ID3D11DepthStencilView *dsv = nullptr; 
   dsv = m_currentCamera->GetDepthStencilTargetView()->GetHandle(); 
   m_context->ClearDepthStencilView( dx_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil ); 
}


//------------------------------------------------------------------------
void RenderContext::BindModelMatrix( mat44 const &model ) 
{
   model_buffer_t buffer; 
   buffer.model = model; 

   m_modelBuffer->CopyCPUToBPU( &buffer, sizeof(buffer) ); 
}

//------------------------------------------------------------------------
void RenderContext::BindIndexStream( IndexBuffer *ibo ) 
{
   ID3D11Buffer *handle = nullptr; 
   if (ibo != nullptr) {
      handle = ibo->GetHandle(); 
   }

   m_context->IASetIndexBuffer( handle, 
      DXGI_FORMAT_R32_UINT,      // 32-bit indices;            
      0 );  // byte offset 
}


//------------------------------------------------------------------------
void RenderContext::DrawVertexArrays( VertexPCU const *vertices, uint count ) 
{
   m_immediateMesh->CopyFromVertexArray( vertices, count ); 
   m_immediateMesh->SetDrawCall( false, count ); 

   DrawMesh( m_immediateMesh ); 
}

//------------------------------------------------------------------------
void RenderContext::DrawMesh( Mesh *mesh )
{
   BindVertexStream( mesh->m_vertexBuffer ); 
   BindIndexStream( mesh->m_indexBuffer ); 

   if (mesh->UsesIndexBuffer()) {
      DrawIndex( mesh->GetElementCount() ); 
   } else {
      Draw( mesh->GetElementCount() ); 
   }
}

//------------------------------------------------------------------------
void RenderContext::DrawIndexed( uint elemCount ) 
{
   // does other code normal Draw does;
   // ...

   // Draw
   m_context->DrawIndexed( elemCount, 
      0,       // elem offset 
      0 );     // vert offset 
}