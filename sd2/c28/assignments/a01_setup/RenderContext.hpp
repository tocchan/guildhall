//------------------------------------------------------------------------
// RenderContext.hpp
//------------------------------------------------------------------------
class RenderContext
{
   public:
      // ...
      // *updated* These now do more work;
      void BeginFrame(); 
      void EndFrame(); 

      void BeginCamera( Camera *cam ); 
      void EndCamera(); 

      void ClearColorTargets( rgba const &color ); 

      // *new* bind a shader to the context; 
      void BindShader( Shader *shader ); 

      // *new* basic draw methods
      // Draws a number of vertices out of the currently bound vertex buffer (todo)
      void Draw( uint vertex_count, uint byte_offset = 0U ); 

   public: // Resources
      Texture2D* GetOrCreateTexture( std::string const &name ); 
      Shader* GetOrCreateShader( std::string const &name ); 

   public:
      //...
      ID3D11Device *m_device;
      ID3D11DeviceContext *m_context; 
      ID3D11SwapChain *m_swapChain; 

      // *new* members
      ColorTargetView *m_currentFrameBackbuffer; 
      Camera *m_currentCamera; 
}; 

//------------------------------------------------------------------------
// RenderContext.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Save out the current backbuffer for this frame; 
void RenderContext::BeginFrame()
{
   // first, acquire the backbuffer (this adds one reference to the backbuffer)
   ID3D11Texture2D *back_buffer = nullptr;
   m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);
  
   // Okay, we have an rtv, store it in our own class
   m_frame_backbuffer_ctv = new ColorTargetView();
   m_frame_backbuffer_ctv->CreateForInternalTexture( back_buffer ); 

   // ctv will hold a reference to the backbuffer, so we
   // can release this functions hold on it; 
   DX_SAFE_RELEASE( back_buffer ); 
}

//------------------------------------------------------------------------
// Swap the backbuffer to the window (so user can see it)
// and free up this frames backbuffer; 
void RenderContext::EndFrame() 
{
   m_swapChain->Present( 0, 0 ); 

   // free this up; 
   // This means we should also release our RTV, otherwise we
   // have a leak; 
   delete m_frame_backbuffer_ctv; 
   m_frame_backbuffer_ctv = nullptr; 

   // TODO: We don't need to new and delete this
   // object each frame, just update the m_rtv (being 
   // sure to release the old one)
}

//------------------------------------------------------------------------
void RenderContext::BeginCamera( Camera *cam )
{
   // this just set ortho before - it will
   // do that again, plus these features; 
   m_currentCamera = cam; // remember the camera; 

   // first, figure out what we're rendering to; 
   ColorTargetView *view = cam->m_color_target; 
   // TODO: If view is nullptr, use the frame's backbuffer; 

   // Bind this as our output (this method takes an array, so 
   // this is binding an array of one)
   m_context->OMSetRenderTargets( 1, &(view->m_rtv), nullptr );

   // Next, we have to describe WHAT part of the texture we're rendering to (called the viewport)
   // This is also usually managed by the camera, but for now, we will just render to the whole texture
   D3D11_VIEWPORT viewport;  
   memset( &viewport, 0, sizeof(viewport) );
   viewport.TopLeftX = 0U;
   viewport.TopLeftY = 0U;
   viewport.Width = view->GetWidth();
   viewport.Height = view->GetHeight();
   viewport.MinDepth = 0.0f;        // must be between 0 and 1 (defualt is 0);
   viewport.MaxDepth = 1.0f;        // must be between 0 and 1 (default is 1)
   m_context->RSSetViewports( 1, &viewport );
}

//------------------------------------------------------------------------
void RenderContext::EndCamera( Camera *cam ) 
{
   // unbind the color targets 
   m_context->OMSetRenderTargets( 0, nullptr, nullptr ); 

   // no current camera being drawn; 
   m_currentCamera = nullptr; 
}

//------------------------------------------------------------------------
void RenderContext::ClearColorTargets( rgba const &color )
{
   // GUARANTEE_OR_DIE possibly
   if (m_currentCamera == nullptr ) {
      return; 
   }

   ColorTargetView *ctv = m_currentCamera->m_colorTargetView; 
   // TODO: if ctv == nullptr, use the backbuffer

   float clear_color[4] = { color.r, color.g, color.b, color.a };
   m_context->ClearRenderTargetView( ctv->m_rtv, clear_color );
}

//------------------------------------------------------------------------
void RenderContext::BindShader( Shader *shader ) 
{
   // Binding a shader for now is just binding
   // these two programmable shaders
   // This method will eventually do a *lot* more
   // as it sets up nearly every part of the pipeline; 
   m_context->VSSetShader( shader->m_vertexStage.m_vs ); 
   m_context->PSSetShader( shader->m_pixelStage.m_fs ); 
}

//------------------------------------------------------------------------
void RenderContext::Draw( uint vertexCount, uint byteOffset /*= 0U*/ ) 
{
   // what is the *shape* of our draw (triangles, lines, etc)
   // for now, always assuming triangles
   // TODO: only call this if different from a previous draw
   m_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); 

   // dispatch the work; (I want to invoke the vertex shader vertexCount 
   // number of times)
   m_context->Draw( vertexCount, byteOffset ); 
}

//------------------------------------------------------------------------
// RenderContext
Shader* RenderContext::GetOrCreateShader( std::string const &filename ) 
{
   // similar to texture UNLESS i need make it; 
   Shader *shader = /*... find in map*/; 

   // oh, doesn't exit, create it and add it; 
   if (nullptr == shader) {
      Shader *shader = new Shader(); 
      shader->CreateFromFile( this, filename ); 

      if (shader->IsValid()) {
         // be sure to delete these on shutdown!
         m_shaderMap.set( filename, shader ); 
      } else {
         delete shader; 
      }
   }

   return shader; 
}