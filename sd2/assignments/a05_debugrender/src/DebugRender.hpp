// NOTE:  I'm more concerned that you have the functionality, and less so about the 
// exact signature.  I'm also only adding the base or common calls - but you can easily add
// easier to use signatures that call the more generic one; 
//
// The goal is to be to easily draw in screen space from anywhere in code. 


//------------------------------------------------------------------------
#define DEFAULT_POINT_SIZE       (4.0f)
#define DEFAULT_LINE_WIDTH       (1.0f)

//------------------------------------------------------------------------
enum eDebugRenderSpace 
{
   DEBUG_RENDER_SCREEN,          // renders in screen space (ie, backbuffer), defined when initializing the system 
   DEBUG_RENDER_WORLD,           // is rendered in the world; 
   DEBUG_RENDER_CAMERA,          // [EXTRA] Glyph Mode.  Uses world coordinates to figure out viewport space to render.
}; 

enum eDebugRenderMode
{
   DEBUG_RENDER_USE_DEPTH,    // default mode - will use depth buffer and write;
   DEBUG_RENDER_ALWAYS,       // always draw, does not write to depth
   DEBUG_RENDER_XRAY,         // render behind (greater, no write), then in front (lequal, write)
                              // darken or thin the line during behind render to differentiate from it being in front; 
}; 

//------------------------------------------------------------------------
// struct to contain options for rendering something.  Contains parameters common to most 
// debug draw calls. 
struct debug_render_options_t 
{
   eDebugRenderSpace space       = DEBUG_RENDER_CAMERA; 
   eDebugRenderMode mode         = DEBUG_RENDER_USE_DEPTH; 

   float durationSeconds         = 0.0f;  // show for a single frame

   RGBA beginColor               = RGBA::WHITE; // color when this object first exist; 
   RGBA endColor                 = RGBA::WHITE; // color when this object is about to be destroyed

   // 2D common - these are where I will treat the "origin" 
   // of the screen for drawing - defaulting to bottom-left corner; 
   // used for SCREEN and CAMERA modes, ignored for WORLD
   vec4 positionRatioAndOffset   = vec4( 0.0f, 0.0f, 0.0f, 0.0f ); // this is bottom-left corner 

   // 3D (WORLD space) common (ignored for SCREEN & CAMERA)
   mat44 modelTransform          = mat44::IDENTITY; // local transform to do to the object; 
};

//------------------------------------------------------------------------
// System
//------------------------------------------------------------------------
void DebugRenderSystemStartup( RenderContext *context, 
   float virtualScreenHeight = 720.0f );  // how high I want to treat the screen for glyph/screen modes; 

void DebugRenderSystemShutdown(); 

void DebugRenderSystemBeginFrame(); // ...placeholder - does nothing
void DebugRenderSystemEndFrame();   // Cleanup old objects (called AFTER frame has been presented)

void DebugRenderToCamera( Camera *camera ); // renders camera/world space objects to this camera; 
void DebugRenderScreen();                   // render screenspace debug objects to backbuffer


//------------------------------------------------------------------------
// 2D Debug Rendering (defaults to SCREEN)
//------------------------------------------------------------------------
void DebugRenderPoint2D( debug_render_options_t const &options, 
   vec2 position, 
   float size = DEFAULT_POINT_SIZE );

void DebugRenderLine2D( debug_render_options_t const &options, 
   vec2 start, vec2 end, 
   float lineWidth = DEFAULT_LINE_WIDTH );

void DebugRenderQuad2D( debug_render_options_t const &options, 
   AABB2 const &quad ); 

void DebugRenderTexturedQuad2D( debug_render_options_t const &options, 
   AABB2 const &quad, 
   TextureView *view ); 

void DebugRenderWireQuad2D( debug_render_options_t const &options, 
   AABB2 const &quad, 
   float lineWidth = DEFAULT_LINE_WIDTH ); 

// to get a ring, you can jus tuse a innerRadius line-thickness smaller than radius; 
void DebugRenderDisc2D( debug_render_options_t const &options, 
   vec2 center, 
   float radius, 
   float innerRadius = 0.0f ); 

//------------------------------------------------------------------------
// 3D Rendering (will always default to WORLD)
//------------------------------------------------------------------------
void DebugRenderPoint( debug_render_options_t const &options, 
   vec3 position, 
   float size = DEFAULT_POINT_SIZE );

void DebugRenderLine( debug_render_options_t const &options, 
   vec3 start, vec3 end, 
   float lineWidth = DEFAULT_LINE_WIDTH );

void DebugRenderArrow3D( debug_render_options_t const &options, 
   vec3 start, vec3 end, 
   float base_thickness, 
   float head_thickness ); 

void DebugRenderSphere( debug_render_options_t const &options, 
   vec3 center, 
   float radius ); 

void DebugRenderBox( debug_render_options_t const &options, 
   AABB3 box ); 

// EXTRA (helps to be able to set raster fill mode to "wire")
// Also, better to use an ICOSphere if available, but UV sphere is fine; 
void DebugRenderWireSphere( debug_render_options_t const &options,
   vec3 center, 
   float radius ); 

// EXTRA (helps to be able to set raster fill mode to "wire")
void DebugRenderWireBox( debug_render_options_t const &options, 
   AABB3 box ); 

// EXTRA (requires being able to render a cone/cylindar)
void DebugRenderArrow( debug_render_options_t const &options, 
   vec3 start, vec3 end, 
   float lineWidth = DEFAULT_LINE_WIDTH ); 

// EXTRA - requires Arrow
void DebugRenderBasis( debug_render_options_t const &options, 
   mat44 const &basis, 
   float lineWidth = DEFAULT_LINE_WIDTH ); 

//------------------------------------------------------------------------
// Rendering Text (works in any mode)
//------------------------------------------------------------------------
void DebugRenderTextv( debug_render_options_t const &options, 
   vec2 pivot, char const *format, va_list args );
void DebugRenderTextf( debug_render_options_t const &options, 
   vec2 pivot, char const *format, ... ); 

//------------------------------------------------------------------------
// Logs (shows up on screen temporily in a list)
// ALWAYS assumes screen space; 
//------------------------------------------------------------------------
void DebugRenderLogv( debug_render_options_t const &options, 
   char const *format, 
   va_list args ); 
void DebugRenderLogf( debug_render_options_t const &options, 
   char const *format,
   ... ); 
