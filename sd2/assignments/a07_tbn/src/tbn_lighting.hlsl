
Texture2D<float4> tDiffuseMap;  // default "white"
Texture2D<float4> tNormalMap;   // default "flat" (.5, .5, 1.0)
Texture2D<float4> tEmissiveMap; // defualt "black"


float4 FragmentFunction( v2f_t input ) 
{
   float4 normal_color = tNormalMap.Sample( sLinear, input.uv ); 

   // transform from color space to a hemisphere (z can't be negative, so we give double precision to z)
   // shorthand - map (0, 0, 0) to (1, 1, 1) -> (-1, -1, 0) to (1, 1, 1)
   float3 surface_normal = normal_color.xyz * float3(2, 2, 1) - float3(1, 1, 0); 

   float3 vertex_tangent = normalize(input.tangent); 
   float3 vertex_bitan = normalize(input.bitangent); 
   float3 vertex_normal = normalize(input.normal); 

   // commonly referred to the TBN matrix
   float3x3 surface_to_world = float3x3( vertex_tangent, vertex_bitan, vertex_normal ); 

   // if you just go with my matrix format...
   float3 world_normal = mul( surface_normal, surface_to_world ); 

   // OR... if you're stubborn
   // surface_to_world = transpose(surface_to_world); 
   // float3 world_normal = mul( surface_to_world, surface_normal ); 

   lighting_t lighting = CalculateLighting( world_normal, ... ); 
   //... apply lighting

   // EMISSIVE (map defaults to "black"); 
   float4 emissive = tEmissiveMap.Sample( sLinear, input.uv ) * EMISSIVE_FACTOR; 
   final_color += float4(emmisive.xyz * emissive.w, 0); 

   return final_color; 
}