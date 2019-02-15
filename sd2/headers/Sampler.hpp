//------------------------------------------------------------------------
// Sampler.hpp
//------------------------------------------------------------------------



class Sampler                                         // A03
{
   public:
      Sampler();                                      // A03
      ~Sampler();                                     // A03

      void SetFilterModes( eFilterMode min, eFilterMode mag );    // A03

   public: // optionally, private as friend
      void CreateStateIfDirty( RenderContext *ctx );  // A03 
      ID3D11SamplerState* GetHandle() const;          // A03

   public:
      ID3D11SamplerState *m_handle  = nullptr; 

      // state we care about
      eFilterMode m_minFilter       = FILTER_MODE_POINT;    // A03
      eFilterMode m_magFilter       = FILTER_MODE_POINT;    // A03
      eFilterMode m_mipFilter       = FILTER_MODE_POINT;    // ???

      // ...wrap
      eWrapMode m_uWrap             = WRAP_MODE_LOOP;       // ???
      eWrapMode m_vWrap             = WRAP_MODE_LOOP;       // ???
      eWrapMode m_wWrap             = WRAP_MODE_LOOP;       // ???

      // mip levels and bias
      uint m_minMip                 = 0;                    // ???
      uint m_maxMip                 = MAX_UINT;             // ???
      float m_mipBias               = 0.0f;                 // ???

      // Compare samplers
      // ...
       
       // marked when state has changed so we know to recreate; 
      bool m_isDirty                = true;                 // A03
}; 
