//------------------------------------------------------------------------
// Camera.hpp
//------------------------------------------------------------------------

class Camera
{
   public:
      //...

      // projection; 
      void SetPerspectiveProjection( float fov, float nz, float fz );                                 // A04 

      // Targets
      void SetDepthStencilTarget( DepthStencilTargetView *dsv );                                      // A04

      // Transforms & Matrices
      void SetModelMatrix( mat44 camModel );                                                          // A04
      mat44 GetModelMatrix() const;                                                                   // A04
      mat44 GetViewMatrix() const;                                                                    // A04
      mat44 GetProjectionMatrix() const;                                                              // A04

   public:
      // Is updated - must pass view and projection matrices; 
      void UpdateUniformBuffer( RenderContext *ctx );                                                 // A02, A04

   public:
      // configuration - stores enough information so that we 
      // can construct our projection from our current target/viewport
      projection_info_t m_projectionInfo;                                                             // A04 (optional)

      // targets
      ColorTargetView *m_colorTarget;                                                                 // A01
      DepthStencilTargetView *m_depthStencilTarget;                                                   // A05

      // cached matrices
      mat44 m_projection;                                                                             // A02
      mat44 m_camera;                                                                                 // A04 (camera's model)
      mat44 m_view; // is inverse of m_camera.                                                        // A04

      //...
}; 

