
Texture2D<float> tPattern   : register(t8);   // 8+ are user defined

cbuffer material_constants : register(b5) 
{
   float DISSOLVE_AMOUNT;
};


// SV_Target0 at the end means the float4 being returned
// is being drawn to the first bound color target.
float4 FragmentFunction( v2f_t input ) : SV_Target0
{
   // use the uv to sample the texture
   float dissolveHeight = tPattern.Sample( sSampler, input.uv ).x;
   clip( DISSOLVE_AMOUNT - dissolveHeight );  // this early returns if value passed in is less than 0


   //   float3 surface_color = (input.color * texture_color).xyz; // multiply our tint with our texture color to get our final color; 
   //   float surface_alpha = (input.color.a * texture_color.a); 
}
