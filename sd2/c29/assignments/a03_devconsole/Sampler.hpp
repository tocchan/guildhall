
enum eSamplerMode 
{
   SAMPLE_MODE_POINT, 
   SAMPLE_MODE_LINEAR, 
   // SAMPLE_MODE_TRILINEAR - when we do mip maps;
}; 

class Sampler
{
   public:
      Sampler( RenderContext* context, eSamplerMode mode ); 
      ~Sampler(); 

   public:
      RenderContext* m_owner; 
      ID3D11Sampler* m_handle; 
}; 