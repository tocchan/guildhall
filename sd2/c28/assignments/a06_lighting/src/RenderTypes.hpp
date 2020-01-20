
//------------------------------------------------------------------------
// Macros or Templates
//------------------------------------------------------------------------
#define DX_SAFE_RELEASE(dx_obj)  if (dx_obj != nullptr) { dx_obj->Release(); dx_obj = nullptr; }

#define MAX_LIGHTS      (8)

//------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// A enum to pick a samplerstate to use
enum eSampleMode
{
   SAMPLE_MODE_POINT       = 0, 
   SAMPLE_MODE_LINEEAR, 
   // SAMPLE_MODE_BILINEAR

   SAMPLE_MODE_COUNT,
   SAMPLE_MODE_DEFAULT     = SAMPLE_MODE_LINEAR; // different games may want to change this.  My projects will use Linear, SD4 will want point; 
}; 

//------------------------------------------------------------------------
// How should filtering be done (MAG, MIN, & MIP)
enum eFilterMode 
{
   FILTER_MODE_POINT,
   FILTER_MODE_LINEAR,
}; 

//------------------------------------------------------------------------
// two options for this - either you can name common blend mode configurations
// using an enum, or you can fully expose it using a color and alpha blend operation; 
// Believe the enum is what Squirrel is intended for your engines; 
enum eBlendMode
{
                        // op,  src_factor,  dst_factor
   BLEND_MODE_OPAQUE,   // ADD, ONE,         ZERO              (or blend disabled)
   BLEND_MODE_ALPHA,    // ADD, SRC_ALPHA,   INV_SRC_ALPHA,
   BLEND_MODE_ADDTIVE,  // ADD, ONE,         ONE
}; 


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

//------------------------------------------------------------------------
// Eventually, when we add new Texture types, these common 
// enums & classes should be moved to Texture.hpp
enum eTextureUsageBit : uint
{
   TEXTURE_USAGE_TEXTURE_BIT              = BIT_FLAG(0),    // Can be used to create a TextureView
   TEXTURE_USAGE_COLOR_TARGET_BIT         = BIT_FLAG(1),    // Can be used to create a ColorTargetView
   TEXTURE_USAGE_DEPTH_STENCIL_TARGET_BIT = BIT_FLAG(2),    // Can be used to create a DepthStencilTargetView
}; 
typedef uint eTextureUsageBits; 

//------------------------------------------------------------------------
// In D3D11 and GL - buffers also are supplied hints on 
// desired usage patterns to help the GPU optimize for it
enum eGPUMemoryUsage 
{
   GPU_MEMORY_USAGE_GPU,     // Can be written/read from GPU only (Color Targets are a good example)
   GPU_MEMORY_USAGE_STATIC,  // Created, and are read only after that (ex: textures from images, sprite atlas)
   GPU_MEMORY_USAGE_DYNAMIC, // Update often from CPU, used by the GPU (CPU -> GPU updates, used by shaders.  ex: Uniform Buffers)
   GPU_MEMORY_USAGE_STAGING, // For getting memory from GPU to CPU (can be copied into, but not directly bound as an output.  ex: Screenshots system)
}; 

//------------------------------------------------------------------------
// As the engine designer, we're going to make the decision to 
// just reserve certain slot indices for specific constant buffers
// We will gruop uniform buffers by how often they change, and for now, we
// really only use the CAMERA
enum eCoreUniformSlot 
{
   UNIFORM_SLOT_FRAME         = 1,
   UNIFORM_SLOT_CAMERA        = 2, 
   UNIFORM_SLOT_MODEL         = 3,
}; 


//------------------------------------------------------------------------
// Constant Buffer Structs
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// I usually use this to store time, as it is a common
// input for shader effects
// A UBO should usually be a multipole of 16-bytes large, so we'll just add some 
// variations of time for our shaders to use; 
// slot: 1
struct frame_buffer_t
{
   float time; 
   float halfTime; 
   float doubleTime; 
   float quadTie; 
}; 

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


//------------------------------------------------------------------------
// used by light buffer (A06)
struct light_t 
{
   rgba color; // a is intensity; 
   
   vec3 position;                   // where is the light
   float pad00;                     // keep a 16-byte alignment for D3D11

   vec3 direction;                  // which way is hte light facing
   float is_direction       = 0.0f; // 0 means use as a point light 

   vec3 diffuse_attenuation = vec3( 1.0f, 0.0f, 0.0f );  // no attenuation
   float pad10; 

   vec3 specular_attenuation = vec3( 1.0f, 0.0f, 0.0f ); // no attenuation
   float pad20; 
}; 

//------------------------------------------------------------------------
// slot 4, lights (A06)
struct light_buffer_t
{
   vec4 ambient; 
   float spec_factor; 
   float spec_power; 
   vec2 pad; 

   light_t lights[MAX_LIGHTS]; 
}; 

// I start at slot 1 out of habit.  I reserve slot 0 for what I call the "SYTEM" buffer, which
// is usually just debug constants and configurations as I'm working. 
// The uniform buffer system is very free-form, you can bind any data to any slot
// but I like to usually have my engine renderer reserve a few slots
// grouped by update frequency.
