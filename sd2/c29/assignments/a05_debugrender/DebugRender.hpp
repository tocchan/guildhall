#pragma once

/************************************************************************/
/*                                                                      */
/* TYPES                                                                */
/*                                                                      */
/************************************************************************/

// Only effects "world" mode stuff; 
enum eDebugRenderMode
{
   DEBUG_RENDER_ALWAYS,          // what is rendered always shows up
   DEBUG_RENDER_USE_DEPTH,       // respect the depth buffer
   DEBUG_RENDER_XRAY,            // renders twice - once darker when it shoudl be hidden, and once more saturated when it should appear
}; 

/************************************************************************/
/*                                                                      */
/* FUNCTION PROTOTYPES                                                  */
/*                                                                      */
/************************************************************************/
//------------------------------------------------------------------------
//  System
//------------------------------------------------------------------------
// setup
void DebugRenderSystemStartup();
void DebugRenderSystemShutdown();

// control
void EnableDebugRendering(); 
void DisableDebugRendering(); 

// output
void DebugRenderBeginFrame();                   // Does nothing, here for completeness.
void DebugRenderWorldToCamera( Camera* cam );   // Draws all world objects to this camera 
void DebugRenderScreenTo( Texture* output );    // Draws all screen objects onto this texture (screen coordinate system is up to you.  I like a 1080p default)
void DebugRenderEndFrame();                     // Clean up dead objects

