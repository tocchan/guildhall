
//------------------------------------------------------------------------
// Macros or Templates
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------

enum eCompareOp            // A04
{
   COMPARE_NEVER,       // false,      "never"
   COMPARE_ALWAYS,      // true,       "always"
   COMPARE_EQUAL,       // ==,         "equal"
   COMPARE_NOTEQUAL,    // !=,         "not"
   COMPARE_LESS,        // <           "less"
   COMPARE_LEQUAL,      // <=          "lequal"
   COMPARE_GREATER,     // >           "greater"
   COMPARE_GEQUAL,      // >=          "gequal"
}; 

//------------------------------------------------------------------------
// We have to specify what can a buffer be used 
// for.  It is possible for a buffer to serve multiple purposes
// though for our case, we'll be special casing them for clarity; 
enum eRenderBufferUsageBit : uint
{
   RENDER_BUFFER_USAGE_VERTEX_STREAM_BIT   = BIT_FLAG(0),   // Can be bound to an vertex input stream slot
   RENDER_BUFFER_USAGE_INDEX_STREAM_BIT    = BIT_FLAG(1),   // Can be bound as an index input stream.  
   RENDER_BUFFER_USAGE_UNIFORMS_BIT        = BIT_FLAG(2),   // Can be bound to a constant buffer slot; 
};
typedef uint eRenderBufferUsageBits; 

// ...

//------------------------------------------------------------------------
// slot 2; 
// What data does the camera provide the shader (slot:2)
// This must follow a 16-byte element alignemnt (for now, this only contains
// ortho information; 
// camera slot I usually bind all constants that change per view; 
struct camera_buffer_t 
{
   mat44 view; 
   mat44 projection; 
};

//------------------------------------------------------------------------
// slot three - per model core set
// (engine set variables that may change per object); 
struct model_buffer_t 
{
   mat44 model; 
}; 

// I start at slot 1 out of habit.  I reserve slot 0 for what I call the "SYTEM" buffer, which
// is usually just debug constants and configurations as I'm working. 
// The uniform buffer system is very free-form, you can bind any data to any slot
// but I like to usually have my engine renderer reserve a few slots
// grouped by update frequency.
