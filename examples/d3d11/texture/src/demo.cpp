//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// New Content will be surrounded by this
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "demo.h"
#include "log.h"

#include "window.h"

// D3D Stuff
// Required Headers
#include <d3d11.h>  
#include <DXGI.h>    

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "DXGI.lib" )

// DEBUG STUFF
#include <dxgidebug.h>
// #pragma comment( lib, "dxguid.lib" )

// NEEDED FOR COMPILING
// Note:  This is not allowed for Windows Store Apps.
// Shaders must be compiled to byte-code offline. 
// but for development - great if the program knows how to compile it.
#include <d3dcompiler.h>
#pragma comment( lib, "d3dcompiler.lib" )

//------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------
#define WINDOW_TITLE    "D3D11 TEXTURES & SAMPLERS"
#define WINDOW_RES_X    (1280)
#define WINDOW_RES_Y    (720)

#define RENDER_DEBUG
#if defined(RENDER_DEBUG)
   #define DEBUG_SHADERS
#endif

typedef unsigned int uint;

//------------------------------------------------------------------------
// MACROS
//------------------------------------------------------------------------
#define SAFE_DELETE(ptr)   if ((ptr) != nullptr) { delete ptr; ptr = nullptr }
#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

//------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct vec2
{
   float x;
   float y;

   vec2() 
      : x(0.0f)
      , y(0.0f) 
   {}

   vec2( float _x, float _y ) 
      : x(_x)
      , y(_y) 
   {}
};

struct vec3
{
   float x; 
   float y;
   float z;

   vec3() 
      : x(0.0f)
      , y(0.0f)
      , z(0.0f)
   {}
   vec3( float _x, float _y, float _z ) 
      : x(_x)
      , y(_y)
      , z(_z) 
   {}
};

struct vertex_t 
{
   vec3 position;
   
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Adding UV coordinates per vertex.
   vec2 uv;

   vertex_t() {}
   vertex_t( vec3 const &p, vec2 const &t ) 
      : position(p)
      , uv(t)
   {}
};

//------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------
static HWND gHWND = NULL;

ID3D11Device *gD3DDevice = nullptr;
ID3D11DeviceContext *gD3DContext = nullptr;
IDXGISwapChain *gD3DSwapChain = nullptr;

// Multiple functions now use this - so need to store it for the frame
// [also, need to clean it up at the end of the app] 
ID3D11RenderTargetView *gD3DRenderTargetView = nullptr;
ID3D11VertexShader *gD3DVertexShader = nullptr;
ID3D11PixelShader *gD3DFragmentShader = nullptr;
ID3D11InputLayout *gD3DInputLayout = nullptr;

ID3D11Buffer *gD3DVertexBuffer = nullptr;
ID3D11Buffer *gD3DIndexBuffer = nullptr;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ID3D11Texture2D *gD3DTexture = nullptr;                              // Actual texture information (texels)
ID3D11ShaderResourceView *gD3DTextureShaderResourceView = nullptr;   // A view of that texture - describe to the shdaer how it should be read
ID3D11SamplerState *gD3DPointSampler = nullptr;                      // A sampler - or a way in which to get information from a texture


//------------------------------------------------------------------------
// Declare functions this demo uses
//------------------------------------------------------------------------

void CreateBackBufferView();

void* FileReadToBuffer( char const *filename, size_t *out_size );
ID3DBlob* CompileHLSLToShaderBlob( char const *opt_filename,         // optional: used for error messages
   void const *source_code,                                          // buffer containing source code.
   size_t const source_code_size,                                    // size of the above buffer.
   char const *entrypoint,                                           // Name of the Function we treat as the entry point for this stage
   char const* target );                                             // What stage we're compiling for (Vertex/Fragment/Hull/Compute... etc...)

void CreateInputLayout( ID3DBlob *vs_bytecode );
bool CreateVertexAndIndexBuffer();

bool CreateTexture();
bool CreateSampler();
bool CreateShaderResourceView( ID3D11Texture2D *texture );


void DemoRender();            // Does rendering for this demo
void DemoRun();               // Demo setup and update loop

//------------------------------------------------------------------------
// Code not pertinant to the topic of this demo is moved here
//
// I will be swapping things in and out as I add/remove steps.
//------------------------------------------------------------------------
#include "demo.inl"


