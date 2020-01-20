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
   // we are not defining our own input data; 
   float3 position      : POSITION; 
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
// Programmable Shader Stages
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// for passing data from vertex to fragment (v-2-f)
struct v2f_t 
{
   float4 position : SV_POSITION; 
   float4 color : COLOR; 
   float2 uv : UV; 
}; 

//--------------------------------------------------------------------------------------
float RangeMap( float v, float inMin, float inMax, float outMin, float outMax ) 
{ 
   // implement me!
   ...
}

//--------------------------------------------------------------------------------------
// Vertex Shader
v2f_t VertexFunction( vs_input_t input )
{
   v2f_t v2f = (v2f_t)0;

   // can no grab it from the input stream; 
   float3 inPos = input.position; 
    
   float clip_x = RangeMap( inPos.x, ORTHO_MIN.x, ORTHO_MAX.x, -1, 1 ); 
   float clip_y = RangeMap( inPos.y, ORTHO_MIN.y, ORTHO_MAX.y, -1, 1 ); 

   // (note: technically clip_space is a homogeneous coordinate
   //  so the above is not 100% accurate, but more on that later)

   // For now, we'll just set w to 1, and forward.
   // TODO: Use ortho information to map our input coordinate 
   // into renderable clip space; 
   v2f.position = float4( clip_x, clip_y, 0.0f, 1.0f ); 

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
   // we'll outoupt our UV coordinates as color here
   // to make sure they're being passed correctly.
   // Very common rendering debugging method is to 
   // use color to portray information; 
   float4 uvAsColor = float4( input.uv, 0.0f, 1.0f ); 
   float4 finalColor = uvAsColor * input.color; 

   return finalColor; 
}
