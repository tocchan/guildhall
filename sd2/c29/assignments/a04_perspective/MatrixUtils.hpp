


class Transform
{
   public:
      mat44 ToMatrix() const; 
      
   public:
      vec3 m_position               = vec3(0.0f);
      vec3 m_rotationEulerDegrees   = vec3(0.0f);
      vec3 m_scale                  = vec3(1.0f); 

}