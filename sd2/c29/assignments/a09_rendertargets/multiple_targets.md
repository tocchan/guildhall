Multiple Render Targets
======

Once you can create arbitrary render targets, you can also output
to multiple at a time; 

This will touch three areas

- [ ] The shader who wants to output multiple data (lighting in bloom's case)
- [ ] The `Camera` class so you can set multiple targets.
- [ ] `RenderContext::BeginCamera` to actually do the work of binding them.


## Shader Output
Before, we were outputting one channel, and this was specified by saying...

```cpp
float4 FragmentFunction( v2f_t input ) : SV_Target0
{

}
```

This is saying that Fragment returns one output, a `float4`, which should output to our first target `SV_Target0`.

Now, we can output more data, by specifying more targets.

```cpp
struct fragment_output_t 
{
   float4 color   : SV_Target0; 
   float4 bloom   : SV_Target1; 
};

fragment_output_t FragmentFunction( v2f_t input )
{
   float4 finalColor;
   float4 bloom; 
   /* compute finalColor and bloom */
   // ...

   // output
   fragment_output_t output;
   output.color = finalColor; 
   output.bloom; 
}
```

This is now outputting two pieces of information, one of which we'll call color and one we'll call bloom.

The name of the struct, or members don't matter - what matters is the semantic, ie `SV_Target0` or `SV_Target1`.  When returned from a fragment function these have special meaning instructing the GPU where each member should be output. 


## Updating the Camera
Now that we can output to multiple targets, we have to bind multiple targets to the GPU for it to write to.

Our camera handles that by now supporting

```cpp
void Camera::SetColorTarget( int idx, Texture* colorTarget ); 
```

Before you only supported one color target, but now you can support an array of them.  You should still have
`SetColorTarget( Texture* target )` exist, but it should just call up to `SetColorTarget(0, target)`.

Internally, this is just storing an array of textures instead of just one, so should be a fairly straight forward change.  

If you set a color target to `nullptr` in the array, use `nullptr`.  You may also want to trim the array of tail elements that are null if you have a camera that is switching color target counts often. 

I purposefully do not like having `nullptr` be treated as the default backbuffer, but that code is in to 
provide backwards support to SD1 projects.  If you want to maintain that functionality, be sure this default
behaviour only applies to target index 0. 


## Updating `BeginCamera`
Now that we have multiple color targets, we have to be sure to bind them.

Our call before was `OMSetRenderTargets( 1, &rtv, dsv )`.  The `1, &rtv` is saying we have one target, and then we're passing an array of 1 sized with our `rtv` in it.  

Now that we have more targets, we can bind all of them.  The one stipulation is all targets must be bound all at once.

So in `BeginCamera` we'll be doing something like this; 

```cpp
   // TEMPORARY - this will be moved
   //m_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
   std::vector<ID3D11RenderTargetView*> rtvs; 

   int rtvCount = camera.GetColorTargetCount(); 
   rtvs.resize( rtvCount ); 
   
   // Get all my render target views
   for (int i = 0; i < rtvCount; ++i) 
   {
      rtvs[i] = nullptr; 

      Texture* colorTarget = camera.GetColorTarget(i); 
      if (colorTarget != nullptr) 
      {
         TextureView* rtv = colorTarget->GetRenderTargetView(); 
         rtvs[i] = rtv->GetAsRTV(); 
      }
   }

   // Clear them all (could do individual targets)
   if( camera.m_clearMode & CLEAR_COLOR_BIT )
   {
      //ClearScreen( camera.m_clearColor );
      Rgba8 clearColor = camera.m_clearColor;
      float clearFloats[4];
      clearFloats[0] = clearColor.r / 255.f;
      clearFloats[1] = clearColor.g / 255.f;
      clearFloats[2] = clearColor.b / 255.f;
      clearFloats[3] = clearColor.a / 255.f;

      for (int i = 0; i < rtvCount; ++i) 
      {
         if (rtvs[i] != nullptr) 
         {
            m_context->ClearRenderTargetView( rtvs[i], clearFloats ); 
         }
      }
   }

   ID3D11DepthStencilView* dsv = nullptr;
   TODO( "Get dsv if this camera uses depth!" ); 

   m_context->RSSetViewports( 1, &viewport );
   m_context->OMSetRenderTargets( rtvCount, rtvs.data(), dsv );
```

### Clearing
Now that we have multiple targets, be sure to clear each one.

For a more feature rich system, allow each target to have it's own clear options, since different targets may want to clear to different things (color may clear to fog color, but bloom will want to clear to black). 

For this assignment though, it should be fine to clear all to the same color as above - just
be sure to use a clear color of black for the bloom effect. 