//------------------------------------------------------------------------
// VertexTypes.hpp
//------------------------------------------------------------------------

// Used for CPUMesh construction; 
struct VertexMaster  // A05
{
   vec3 position;    // A05
   vec3 normal;      // A06
   vec4 tangent;     // A07

   rgba color;       // A05
   vec2 uv;          // A05
};

// SD1 basic vertex type (used for most unlit effects)
struct VertexPCU     // from SD1
{
	vec3 position; 
	rgba color; 
	vec2 uv; 

   static buffer_attribute_t LAYOUT[];                                              // A06
   static void CopyFromMaster( void *buffer, VertexMaster const *src, uint count ); // A06
};

// Vertex type used for more lit effects
struct VertexLit      // A06
{
   vec3 position; // A06
   vec3 normal;   // A06
   vec4 tangent;  // A07 - alternatively, vec3 tangent; vec3 bitangent; 
   
   rgba color;    // A06
   vec2 uv;       // A06

   static buffer_attribute_t LAYOUT[]; 
   static void CopyFromMaster( void *buffer, VertexMaster const *src, uint count ); 
}; 

// Vertex type used for vertex painting, used for our map; 
struct VertexTerrain    // A08
{
   vec3 position; 
   vec3 normal; 
   vec4 tangent; 

   vec4 texture_weights;   // I probably wouldn't add to vertex master - will likely set this manually
   rgba color;    
   vec2 uv;       
   
   static buffer_attribute_t LAYOUT[]; 
   static void CopyFromMaster( void *buffer, VertexMaster const *src, uint count ); 
}; 



//------------------------------------------------------------------------
// in .cpp - example of defining one - introduced A06
//------------------------------------------------------------------------
//------------------------------------------------------------------------
STATIC buffer_attribute_t const VertexPCU::LAYOUT[] = {
   buffer_attribute_t( "POSITION",  RDF_VEC3,      offsetof(VertexPCU, position) ), 
   buffer_attribute_t( "COLOR",     RDF_RGBA32,    offsetof(VertexPCU, color) ), 
   buffer_attribute_t( "UV",        RDF_VEC2,      offsetof(VertexPCU, uv) ), 
   buffer_attribute_t() // end
};

//------------------------------------------------------------------------
// a function to convert VertexMaster's to VertexPCU's
STATIC void VertexPCU::CopyFromMaster( void *buffer, VertexMaster const *src, uint count ) 
{
	VertexPCU *dst = (VertexPCU*)buffer; 

	for (uint i = 0; i < count; ++i) {
		dst[i].position = src[i].position; 
      // ... color
      // ... uv
   }
}


