Effect Framework
======

## End Goal
```cpp
void Game::Render()
{
   // I want to render to the backbuffer (the image that
   // will present to the user), but can't
   // use that for image effects...
   // ...so get a texture I can use that matches it.
   Texture* backbuffer = m_renderer->GetFrameBackbuffer(); 
   Texture* output = m_renderer->AcquireRenderTargetMatching( backbuffer ); 

   // Set that as the output - and draw
   m_camera->SetColorTarget( output ); 
   m_renderer->BeginCamera( m_camera ); 
   // render world...
   m_renderer->EndCamera(); 

   // Now, we want to apply an effect to the game given a material
   Texture* temp = m_renderer->AcquireRenderTargetMatching( output ); 
   m_renderer->ApplyEffect( temp, output, m_tonemap_material ); 

   // The final output is now in temp, so copy it back
   // to the backbuffer so we can present
   m_renderer->CopyTexture( backbuffer, temp );

   // Release the two temporary textures back to the pool
   m_renderer->ReleaseRenderTarget( temp ); 
   m_renderer->ReleaseRenderTarget( output ); 

   // Print out how many are alive as a debug step
   // to make sure we're not leaking (for this 
   // demo, we'll have at most 2)
   DebugRenderScreenText( "Pool Size: %u free of %u total", 
      m_renderer->GetFreeRenderTargetCount(), 
      m_renderer->GetTotalRenderTargetCount() ); 

   // Present backbuffer to the user...
   // ...this is happening in `RenderContext::EndFrame()
   // m_renderer->Present(); 
}
```

## Render Context Methods
```cpp
class RenderContext
{
   public:
      // old things we should already have
      Texture* GetFrameBackbuffer(); 
      void Present();

      // NEW things
      // TexturePool Things
      Texture* AcquireRenderTargetMatching( Texture* tex ); // user should release with call below
      void ReleaseRenderTarget( Texture* tex );             // recycles texture to be used again
      Texture* CreateRenderTarget( int width, int height ); // returned object should be deleted

      // Debug Things
      int GetTotalRenderTargetCount(); // how many temporary textures have we made total
      int GetFreeRenderTargetCount(); // how many are currently in the pool

      // Effect Framework
      void ApplyEffect( Texture* dst, Texture* src, Material* mat ); 
      void CopyTexture( Texture* dst, Texture* src ); 

};
```

### Steps
- [ ] Be able to render to a custom render target
    - [ ] Be able to create a render target
    - [ ] Do the render target pool implementation
    - [ ] Verify using `RenderDoc` or equivalent
- [ ] Be able to copy textures -> `CopyTexture`
- [ ] Verify you can now see things again
- [ ] Implement `ApplyEffect`
    - [ ] Some variants if you don't have materials yet


### `Texture` Pooling
...

### `RenderContext::ApplyEffect`
Effects happen OUTSIDE of `BeingCamera` and `EndCamera`, so you may want to confirm you do
not have an active camera.

...

### The Full Screen Triangle


------
## Camera Effect Framework
This is completely optional - and just brings this more in line with
 `Unity` or `Unreal`'s' interface, where you attach effects to a camera
 and it automatically applies

```cpp
void Camera
{
   public:
      void AddEffect( Material* mat, int sortOrder = 0 ); 
      void RemoveEffect( Material* mat ); 
}
```

Idea is during `RenderContext::EndCamera`, we will automatically
apply all effects on the camera by creating two temporary textures
matching the camera's output, and then just walks the `effect stack` 
applying effects, finally copying the final result back to the original
output.  

It is a way to automate the steps demonstrated in `Game::Render` above, but
would suggest only doing so once you have the proper infastructure in place 
instead of only letting it exist on the camera.


## Effect Notes
### The Full Screen Triangle


### Color Transform (Grayscale)
...

### Bloom
...

### ShadowMaps
...