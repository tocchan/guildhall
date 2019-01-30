//------------------------------------------------------------------------
// Camera.hpp/cpp
//------------------------------------------------------------------------
class Camera 
{
   public:
      // What are we rendering to?
      void SetColorTarget( ColorTargetView *ctv )
      {
         m_colorTarget = ctv; 
      } 

   public:
      ColorTargetView *m_colorTarget; // What are we rendering to?

      // orthographic projection information
      // eventually just projection informtion; 
      // just representing this with a struct for now,
      // you can leave what you Camera already has; 
      camera_projection_t m_projectionInfo; 
}; 


