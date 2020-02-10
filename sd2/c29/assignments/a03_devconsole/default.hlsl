//--------------------------------------------------------------------------------------
// Stream Input
// ------
// Stream Input is input that is walked by the vertex shader.  
// If you say "Draw(3,0)", you are telling to the GPU to expect '3' sets, or 
// elements, of input data.  IE, 3 vertices.  Each call of the VertxShader
// we be processing a different element. 
//--------------------------------------------------------------------------------------

// Input from Vertex Buffers
struct vs_input_t 
{
   // we are not defining our own input data; 
   float3 position      : POSITION; 
   float4 color         : COLOR; 
   float2 uv            : TEXCOORD; 
}; 


//--------------------------------------------------------------------------------------
// Constant Input
// ------
// Constant Input are buffers bound that all shaders share/read.
// It can't be changed from the shader, but can be
// changed in-between draw calls on the CPU
//--------------------------------------------------------------------------------------

// buffer holding time information from our game
cbuffer frame_constants : register(b0)          // constant buffer slot 0
{
   float SYSTEM_TIME;
   float SYSTEM_FRAME_TIME;
   float GAME_TIME;
   float GAME_DELTA_TIME;
};

// buffer telling us information about our camera
cbuffer camera_constants : register(b1)         // constant buffer slot 1
{
   float4x4 VIEW;          // aka, WorldToCameraTransform
   float4x4 PROJECTION;    // aka, CameraToClipTransform
};

// Textures & Samplers are also a form of constant
// data - uniform/constant across the entire call
Texture2D <float4> tDiffuse   : register(t0);   // color of the surface
SamplerState sSampler : register(s0);           // sampler are rules on how to sample (read) from the texture.



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
// Vertex Shader
v2f_t VertexFunction( vs_input_t input )
{
   v2f_t v2f = (v2f_t)0;

   // move the vertex through the spaces
   vec4 local_pos = float4( input.position );               // passed in position is usually inferred to be "local position", ie, local to the object
   vec4 world_pos = local_pos;  // mul( MODEL, local_pos ); // world pos is the object moved to its place int he world by the model, not used yet
   vec4 camera_pos = mul( VIEW, world_pos ); 
   vec4   = mul( PROJECTION, camera_pos ); 

   v2f.position = clip_pos; // we want to output the clip position to raster (a perspective point)
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
   // note - input.position is now pixel coordinates

   // use the uv to sample the texture
   float4 texture_color = tDiffuse.Sample( sSampler, input.uv );
   float4 final_color = input.color * texture_color; // multiply our tint with our texture color to get our final color; 
   return final_color; 
}
