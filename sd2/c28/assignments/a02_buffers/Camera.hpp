//------------------------------------------------------------------------
// Camera.hpp
//------------------------------------------------------------------------


class Camera
{
   public:
      void SetOrthographicProjection( vec2 min, vec2 max, float minZ = -1.0f, float maxZ = 1.0f ); 

   public:
      // public - but should only be called from EngineCode as it knows when it needs
      // to do the copy from CPU to GPU
      void UpdateUniformBuffer( RenderContext *ctx ); 

   public:
      // some local data to define our ortho
      // (we may want to store it different ways - good
      // use case for like a std::any or enum and union; 
      vec2 m_minOrtho; 
      vec2 m_maxOrtho; 

      // UniformBuffer that will store our camera information
      UniformBuffer *m_cameraUBO    = nullptr; // UBO -> Uniform Buffer Object
}; 

//------------------------------------------------------------------------
// Camera.cpp
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void Camera::UpdateUniformBuffer( RenderContext *ctx )
{
   if (m_cameraUBO == nullptr) {
      m_cameraUBO = new UniformBuffer( ctx ); 
   }

   // TODO: This method is called during BeginCamera - so we know our outputs
   // are final - so if you have enough data to figure out an aspect correct
   // projection, you can do it here;  For now, we'll keep with the SD1
   // version that just sets a fixed ortho; 
   camera_buffer_t cpuData;
   cpuData.min = m_minOrtho; 
   cpuData.max = m_maxOrtho; 

   // copy the cpu to the gpu (will create or update the buffer)
   m_cameraUBO->CopyCPUToGPU( &cpuData, sizeof(cpuData) ); 
}
