//------------------------------------------------------------------------
// Camera.hpp
//------------------------------------------------------------------------

class Camera
{
   public:
      // projection; 
      void SetOrthographicProjection( vec2 min, vec2 max, float minZ = -1.0f, float maxZ = 1.0f );    // A02
      void SetPerspectiveProjection( float fov, float nz, float fz );                                 // A04 

      // Targets
      void SetColorTarget( ColorTargetView *ctv );                                                    // A04
      void SetDepthStencilTarget( DepthStencilTargetView *dsv );                                      // A04

      // Viewports
      void SetNormalizedViewport( aabb2 viewport );                                                   // ???
      aabb2 GetNormalizedViewport() const;                                                            // ???
      aabb2 GetPixelViewport() const;                                                                 // ??? 

      // Transforms & Matrices
      void SetModelMatrix( mat44 camModel );                                                          // A04
      mat44 GetModelMatrix() const;                                                                   // A04
      mat44 GetViewMatrix() const;                                                                    // A04
      mat44 GetProjectionMatrix() const;                                                              // A04

		// Space Conversions
      // Client to World
		vec2 ClientToUV( vec2 client ) const; 			                                                   // ???
		vec3 ClientToNDC( vec2 client, float ndcDepth = 1.0f ) const;                                   // ???
		vec3 NDCToWorld( vec3 ndc ) const;                                                              // ???
		vec3 ClientToWorld( vec2 client, float ndcDepth = 1.0f ) const;                                 // ???
		Ray3D ClientToWorldRay( vec2 client );                                                          // ???

		// World to Client
		vec2 WorldToClient( vec3 worldPos ) const;                                                      // ???
		vec3 WorldToNDC( vec3 worldPos ) const;                                                         // ???
		vec2 WorldToUV( vec3 worldPos ); const;                                                         // ???

      // Utility
      Frustum GetWorldFrustum() const;                                                                // ???
      aabb2 GetViewBoundsAtDepth( float z = 0.0f );                                                   // ???

   public:
      // public - but should only be called from EngineCode as it knows when it needs
      // to do the copy from CPU to GPU
      void UpdateUniformBuffer( RenderContext *ctx );                                                 // A02

   public:
      // configuration
      projection_info_t m_projectionInfo;                                                             // A04
      aabb2 m_normalizedViewport;                                                                     // ???      

      // targets
      ColorTargetView *m_colorTarget;                                                                 // A01
      DepthStencilTargetView *m_depthStencilTarget;                                                   // A05

      // cached matrices
      mat44 m_projection;                                                                             // A02
      mat44 m_camera;                                                                                 // A04
      mat44 m_view; // is inverse of m_camera.                                                        // A04
      mat44 m_ndcToWorld; // View to World matrix (invert(proj) * invert(view) (in row major)         // ???

      // Used by rendering system; 
      UniformBuffer *m_cameraUBO    = nullptr;                                                        // A02
}; 

