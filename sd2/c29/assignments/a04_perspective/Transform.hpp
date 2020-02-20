class Transform
{
   public:
      void SetPosition( vec3 pos ); 
      void SetRotationFromPitchRollYawDegrees( float pitch, float roll, float yaw ); 

      mat44 ToMatrix() const; 
      
   public:
      vec3 m_position                     = vec3(0.0f);
      vec3 m_rotationPitchRollYawDegrees  = vec3(0.0f);  
      vec3 m_scale                        = vec3(1.0f); 
};