static Camera* gDebugCamera;

void DebugRenderWorldTo( Camera* cam )
{
   // copy all the data from cam -> gDebugCamera


   // color targets
   // depth stencil target
   // model matrix (transform)
   // perspective matrix 

   gDebugCamera->SetColorTarget( cam->GetColorTarget() );
   gDebugCamera->SetDepthStencil( cam->GetDepthStencilTarget() );
   gDebugCamera->SetTransform( cam->GetTransform() );
   gDebugCamera->SetPerspectiveMatrix( cam->GetPerspectiveMatrix() ); 
   gDebugCamera->SetViewport( cam->GetViewport() ); 

   // two unique ubo objects, just so happen have the same data
   // so deleting doesn't effect the other.

   gDebugCamera->SetClearMode( CLEAR_NONE ); 

   // Rendering
   rc->BeginCamera( gDebugCamera ); 
   // ...



   // if you want to be really safe;
   gDebugCamera->SetColorTarget(nullptr); 
   gDebugCamera->SetDepthStencil(nullptr); 
} 


// Alternative
{
   clear_options_t clear_options = cam->SaveClearOptions();
   cam->SetClearMode( CLEAR_NONE ); 

   // render

   cam->LoadClearOptions( clear_options ); 
}


// What are my clear states?
enum eClearBit : uint
{
   CLEAR_NONE = 0, 
   CLEAR_COLOR          = BIT_FLAG(0), // 1
   CLEAR_DEPTH          = BIT_FLAG(1), // 2
   CLEAR_STENCIL        = BIT_FLAG(2), // 4   // BIT_FLAG(b) (1 << b)

   CLEAR_ALL            = ~0U;         // 0xffff`ffff
};
typedef uint eClearBits; 

Camera::SetClearMode( eClearBits clear_bits, rgba color = rgba(0), float depth = 0.0f, uint8_t stencil = 0 );

CLEAR_COLOR | CLEAR_DEPTH
-> ClearColorTargetView
-> ClearDepthStecilView

IS_BIT_FLAG_SET( flags, b )     ((flags & b) == b)
