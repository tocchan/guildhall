// assume these are orthogonal (we could reortho, but doubt they'll change enough to matter)
   float3 normal = normalize(input.normal); 
   float3 tangent = normalize(input.tangent); 
   float3 bitangent = normalize(input.bitangent); 

   float3x3 tbn = float3x3( tangent, bitangent, normal );  
   float3 world_normal = mul( surface_normal, tbn );        //

   // lighting continues as normal
   // ... 