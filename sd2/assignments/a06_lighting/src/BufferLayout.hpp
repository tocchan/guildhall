//------------------------------------------------------------------------
// BufferLayout.hpp
//------------------------------------------------------------------------
// Introduced A06

//------------------------------------------------------------------------
enum eDataType
{
	DT_NULL, 

	DT_FLOAT, 
	DT_VEC2, 
	DT_VEC3, 
	DT_RGBA32, 
};

typedef void (*CopyFromMasterCallback)( void *dst, VertexMaster const *src, uint count ); 

//------------------------------------------------------------------------
//------------------------------------------------------------------------
struct buffer_attribute_t 
{
   std::string name;       // what is the semantic name 
   eDataType type;         // what is the type
   size_t member_offset;   // where does this attribute start

   inline bool is_null() const   { return type == DT_NULL; }
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Entire point of this class is to fully describe the input into
// the shader pipeline; 
class BufferLayout 
{
   public:
      // common constructors
      BufferLayout(); 
      BufferLayout( buffer_attribute_t const *list ); 

      inline uint get_attribute_count() const   { return m_attributes.size(); }
      inline uint get_stride() const            { return m_stride; }

   public:
      std::vector<buffer_attribute_t> m_attributes;   // what is in this buffer and how does it bind
      uint m_stride;                                  // how large is a single element
      CopyFromMasterCallback m_copyFromMaster;        // how do we copy master to this format?

   public:
      // Static ways to look for a unique layout pointer based an attribute list; 
      static BufferLayout const* For( buffer_attribute_t const *attrib_list, size_t stride ); 
      
      // templated helper
      // ussage: BufferLayout const* layout = BufferLayout::For<VertexPCU>(); 
      template <typename T>
      static BufferLayout const* For() { return For( T::LAYOUT, sizeof(T) ); } 
};


//------------------------------------------------------------------------
// Example of defining a layout;
//------------------------------------------------------------------------
// in .hpp
struct VertexPCU
{
	vec3 position; 
	rgba color; 
	vec2 uv; 

   static buffer_attribute_t[] LAYOUT; 
   static void CopyFromMaster( void *buffer, VertexMaster const *src, uint count );
};

//------------------------------------------------------------------------
// in .cpp
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


