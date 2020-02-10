- Creating a Texture from an Image
- UV considerations (v is flipped from GL)
  - fix up during construction
    - `vec2 FixUV(vec2 uv)` as part of `RenderCommon.hpp` that can be API specific is a nice approach here.
  - fix in shader
  - flip the texture on load (do not recommend, though Unity goes this route)
- Creating a Sampler
  - What does a sampler do
  - What are the options
  - When will you have to revisit this code?
- Using this in a shader

## Creating a Texture

```cpp
// assume dimensions are `IntVec2 size`;

// cpuAccess is used for STAGING buffers (screenshots) or dynamic buffers (height maps/noise?).  For now, we'll just assume none; 
uint cpuAccess = 0; 

// Setup the Texture Description (what the resource will be like on the GPU)
D3D11_TEXTURE2D_DESC texDesc2D;
MemZero( &texDesc2D );

texDesc2D.Width = size.x;
texDesc2D.Height = size.y;
texDesc2D.MipLevels = 1; // setting to 0 means there's a full chain (or can generate a full chain)
texDesc2D.ArraySize = 1; // only one texture
texDesc2D.Usage = DXUsageFromMemoryUsage(m_mem_usage); // loaded from image - probably not changing
texDesc2D.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // for now assume 4 channel RGBA
texDesc2D.BindFlags = DXTextureBindFromUsage(m_usage); // only allowing rendertarget for mipmap generation
texDesc2D.CPUAccessFlags = cpuAccess; // Determines how I can access this resource CPU side 
texDesc2D.MiscFlags = 0;  

// If Multisampling - set this up.
texDesc2D.SampleDesc.Count = 1;
texDesc2D.SampleDesc.Quality = 0;

// Actually create it
HRESULT hr = dd->CreateTexture2D( &texDesc2D,
  nullptr, 
  &m_handle_2d );

// If succeeded - copy our image to it
if (SUCEEDED(hr)) {

}
```

```cpp
static UINT DXTextureBindFromUsage( eTextureUsage usage ) 
{
	UINT ret = 0; 
   if (bind_flags & (TEXTURE_SHADER_RESOURCE_BIT) {
      ret |= D3D11_BIND_SHADER_RESOURCE;
   }

   if (bind_flags & TEXTURE_RENDER_TARGET_BIT) {
      ret |= D3D11_BIND_RENDER_TARGET;
   }
   
   if (bind_flags & TEXTURE_DEPTH_STENCIL_BIT) {
      ret |= D3D11_BIND_DEPTH_STENCIL;
   }

   return ret;
}
```