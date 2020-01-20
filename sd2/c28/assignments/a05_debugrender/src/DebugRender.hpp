// NOTE:  I'm more concerned that you have the functionality, and less so about the 
// exact signature.  I'm also only adding the base or common calls - but you can easily add
// easier to use signatures that call the more generic one; 
//
// The goal is to be to easily draw in screen space from anywhere in code. 


//------------------------------------------------------------------------
#define DEFAULT_POINT_SIZE       (0.25f)
#define DEFAULT_LINE_WIDTH       (0.025f)

#define DEFAULT_SCREEN_POINT_SIZE       (8.0f)
#define DEFAULT_SCREEN_LINE_SIZE        (2.0f)


enum eDebugRenderMode 
{
   DEBUG_RENDER_USE_DEPTH, 
   DEBUG_RENDER_ALWAYS, 
   DEBUG_RENDER_XRAY, 
};

//------------------------------------------------------------------------
// Start of psuedo-required section!
//------------------------------------------------------------------------



//------------------------------------------------------------------------
// System
//------------------------------------------------------------------------
// Determines the "screen size" to use for for the debug render system
// I use 720.0f high, but use whatever makes sense to you for a "screen" coordinate
void DebugRenderSystemStartup( RenderContext *context, 
   float virtualScreenHeight = 720.0f );  // how high I want to treat the screen for glyph/screen modes; 
void DebugRenderSystemShutdown(); 

// Common functionality
void DebugRenderSystemBeginFrame(); // ...placeholder - does nothing
void DebugRenderSystemEndFrame();   // Cleanup old objects (called AFTER frame has been presented)

// Rendering
void DebugRenderToCamera( Camera *camera ); // renders camera/world space objects to this camera; 
void DebugRenderScreen();                   // render screenspace debug objects to backbuffer

void DebugRenderShow( bool show );  // enable/disable rendering debug commands
void DebugRenderClear();            // Clear all Debug Render commands that have duration; 

//------------------------------------------------------------------------
// 3D
//------------------------------------------------------------------------
void DebugRenderPoint( float duration, eDebugRenderMode mode, 
   vec3 pos, 
   rgba start_color, rgba end_color,
   float size = DEFAULT_POINT_SIZE ); 

void DebugRenderQuad( float duration, eDebugRenderMode mode, 
   vec3 pos, 
   aabb2 const &quad, 
   rgba start_color, rgba end_color,
   TextureView *view = nullptr ); 

void DebugRenderLine( float duration, eDebugRenderMode mode, 
   vec3 p0, vec3 p1, 
   rgba start_color, rgba end_color, 
   float width = DEFAULT_LINE_WIDTH ); 

void DebugRenderWireSphere( float duration, eDebugRenderMode mode, 
   vec3 pos, 
   float radius, 
   rgba start_color, rgba end_color ); 

void DebugRenderTextf( float duration, eDebugRenderMode mode, 
   vec3 pos,
   vec2 pivot, 
   float height,
   rgba start_color, rgba end_color, 
   char const *format, ... ); 

//------------------------------------------------------------------------
// 2D
//------------------------------------------------------------------------
void DebugRenderScreenPoint( float duration, 
   vec2 pos, 
   rgba start_color, rgba end_color,
   float size = DEFAULT_SCREEN_POINT_SIZE ); 

void DebugRenderScreenQuad( float duration, 
   aabb2 const &quad, 
   rgba start_color, rgba end_color,
   TextureView *view = nullptr ); 

void DebugRenderScreenLine( float duration,
   vec2 p0, vec2 p1, 
   rgba start_color, rgba end_color, 
   float width = DEFAULT_LINE_WIDTH ); 

void DebugRenderScreenTextf( float duration,
   vec2 pivot, 
   float height,
   rgba start_color, rgba end_color, 
   char const *format, ... ); 

//------------------------------------------------------------------------
// Message System (renders with screen items)
//------------------------------------------------------------------------
DebugRenderMessage( float duration,
   rgba start_color, rgba end_color, 
   char const *format, ... ); 