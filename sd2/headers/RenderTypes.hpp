//------------------------------------------------------------------------
// Enums
//------------------------------------------------------------------------

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

// I start at slot 1 out of habit.  I reserve slot 0 for what I call the "SYTEM" buffer, which
// is usually just debug constants and configurations as I'm working. 
// The uniform buffer system is very free-form, you can bind any data to any slot
// but I like to usually have my engine renderer reserve a few slots
// grouped by update frequency.
