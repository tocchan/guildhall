// input to the vertex shader - for now, just a position
struct vs_input_t
{
   float3 position : POSITION;
};

// Output from Vertex Shader, and Input to Fragment Shader
// For now, just position
struct v2f_t
{
   // SV_POSITION is a semantic - or a name identifying this variable. 
   // Usually a semantic can be any name we want - but the SV_* family
   // usually denote special meaning (SV = System Variable I believe)
   // SV_POSITION denotes that this is output in clip space, and will be 
   // use for rasterization.  When it is the input (pixel shader stage), it will
   // actually hold the pixel coordinates.
   float4 position : SV_Position;
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
   // We just output a solid colour - in this case, white. 
   return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}