//------------------------------------------------------------------------
// D3D11 STUFF
//------------------------------------------------------------------------



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Need to add UV here
void CreateInputLayout( ID3DBlob *vs_bytecode ) 
{
   // Finally, we need to create an input layout - so that we know how to get our vertices
   // to the shader - this requires still having the bytecode for your fragment shader around.
   D3D11_INPUT_ELEMENT_DESC input_description[2];
   memset( input_description, 0, sizeof(input_description) );

   // Map Position
   input_description[0].SemanticName = "POSITION";             // name we gave this input
   input_description[0].SemanticIndex = 0;                     // Semantics that share a name (or are large) are spread over multiple indices (matrix4x4s are four floats for instance)
   input_description[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Type this data is (float3/vec3 - so 3 floats)
   input_description[0].InputSlot = 0U;                        // Input Pipe this comes from
   input_description[0].AlignedByteOffset = offsetof( vertex_t, position );   // Offset into this pipe to get to the data we want (this resolves to 0)
   input_description[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;   // What is this data for (per vertex data is the only one we'll be dealing with in this class)
   input_description[0].InstanceDataStepRate = 0U;             // If this were instance data - how often do we step it (0 for vertex data)

   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Map UV
   input_description[1].SemanticName = "UV";                   // name we gave this input
   input_description[1].SemanticIndex = 0;                     // Semantics that share a name (or are large) are spread over multiple indices (matrix4x4s are four floats for instance)
   input_description[1].Format = DXGI_FORMAT_R32G32_FLOAT;     // Type this data is (float2/vec2 - so 2 floats)
   input_description[1].InputSlot = 0U;                        // Input Pipe this comes from
   input_description[1].AlignedByteOffset = offsetof( vertex_t, uv );   // Offset into this pipe to get to the data we want (this resolves to 0)
   input_description[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;   // What is this data for (per vertex data is the only one we'll be dealing with in this class)
   input_description[1].InstanceDataStepRate = 0U;             // If this were instance data - how often do we step it (0 for vertex data)


   gD3DDevice->CreateInputLayout( input_description, 
      ARRAYSIZE(input_description),
      vs_bytecode->GetBufferPointer(), 
      vs_bytecode->GetBufferSize(), 
      &gD3DInputLayout );   
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This changes to make a quad (two triangles)
// and adds UVs.  Also introduces the concepts of 
// Index Buffers to cut down on duplicate points.
// (and to make sure this example wasn't such a small change)
bool CreateVertexAndIndexBuffer()
{
   // Make a quad
   //
   // [1]---[3]
   //  |  /  |
   // [0]---[2]
   vertex_t vertices[] = {
      vertex_t( vec3( -.5f, -.5f, 0.0f ), vec2( 0.0f, 1.0f ) ), 
      vertex_t( vec3( -.5f, 0.5f, 0.0f ), vec2( 0.0f, 0.0f ) ), 
      vertex_t( vec3( 0.5f, -.5f, 0.0f ), vec2( 1.0f, 1.0f ) ),
      vertex_t( vec3( 0.5f, 0.5f, 0.0f ), vec2( 1.0f, 0.0f ) ), 
   };
   uint vertex_count = ARRAYSIZE(vertices);  

   // First, describe the buffer
   D3D11_BUFFER_DESC vb_desc;
   memset( &vb_desc, 0, sizeof(vb_desc) );

   vb_desc.ByteWidth = sizeof(vertex_t) * vertex_count;  // How much data are we putting into this buffer
   vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;         // What can we bind this data as (in this case, only vertex data)
   vb_desc.Usage = D3D11_USAGE_IMMUTABLE;                // Hint on how this memory is used (in this case, it is immutable, or constant - can't be changed)
                                                         // for limitations/strenghts of each, see;  
                                                         //    https://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx
   vb_desc.StructureByteStride = sizeof(vertex_t);       // How large is each element in this buffer

   // Next, setup the initial data (required since this is an immutable buffer - so it must be instantiated at creation time)
   D3D11_SUBRESOURCE_DATA initial_data;
   memset( &initial_data, 0, sizeof(initial_data) );
   initial_data.pSysMem = vertices;

   // Finally create the vertex buffer
   HRESULT hr = gD3DDevice->CreateBuffer( &vb_desc, &initial_data, &gD3DVertexBuffer );
   if (FAILED(hr)) {
      return false;
   }

   
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Now make the Index Buffer

   // Make two triangles using the top two points
   uint indices[] = { 0, 1, 3, 0, 3, 2 };

   // Describe the D3D Buffer
   D3D11_BUFFER_DESC ib_desc;
   memset( &ib_desc, 0, sizeof(ib_desc) );

   ib_desc.ByteWidth = sizeof(indices);
   ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;    // different from above - now it is used as an Index Buffer
   ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
   ib_desc.StructureByteStride = sizeof(uint);

   // reusing initial_data from above for brevity
   initial_data.pSysMem = indices;

   // Finally, create the index buffer
   hr = gD3DDevice->CreateBuffer( &ib_desc, &initial_data, &gD3DIndexBuffer );
   return SUCCEEDED(hr);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool CreateTexture()
{
   // I'm going to be creating a texture in the CPU for this, since image loading code is non-trivial.
   // I recommend using stb_imge (https://github.com/nothings/stb) for image loading as it is free 
   // and light weight.  libpng, and libjpeg are also good options if you want something more verbose.

   // Texture I am created is an RGBA8 (four channels, one byte per channel - red should be in the highest order 
   // bits.
   // I'm just going to create a checkerboard (1024x1024, which each square being 32x32

   uint *colors = (uint*) ::malloc( sizeof(uint) * 1024 * 1024 );
   for (uint i = 0; i < 1024 * 1024; ++i) {
      uint x = i % 1024;
      uint y = i / 1024;

      uint cx = x / 32;
      uint cy = y / 32;

      if (((cx + cy) % 2) == 0) {
         // black
         colors[i] = 0xff000000;
      } else {
         colors[i] = 0xffffffff;
      }
   }

   // Now, create a texture from this
   D3D11_TEXTURE2D_DESC tex_desc;
   memset( &tex_desc, 0, sizeof(tex_desc) );

   tex_desc.Width = 1024;     // width... 
   tex_desc.Height = 1024;    // ...and height of image in pixels.
   tex_desc.MipLevels = 1;    // setting to 0 means there's a full chain (or can generate a full chain) - we're immutable, so not allowed
   tex_desc.ArraySize = 1;    // only one texture (
   tex_desc.Usage = D3D11_USAGE_IMMUTABLE;            // data is set at creation time and won't change
   tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // R8G8B8A8 texture
   tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;   // we're going to be using this texture as a shader resource
   tex_desc.CPUAccessFlags = 0U;                      // Determines how I can access this resource CPU side (IMMUTABLE, So none)
   tex_desc.MiscFlags = 0;                            // Extra Flags, of note is;
                                                      // D3D11_RESOURCE_MISC_GENERATE_MIPS - if we want to use this to be able to generate mips (not compatible with IMMUTABLE)
   
   // If Multisampling - set this up - we're not multisampling, so 1 and 0
   // (MSAA as far as I know only makes sense for Render Targets, not shader resource textures)
   tex_desc.SampleDesc.Count = 1;
   tex_desc.SampleDesc.Quality = 0;

   // Setup Initial Data
   D3D11_SUBRESOURCE_DATA data;
   memset( &data, 0, sizeof(data) );

   data.pSysMem = colors;
   data.SysMemPitch = 1024 * sizeof(uint); // pitch is byte size of a single row)

   tex_desc.Usage = D3D11_USAGE_IMMUTABLE;
   HRESULT hr = gD3DDevice->CreateTexture2D( &tex_desc, &data, &gD3DTexture );

   // free CPU memory, no longer needed
   free(colors);

   return SUCCEEDED(hr);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Determines how we "sample" from the texture.
// Since a texture very rarely 1-1 matches texel to pixel, these 
// are the rules for when we need to get the colour as a specific
// fragment location, how does it combine texels to get it.
bool CreateSampler()
{
   D3D11_SAMPLER_DESC desc;
   memset( &desc, 0, sizeof(desc) );

   desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // POINT means pick the nearest - gives a pixelated look.
   desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;   // Set it to wrap on overflow (so sampling at 1.1 is the same as sampling at .1)
   desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
   desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
   // desc.BorderColor // used to set the border color for border clamp mode

   // Mip Levels (default - no mip)
   desc.MinLOD = 0;
   desc.MaxLOD = -1;

   HRESULT hr = gD3DDevice->CreateSamplerState( &desc, &gD3DPointSampler );
   return SUCCEEDED(hr);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
bool CreateShaderResourceView( ID3D11Texture2D *texture )
{
   // We create a view and use the resource to figure out what it should be
   // for most cases this is enough (and I usually store the view with the texture)
   // but in some cases (Depth Texture), we create a view different than what is stored
   // in the texture.
   HRESULT hr = gD3DDevice->CreateShaderResourceView( texture, nullptr, &gD3DTextureShaderResourceView );
   return SUCCEEDED(hr);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Moving this to it's own function for cleanliness
void SetViewport( uint x, uint y, uint width, uint height ) 
{
   // Also, set which region of the screen we're rendering to, in this case, all of it 
   D3D11_VIEWPORT viewport;
   memset( &viewport, 0, sizeof(viewport) );
   viewport.TopLeftX = (FLOAT)x;
   viewport.TopLeftY = (FLOAT)y;
   viewport.Width = (FLOAT)width;
   viewport.Height = (FLOAT)height;
   viewport.MinDepth = 0.0f;        // must be between 0 and 1 (defualt is 0);
   viewport.MaxDepth = 1.0f;        // must be between 0 and 1 (default is 1)
   gD3DContext->RSSetViewports( 1, &viewport );
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void DemoRender()
{
   ClearScreen();

   // Now, we don't actually render to anything by default - so before we can draw anything
   // we must tell D3D11 where we are rendering to.  In this case - the back buffer.
   gD3DContext->OMSetRenderTargets( 1, &gD3DRenderTargetView, nullptr );

   SetViewport( 0, 0, WINDOW_RES_X, WINDOW_RES_Y );

   // Next, we want to draw our vertex buffer - so to do that, we need to 
   // tell D3D11 a few things (in any order before the draw call)

   // 1. What are we drawing - so input data.  In this case, we're going to 
   // be drawing some vertices stored in a Vertex Buffer Object (VBO)
   uint stride = sizeof(vertex_t);
   uint offset = 0U;
   gD3DContext->IASetVertexBuffers( 0,    // Start slot index
      1,                      // Number of buffers we're binding
      &gD3DVertexBuffer,      // Array of buffers
      &stride,                // Stride (read: vertex size, or amount we move forward each vertex) for each buffer
      &offset );             // Offset into each buffer (array - we are only passing one. 

   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // 1b. Set the index buffer (if we're drawing indexed, which we are now
   // It is a 32-bit index, so that is the format we use.
   gD3DContext->IASetIndexBuffer( gD3DIndexBuffer, DXGI_FORMAT_R32_UINT, 0U );

   // 2. How are we drawing - the Shader Programs we compiled earliar.  For this class
   //    we only worry about the Vertex and Pixel Shader - but all stages in the Graphics
   //    pipeline our program uses should be set.
   //    <note: I use OpenGL naming, and refer to Pixel Shaders are Fragment Shaders>
   gD3DContext->VSSetShader( gD3DVertexShader, nullptr, 0U );
   gD3DContext->PSSetShader( gD3DFragmentShader, nullptr, 0U );

   // 2b.  Set the input layout - this tells D3D how my vertex data relates (or should be fed into)
   //      this program.  This is actually a combination of the mesh and vertex shader - but in this class
   //      we are sticking to a single vertex format - so I will be storing it with the Shader
   //      for simplicity.
   gD3DContext->IASetInputLayout( gD3DInputLayout );

   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // 3.   Any textures or constant buffers we need.  This demo has
   //      a sampler and texture, so bind those now.

   // Now, I'm setting these to the PIXEL (FRAGMENT) stage.  Since in my HLSL
   // that is the stage that uses them.  If the vertex shader needed
   // textures, we would need to bind the VS stage.
   gD3DContext->PSSetShaderResources( 0, 1, &gD3DTextureShaderResourceView );
   gD3DContext->PSSetSamplers( 0, 1, &gD3DPointSampler );


   // 4.   Topology Type - the data in the Vertex Buffer/Index Buffer describes what kind of shapes.  
   //      In thish class - I only deal with POINTS, LINES, and TRIANGLES.
   gD3DContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Now that everything is bound, we can draw!
   // I'm drawing two triangesl now - so 6 vertices total.
   gD3DContext->DrawIndexed( 6, 0, 0 );


   // We're done rendering, so tell the swap chain they can copy the back buffer to the front (desktop/window) buffer
   gD3DSwapChain->Present( 0, // Sync Interval, set to 1 for VSync (note, the OS may override this anyway)
      0 );                    // Present flags, see;
                              // https://msdn.microsoft.com/en-us/library/windows/desktop/bb509554(v=vs.85).aspx
}

//------------------------------------------------------------------------
// DEMO ENTRY POINT
//------------------------------------------------------------------------
void DemoRun()
{
   // Need a Window so I have something to render to.
   WindowCreate();

   // Setup D3D11 - will crash if this fails (demo contains no error checking)
   D3D11Setup(gHWND);
   
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   CreateBackBufferView(); // can be part of setup
   CreateShaders( "hlsl/nop_textured.hlsl" );
   CreateVertexAndIndexBuffer();
   CreateTexture();
   CreateSampler();
   CreateShaderResourceView(gD3DTexture);

   // While this window is open, process messages, and do other game processing
   while (WindowIsOpen()) {
      WindowStep();

      // Do other stuff.
      DemoRender();
   }

   
   // Clenaup our shader
   DX_SAFE_RELEASE(gD3DVertexShader);
   DX_SAFE_RELEASE(gD3DFragmentShader);
   DX_SAFE_RELEASE(gD3DInputLayout);
   //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   // Cleanup Textures
   DX_SAFE_RELEASE(gD3DTexture);
   DX_SAFE_RELEASE(gD3DTextureShaderResourceView);
   DX_SAFE_RELEASE(gD3DPointSampler);
   // Cleanup our buffer
   DX_SAFE_RELEASE(gD3DVertexBuffer);
   DX_SAFE_RELEASE(gD3DIndexBuffer);
   // Clean up our view.
   DX_SAFE_RELEASE(gD3DRenderTargetView);

  
   D3D11Cleanup();
   WindowDestroy();
}
