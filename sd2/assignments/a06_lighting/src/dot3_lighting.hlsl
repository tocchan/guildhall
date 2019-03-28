#define MAX_LIGHTS (8)

struct vs_input_t
{
   float3 position         : POSITION;
   float3 normal           : NORMAL;   // this is new - need buffer layouts to get it there; 
   float4 color            : COLOR;
   float2 uv               : UV; 
};


struct light_t 
{
   float3 color; 
   float intensity; 

   float3 position; 
   float is_directional;

   float3 direction; 
   float pad10; 

   float3 diffuse_attenuation; 
   float pad20; 

   float3 specular_attenuation;
   float pad30; 
};


//--------------------------------------------------------------------------------------
cbuffer camera_constants : register(b2)
{
   float4x4 VIEW;
   float4x4 PROJECTION;
   float4x4 VIEW_PROJECTION;  // optional 

   float3 CAMERA_POSITION;    float cam_unused0;   // NEW - make sure this is updated and passed in per frame; 
};

//--------------------------------------------------------------------------------------
cbuffer light_buffer : register(b4) 
{
   float4 AMBIENT; 

   float SPEC_FACTOR;   // (0, 1), limits specular amount
   float SPEC_POWER; 
   float2 pad00; 

   light_t LIGHTS[MAX_LIGHTS]; 
};

//--------------------------------------------------------------------------------------
struct lighting_t
{
   float3 diffuse; 
   float3 specular; 
};


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
lighting_t GetLighting( float3 eye_pos, 
   float3 surface_position, 
   float3 surface_normal )
{
   // 
   lighting_t lighting; 
   lighting.diffuse = (AMBIENT.xyz * AMBIENT.w); 

   float3 dir_to_eye = normalize(eye_pos - surface_position); 


   // directional light; 
   for (int i = 0; i < MAX_LIGHTS; ++i) {
      light_t light = LIGHTS[i]; 

      // directional 
      float3 dir_dir = light.direction; 
      float3 point_dir = normalize(surface_position - light.position); 
      float3 light_dir = lerp( point_dir, dir_dir, light.is_directional ); 

      // common things
      // directional light
      float dir_dist = abs( dot( (surface_position - light.position), light.direction ) );   // for directional
      float point_dist = length( surface_position - light.position );                          // for point
      float distance = lerp( point_dist, dir_dist, light.is_directional ); 

      // Diffuse Part
      float3 la = light.diffuse_attenuation; 
      float attenuation = 1.0f / (la.x + la.y * distance + la.z * distance * distance); 
      float dot3 = max( dot( -light_dir, surface_normal ), 0.0f ); 

      float3 diffuse_color = light.color * light.intensity * attenuation * dot3; 
      lighting.diffuse += diffuse_color; 

      // Specular 
      // blinn-phong 
      // dot( H, N );  -> H == half_vector, N == normal
      vec3 dir_to_light = -light_dir; 
      float3 half_vector = normalize( dir_to_eye + dir_to_light ); 
      float spec_coefficient = max( dot( half_vector, normal ), 0.0f ); // DO not saturate - spec can go higher;  

      float3 sa = light.specular_attenuation; 
      float spec_attenuation = 1.0f / (sa.x + sa.y * distance + sa.z * distance * distance); 

      // finalize coefficient
      spec_coefficient = SPEC_FACTOR * pow( spec_coefficient, SPEC_POWER ); 
      float3 specular_color = light.color * light.intensity * spec_attenuation * spec_coefficient; 
      lighting.specular += specular_color; 
   }

   lighting.diffuse = saturate(light.diffuse); // clamp this to (0, 1)
   // light.specular is untouched - greater than one can tell us information on how bright it is - used for bloom;  

   return lighting; 
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
float4 FragmentShader( v2f_t input ) : SV_TARGET0
{
   float4 surface_color = ...; // from texture; 
   surface_color = pow( surface_color, GAMMA );  // move to linear space; 

   lighting_t lighting = GetLighting( CAMERA_POSITION, input.normal ); 

   float4 final_color = float4(lighting.diffuse, 1.0f) * surface_color
      + float4(lighting.specular, 0.0f); 


   final_color = pow( final_color, 1.0f / GAMMA ); // convert back to sRGB space
   return final_color; 
};
