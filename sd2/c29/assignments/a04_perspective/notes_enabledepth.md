## THINGS
- Coloumb's Friction - slight bug.  You clamp its magnitude by jn * friction, and that's about it.  More involved if you want Static and Dynamic Frictions.  See link in assignment 

- Don't use verlet in impact velocity.  Bugs it is causing are worse than the bugs it solves.  Keep verlet around though - it is useful for gameplay purposes we'll be doing later; 

- Managing `RenderContext` State
  - Raster State
    - `SetCullMode`
    - `SetFillMode`
  - Depth Stencil State
    - `EnableDepth( eCompareOp op, bool write )`
    - `DisableDepth()`

 ```cpp

class RenderContext
{
   public:
      // depth state
      void EnableDepth( eCompareOp op, bool write ); 
      void DisableDepth();

      // A05 - raster state
      void SetCullMode( eCullMode mode ); // ID3D11RasterState
      void SetFillMode( eFillMode mode ); 

      // Optional 
      // void SetBlendState( op, src, dst, op, src, dst ); 
};


void Game::Render()
{
   rc->BeginCamera( m_gameCamera ); 

   // drawing opaque level geometer
   rc->EnableDepth( COMPARE_LESS_EQUAL, true );
   DrawLevel();

   // sprite rendering mode
   rc->EnableDepth( COMPARE_LESS_EQUAL, false ); 
   DrawSprites(); 

   rc->DisableDepth(); // ALWAYS, false
   RenderPlayerGlyphs(); 

   rc->EndCamera(); 


   rc->BeginCamera( m_uiCamera ); 
   // draw my ui
   rc->EndCamera(); 
}
```

```cpp
void RenderContext::EnableDepth( eCompareOp op, bool write )
{
   ASSERT( m_currentCamera != nullptr, "Only call between BeginCamera and EndCamera" ); 
   // ID3D11DepthStencilState* m_depthStencilState = nullptr; 
   if (m_depthStencilState != nullptr) {
      D3D_SAFE_RELEASE(m_depthStencilState); 
   }

   ID3D11_DEPTH_STENCIL_DESC desc; 
   memset( &desc, 0, sizeof(desc) ); // set everything to 0

   desc.DepthEnable = TRUE;
   desc.DepthWriteMask = write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_NONE; 
   desc.DepthFunc = ToD3DType(op); // you implement ToD3DType to convert to D3D11_COMPARISON_FUNC 

   m_device->CreateDepthStencilState( &desc, &m_depthStencilState ); 
   m_context->OMSetDepthStencilState( m_depthStencilState, 0 ); 
}

void RenderConext::EndCamera()
{
   DX_SAFE_RELEASE( m_depthStencilState ); 

   // ...

}

