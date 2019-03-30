//------------------------------------------------------------------------
// VertexTypes.hpp
//------------------------------------------------------------------------

//... 

// Vertex type used for more lit effects
struct VertexLit   
{
   vec3 position; 
   vec3 normal;   
   vec4 tangent;  // alternatively, vec3 tangent; vec3 bitangent; 
   
   rgba color;    
   vec2 uv;       

   static buffer_attribute_t LAYOUT[]; 
   static void CopyFromMaster( void *buffer, VertexMaster const *src, uint count ); 
}; 




