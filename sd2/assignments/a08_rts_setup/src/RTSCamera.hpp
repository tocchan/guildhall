// Optionally, you can have RTSCamera own a Camera
class RTSCamera : public Camera
{
   public:
      // Called each frame to update the underlying camera with the RTSCamera's options; 
      void Update(); 

      void SetFocalPoint( vec3 const &pos ); 
      void SetZoom( float zoom ); 
      void SetAngle( float angle ); // really is setting an angle offset

      // Whatever helpers you would prefer - I like having methods
      // to changing the above by a delta
      // ...


   public:
      vec3 m_focalPoint; 
      float m_distance; 
      
      // configuration - mess with these numbers to get a view you like; 
      float m_minDistance     = 2.0f;
      float m_maxDistance     = 16.0f;

      float m_defaultAngle    = -45.0f; 
      float m_defaultTilt     = 60.0f; 

      // used for moving from the default
      float m_angleOffset     = 0.0f; 

      // juice
      // ...to make a smoother camera, you may want to save off "targets" in which
      // to trend toward over multiple frames.  Not required, but that sort of logic I think
      // lives comfortably on the camera

}; 