class RenderContext
{

   public: 
      // data to store lighting
      light_buffer_t m_light_data;     // stores all lighting information (mirrors uniform buffer)
      UniformBuffer *m_light_buffer;   // constant buffer (bound to slot 4)
      bool m_is_light_data_dirty;      // used during draw to know if I should update the light buffer
};