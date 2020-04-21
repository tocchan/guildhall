```cpp
//------------------------------------------------------------------------
enum eTextureCubeSide : uint
{
   TEXCUBE_RIGHT,
   TEXCUBE_LEFT,
   TEXCUBE_TOP,
   TEXCUBE_BOTTOM,
   TEXCUBE_FRONT, // -z
   TEXCUBE_BACK,  // +z

   TEXCUBE_SIDE_COUNT, 
};
```

```cpp
//------------------------------------------------------------------------
bool TextureCube::make_from_images( Image const *src )
{
   destroy();

   #if defined(_DEBUG)  
      // Make sure all textures are the same size.
      uint tx = src[0].get_width();
      for (uint i = 0; i < TEXCUBE_SIDE_COUNT; ++i) {
         ASSERT_RETURN_VALUE( (src[i].get_width() == tx) && (src[i].get_height() == tx), false );
      }
   #endif


   width = src[0].width;
   format = src[0].format;
   usage = BUFFERUSAGE_STATIC;  // and we're copying from textures, so has to be default
   sample_rate = 1;
   bind_flags = BIND_SHADER_RESOURCE; // for now let's just deal with binding a shader resource
   mip_levels = 1;


   // Setup our cubemap desc
   D3D11_TEXTURE2D_DESC cube_desc;
   MemZero( &cube_desc );

   cube_desc.Width = width;
   cube_desc.Height = width;
   cube_desc.MipLevels = mip_levels;
   cube_desc.ArraySize = TEXCUBE_SIDE_COUNT; // 6
   cube_desc.Format = DXFormatFromImageFormat(format);
   cube_desc.Usage = DXUsageFromUsage( usage );
   cube_desc.BindFlags = DXGetBindFlags( bind_flags );
   cube_desc.CPUAccessFlags = 0;
   cube_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; // IMPORTANT - Has to be marked as used as a cubemap

   // multisampling
   cube_desc.SampleDesc.Count = 1;
   cube_desc.SampleDesc.Quality = 0;

   // create the texture array
   // we're creating from 6 existing textures
   D3D11_SUBRESOURCE_DATA data[TEXCUBE_SIDE_COUNT]; // one per side - image
   D3D11_SUBRESOURCE_DATA *pdata = nullptr;
   if (nullptr != src) {
      pdata = data;
      MemZero( &data );
      uint pitch = width * ImageFormatGetStride(format);

      for (uint i = 0; i < TEXCUBE_SIDE_COUNT; ++i) {
         data[i].pSysMem = src[i].pixel_data.get_buffer();
         data[i].SysMemPitch = pitch;
      }
   }

   // create my resource
   if (FAILED(device->dx_device->CreateTexture2D( &cube_desc, pdata, &dx_resource ))) {
      ERROR_MSG( "Failed to create cube texture." );
      return false;
   }

   // create my view
   create_views();
   
   return true;
}


//------------------------------------------------------------------------
bool TextureCube::make_from_image( Image const &src )
{
   destroy();

   // Make sure dimensions match up. 
   uint w = src.get_width() / 4;
   uint h = src.get_height() / 3;
   ASSERT_RETURN_VALUE( (w > 0) && (w == h), false );

   width = w;
   format = src.format;
   usage = BUFFERUSAGE_STATIC;  // and we're copying from textures, so has to be default
   sample_rate = 1;
   bind_flags = BIND_SHADER_RESOURCE; // for now let's just deal with binding a shader resource
   mip_levels = 1;

   // Setup our cubemap desc
   D3D11_TEXTURE2D_DESC cube_desc;
   MemZero( &cube_desc );

   cube_desc.Width = width;
   cube_desc.Height = width;
   cube_desc.MipLevels = mip_levels;
   cube_desc.ArraySize = TEXCUBE_SIDE_COUNT;
   cube_desc.Format = DXFormatFromImageFormat(format);
   cube_desc.Usage = DXUsageFromUsage( usage );
   cube_desc.BindFlags = DXGetBindFlags( bind_flags );
   cube_desc.CPUAccessFlags = 0;
   cube_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

   // multisampling
   cube_desc.SampleDesc.Count = 1;
   cube_desc.SampleDesc.Quality = 0;

   // create the texture array
   // we're creating from 6 existing textures
   D3D11_SUBRESOURCE_DATA data[TEXCUBE_SIDE_COUNT];
   D3D11_SUBRESOURCE_DATA *pdata = nullptr;
   if (src.is_valid()) {
      pdata = data;
      MemZero( &data );
      uint pitch = width * ImageFormatGetStride(format); // width of a single part in bytes
      byte_t const *start = (byte_t const*) src.pixel_data.get_buffer();
      uint total_pitch = 4 * pitch; // total with of entire image in bytes
      uint row = width * total_pitch; // how far to get to the next row

      uint offsets[] = {
         (1 * row) + (2 * pitch),   // right
         (1 * row) + (0 * pitch),   // left
         (0 * row) + (1 * pitch),   // top
         (2 * row) + (1 * pitch),   // bottom
         (1 * row) + (1 * pitch),   // front
         (1 * row) + (3 * pitch)    // back
      };
      
      for (uint i = 0; i < TEXCUBE_SIDE_COUNT; ++i) {
         data[i].pSysMem = start + offsets[i];
         data[i].SysMemPitch = total_pitch;
      }
   }

   // create my resource
   if (FAILED(device->dx_device->CreateTexture2D( &cube_desc, pdata, &dx_resource ))) {
      ERROR_MSG( "Failed to create cube texture." );
      return false;
   }

   create_views();
   return true;
}

//------------------------------------------------------------------------
void TextureCube::create_views()
{
   CD3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
   MemZero( &view_desc );

   view_desc.Format = DXFormatFromImageFormat( format );
   view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
   view_desc.TextureCube.MostDetailedMip = 0;
   view_desc.TextureCube.MipLevels = 1;
   
   device->dx_device->CreateShaderResourceView( dx_resource, &view_desc, &dx_srv );
}
```

------

## How it is used in Shader

```cpp
// resource like a texture
TextureCube<float4> tSkybox : register(t2); 

float4 FragmentFunction() : SV_TARGET0
{
   // test skybox - will show skybox on outside of sphere
   float4 sky_color = tSkybox.Sample( sampDefault, normal ); 
   return sky_color; 

   // sphere of glass
   float3 incident = worldPosition - CAMERA_POSITION; 
   float3 reflection = reflect( incident, normal ); 
   float4 color =  tSkybox.Sample( sampDefault, reflection ); 

   return color; 
}
```