//------------------------------------------------------------------------
// World Rendering
//------------------------------------------------------------------------
// points
void DebugAddWorldPoint( vec3 pos, float size, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode ); 
void DebugAddWorldPoint( vec3 pos, float size, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWorldPoint( vec3 pos, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// lines
void DebugAddWorldLine( vec3 p0, rgba p0_start_color, rgba p0_end_color, 
   vec3 p1, rgba p1_start_color, rgba p1_end_color, 
   float duration, 
   eDebugRenderMode mode ); 
void DebugAddWorldLine( vec3 start, vec3 end, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// line strip [extra]
void DebugAddWorldLineStrip( uint count, vec3 const* positions, 
   rgba start_p0_color, rgba start_pf_color,    // color of first/end point at the beginning of duration
   rgba end_p0_color, rgba end_pf_color,        // color of first/end point at the end of duration
   float duration, 
   eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// arrows
void DebugAddWorldArrow( vec3 p0, rgba p0_start_color, rgba p0_end_color, 
   vec3 p1, rgba p1_start_color, rgba p1_end_color, 
   float duration, 
   eDebugRenderMode mode ); 
void DebugAddWorldArrow( vec3 start, vec3 end, rgba color, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// Quads
void DebugAddWorldQuad( vec3 p0, vec3 p1, vec3 p2, vec3 p4, aabb2 uvs, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// bounds
void DebugAddWorldWireBounds( obb3 bounds, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWorldWireBounds( obb3 bounds, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWorldWireBounds( aabb3 bounds, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

void DebugAddWorldWireSphere( vec3 pos, float radius, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWorldWireSphere( vec3 pos, float radius, rgba color, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// basis
void DebugAddWorldBasis( mat44 basis, rgba start_tint, rgba end_tint, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWorldBasis( mat44 basis, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 

// text
// non-billboarded will be oriented in the world based on the passed in basis matrix 
void DebugAddWorldText( mat44 basis, vec2 pivot, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode, char const* text ); 
void DebugAddWorldTextf( mat44 basis, vec2 pivot, rgba color, float duration, eDebugRenderMode mode, char const* text, ... ); 
void DebugAddWorldTextf( mat44 basis, vec2 pivot, rgba color, char const* text, ... ); // assume DEBUG_RENDER_USE_DEPTH

// this text will always orient itself to the current camera
void DebugAddWorldBillboardText( vec3 origin, vec2 pivot, rgba start_color, rgba end_color, float duration, eDebugRenderMode mode, char const* text ); 
void DebugAddWorldBillboardTextf( vec3 origin, vec2 pivot, rgba color, float duration, eDebugRenderMode mode, char const* format, ... ); 
void DebugAddWorldBillboardTextf( vec3 origin, vec2 pivot, rgba color, char const* format, ... ); 

// grid [extra] 
void DebugAddWorldGrid( vec3 origin, 
         vec3 i, float iMin, float iMax, float iMinorSegment, iMajorSegment, rgba iMinorColor, rgba iMajorColor, rgba iOriginColor,
         vec3 j, float jMin, float jMax, float jMinorSegment, jMajorSegment, rgba jMinorcolor, rgba jMajorColor, rgba jOriginColor, 
         rgba axisColor );
void DebugAddWorldXYGrid(); 
void DebugAddWorldXZGrid(); 
void DebugAddWorldYZGrid(); 

// mesh [extra]
void DebugAddWireMeshToWorld( mat44 transform, GPUMesh* mesh, rgba start_tint, rgba end_tint, float duration, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 
void DebugAddWireMeshToWorld( mat44 transform, GPUMesh* mesh, float duration = 0.0f, eDebugRenderMode mode = DEBUG_RENDER_USE_DEPTH ); 


//------------------------------------------------------------------------
// Screen Rendering
//------------------------------------------------------------------------
void DebugRenderSetScreenHeight( float height ); // default to 1080.0f when system starts up.  Meaning (0,0) should always be bottom left, (aspect * height, height) is top right
aabb2 DebugGetScreenBounds();                    // useful if you want to align to top right for something

// points
void DebugAddScreenPoint( vec2 pos, float size, rgba start_color, rgba end_color, float duration ); 
void DebugAddScreenPoint( vec2 pos, float size, rgba color, float duration = 0.0f ); 
void DebugAddScreenPoint( vec2 pos, rgba color ); // assumed size;

// lines
void DebugAddScreenLine( vec2 p0, rgba p0_start_color, rgba p0_end_color, 
   vec2 p1, rgba p1_start_color, rgba p1_end_color, 
   float duration ); 
void DebugAddScreenLine( vec2 p0, vec2 p1, rgba color, float duration = 0.0f ); 

// arrows
void DebugAddScreenArrow( vec2 p0, rgba p0_start_color, rgba p0_end_color, 
   vec2 p1, rgba p1_start_color, rgba p1_end_color, 
   float duration ); 
void DebugAddScreenArrow( vec2 p0, vec2 p1, rgba color, float duration = 0.0f ); 

// quad
void DebugAddScreenQuad( aabb2 bounds, rgba start_color, rgba end_color, float duration ); 
void DebugAddScreenQuad( aabb2 bounds, rgba color, float duration = 0.0f ); 

// texture
void DebugAddScreenTexturedQuad( aabb2 bounds, Texture* tex, aabb2 uvs, rgba start_tint, rgba end_tint, float duration = 0.0f ); 
void DebugAddScreenTexturedQuad( aabb2 bounds, Texture* tex, aabb2 uvs, rgba tint, float duration = 0.0f ); 
void DebugAddScreenTexturedQuad( aabb2 bounds, Texture* tex, rgba tint = rgba::WHITE, float duration = 0.0f ); // assume UVs are full texture

// text
void DebugAddScreenText( vec4 pos, vec2 pivot, float size, rgba start_color, rgba end_color, float duration, char const* text ); 
void DebugAddScreenTextf( vec4 pos, vec2 pivot, float size, rgba start_color, rgba end_color, float duration, char const* format, ... ); 
void DebugAddScreenTextf( vec4 pos, vec2 pivot, float size, rgba color, float duration, char const* format, ... ); 
void DebugAddScreenTextf( vec4 pos, vec2 pivot, float size, rgba color, char const* format, ... ); 
void DebugAddScreenTextf( vec4 pos, vec2 pivot, rgba color, char const* format, ... ); 

// screen basis [extra]
void DebugAddScreenBasis( vec2 screen_origin_location, mat44 basis_to_render, rgba start_tint, rgba end_tint, float duration ); 
void DebugAddScreenBasis( vec2 screen_origin_location, mat44 basis_to_render, rgba tint = rgba::WHITE, float duration = 0.0f ); 

// message log system [extra]
void DebugAddMessage( float duration, rgba color, char const* format, ... ); 
void DebugAddMessage( rgba color, char const* format, ... ); 


#endif 
