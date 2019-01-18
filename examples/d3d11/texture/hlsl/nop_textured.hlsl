// Texture - I put the <float4> at the end to really drive home this is a 4-channle texture
Texture2D <float4> tImage : register(t0);

// SamplerState - HOW do we sample the texture.  
SamplerState sSampler : register(s0);

// Input to vertex Shader
struct vs_input_t
{
   float3 position : POSITION;

   // NEW - UV Coordinate - textures are sampled using UV coordinates, which are coordinates that 
   // span [0,1] over the whole of the texture.
   // In D3D, <0,0> corresponds to to the TOP LEFT
   //    and  <1,1> corresponds to the BOTTOM RIGHT 
   float2 uv : UV;
};

struct v2f_t
{
   // SV_POSITION is a semantic - or a name identifying this variable. 
   // Usually a semantic can be any name we want - but the SV_* family
   // usually denote special meaning (SV = System Variable I believe)
   // SV_POSITION denotes that this is output in clip space, and will be 
   // use for rasterization.  When it is the input (pixel shader stage), it will
   // actually hold the pixel coordinates.
   float4 position : SV_Position;

   // We can only pass data to the vertex shader - so to get to the fragment shader
   // we must pass it through.
   float2 uv : UV;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
v2f_t VertexFunction(vs_input_t input)
{
    v2f_t v2f = (v2f_t)0;
    
    // The output of a vertex shader is in clip-space, which is a 4D vector
    // so we need to convert out input to a 4D vector.
    v2f.position = float4( input.position, 1.0f );

    // do nothing but pass it through/
    v2f.uv = input.uv;
    
    // And return - this will pass it on to the next stage in the pipeline;
    return v2f;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
// If I'm only returning one value, I can optionally just mark the return value with
// a SEMANTIC - in this case, SV_TARGET0, which means it is outputting to the first colour 
// target.
float4 FragmentFunction(v2f_t input) : SV_Target0 // semeantic of what I'm returning
{
   // Sample the texture at the passed in UV coordinate
   float4 diffuse = tImage.Sample( sSampler, input.uv );

   // We just output a solid colour - in this case, white. 
   return diffuse;
}