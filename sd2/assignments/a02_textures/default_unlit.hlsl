//--------------------------------------------------------------------------------------
// Stream Input
// ------
// Stream Input is input that is walked by the vertex shader.  
// If you say "Draw(3,0)", you are telling to the GPU to expect '3' sets, or 
// elements, of input data.  IE, 3 vertices.  Each call of the VertxShader
// we be processing a different element. 
//--------------------------------------------------------------------------------------

// inputs are made up of internal names (ie: uv) and semantic names
// (ie: TEXCOORD).  "uv" would be used in the shader file, where
// "TEXCOORD" is used from the client-side (cpp code) to attach ot. 
// The semantic and internal names can be whatever you want, 
// but know that semantics starting with SV_* usually denote special 
// inputs/outputs, so probably best to avoid that naming.
struct vs_input_t 
{
   float3 position      : POSITION; 
   float4 color         : COLOR; 
   float2 uv            : TEXCOORD; 
}; 

// Vertex to Fragment data
// What is output from the vertex stage
// and then passed to the fragment stage
//
// Notice SV_POSITION.  This special name
// denotes positions used by the rasterizer to 
// calculate pixels.  It is a required output before
// the rasterizer stage. 
struct v2f_t 
{
   float4 position      : SV_POSITION; 
   float4 color         : COLOR;
   float2 uv            : TEXCOORD; 
}; 

//--------------------------------------------------------------------------------------
// Uniform Input
// ------
// Uniform Data is also externally provided data, but instead of changing
// per vertex call, it is constant for all vertices, hence the name "Constant Buffer"
// or "Uniform Buffer".  This is read-only memory; 
//
// I tend to use all cap naming here, as it is effectively a 
// constant from the shader's perspective. 
//
// register(b2) determines the buffer unit to use.  In this case
// we'll say this data is coming from buffer slot 2. 
//--------------------------------------------------------------------------------------
cbuffer camera_constants : register(b2)
{
   float2 ORTHO_MIN; 
   float2 ORTHO_MAX; 
};

//--------------------------------------------------------------------------------------
// Texures & Samplers
// ------
// Another option for external data is a Texture.  This is usually a large
// set of data (like an image) that we want to "sample" from.  
//
// A sampler are the rules for how to collect texel data for a given UV. 
//
// Like constant buffers, these hav ea slot they're expecting to be bound
// t0 means use texture unit 0,
// s0 means use sampler unit 0,
//
// In D3D11, constant buffers, textures, and samplers all have their own set 
// of slots.  Some data types may share a slot space (for example, unordered access 
// views (uav) use the texture space). 
//--------------------------------------------------------------------------------------
Texture2D<float4> tAlbedo : register(t0); // texutre I'm using for albedo (color) information
SamplerState sAlbedo : register(s0);      // sampler I'm using for the Albedo texture

//--------------------------------------------------------------------------------------
// Programmable Shader Stages
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Vertex Shader
v2f_t VertexFunction(vs_input_t input)
{
   v2f_t v2f = (v2f_t)0;

   // we'll be getting a float3, but have to output a float4
   // which is the output in "clip space". 
   //
   // Only outputs in the following ranges will display..
   // [-1, 1] on the x (left to right)
   // [-1, 1] on the y (bottom to top)
   // [ 0, 1] on the z 

   // (note: technically clip_space is a homogeneous coordinate
   //  so the above is not 100% accurate, but more on that later)

   // For now, we'll just set w to 1, and forward.
   // TODO: Use ortho information to map our input coordinate 
   // into renderable clip space; 
   v2f.position = float4( input.position, 1.0f ); 

   // The other two inputs we'll just forward on to the pixel shader
   v2f.color = input.color; 
   v2f.uv = input.uv; 
    
   return v2f;
}

//--------------------------------------------------------------------------------------
// Fragment Shader
// 
// SV_Target0 at the end means the float4 being returned
// is being drawn to the first bound color target.
float4 FragmentFunction( v2f_t input ) : SV_Target0
{
   // First, we sample from our texture
   float4 texColor = tAlbedo.Sample( sAlbedo, input.uv ); 

   // comonent wise multiply to "tint" the output
   float4 finalColor = texColor * input.color; 

   // output it; 
   return finalColor; 
}
