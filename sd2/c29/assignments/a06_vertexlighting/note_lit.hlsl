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

   float3 normal        : NORMAL; 
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


// information that might change per model/object
cbuffer model_constants : register(b2)         // constant buffer slot 1
{
   float4x4 MODEL;          
   float4 TINT;    
};

struct light_t
{
   float3 world_position; 
   float pad00;  // this is not required, but know the GPU will add this padding to make the next variable 16-byte aligned

   float3 color; 
   float intensity; // rgb and an intensity
};

/*
// cpu side
struct light_t
{
   Vec3 worldPosition; 
   float pad00; 
   Vec3 color; 
   float intesity; // RGBA will not work - is not the same size;  
};
*/

// buffer telling us information about our camera
cbuffer light_constants : register(b3)         // constant buffer slot 1
{
   float4 AMBIENT; 
   light_t LIGHT; 
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

   float3 world_position : WORLD_POSITION; 
   float3 world_normal : WORLD_NORMAL; 
}; 

//--------------------------------------------------------------------------------------
// Vertex Shader
v2f_t VertexFunction( vs_input_t input )
{
   v2f_t v2f = (v2f_t)0;

   // move the vertex through the spaces
   vec4 local_pos = float4( input.position, 1.0f );               // passed in position is usually inferred to be "local position", ie, local to the object
   vec4 world_pos =  mul( MODEL, local_pos );               // world pos is the object moved to its place int he world by the model, not used yet
   vec4 camera_pos = mul( VIEW, world_pos ); 
   vec4   = mul( PROJECTION, camera_pos ); 

   // normal is currently in model/local space
   vec4 local_normal = float4( input.normal, 0.0f );
   vec4 world_normal = mul( MODEL, local_normal ); 

   // tangent & bitangent

   v2f.position = clip_pos; // we want to output the clip position to raster (a perspective point)
   v2f.color = input.color * TINT; 
   v2f.uv = input.uv; 
   v2f.world_position = world_position.xyz; 
   v2f.world_normal = world_normal.xyz; 

   return v2f;
}

//--------------------------------------------------------------------------------------
// Fragment Shader
// 
// SV_Target0 at the end means the float4 being returned
// is being drawn to the first bound color target.
float4 FragmentFunction( v2f_t input ) : SV_Target0
{
   // use the uv to sample the texture
   float4 texture_color = tDiffuse.Sample( sSampler, input.uv );
   float3 surface_color = (input.color * texture_color).xyz; // multiply our tint with our texture color to get our final color; 
   float3 surface_alpha = (input.color.a * texture_color.a); 

   float3 diffuse = AMBIENT.xyz * AMBIENT.w; // ambient color * ambient intensity

   // get my surface normal - this comes from the vertex format
   // We now have a NEW vertex format
   float3 surface_normal = normalize( input.world_normal ); 

   // for each light, we going to add in dot3 factor it
   float3 light_position = LIGHT.world_position; 
   float3 dir_to_light = normalize( light_position - input.world_position );
   float dot3 = max( 0.0f, dot( dir_to_light, surface_normal ) ); 

   diffuse += dot3; 

   // just diffuse lighting
   diffuse = min( float3(1,1,1), diffuse ); 
   diffuse = saturate(diffuse); // saturate is clamp01(v)
   float final_color = diffuse * surface_color; 

   
   return float4( final_color, surface_alpha );  
}
