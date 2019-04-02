
//------------------------------------------------------------------------
//------------------------------------------------------------------------
class Clock
{  
   public:
      Clock() 
      {
         SetParent( &Master ); 
      }

      Clock( Clock *parent ) 
      {
         SetParent( parent ); 
      }

      ~Clock()
      {
         // two options;

         // option 1 - children should die before me;  Someone didn't clean up; 
         ASSERT( m_children.size() == 0 ); // other people should have cleaned up 
         SetParent(nullptr); 
        
         // option 2 - orphan and adopt
         /*
         if (m_parent != nullptr) {
            while (m_children.size() > 0) {
               m_children[0]->SetParent( m_parent ); 
            }
         }
         */
      }

      void step( double dt )
      {
         dt *= m_dilation; 
         if (IsPaused()) {
            dt = 0.0; 
         }

         dt = Clamp( dt, 0.0f, m_frameLimit ); 

         ++m_frameCount; 
         m_totalTime += dt; 
         m_frameTime = dt; 

         for (Clock *clockIter : m_children) {
            clockIter->step( dt ); 
         }
      }

      void SetFrameLimit( double time ); 
      void Dilate( double dilation ); 

      void Pause(); 
      void Resume(); 
      void ForcePause();
      void ForceResume(); 

      void SetParent( Parent *newParent ) 
      {
         // I have parent...
         if (m_parent != nullptr) {
            m_parent.m_children.remove(this); 
            m_parent = nullptr; 
         }

         m_parent = newParent; 
         if (m_parent != nullptr) {
            m_parent.m_children.push_back(this); 
         }
      }


   public:
      uint64_t m_frameCount   = 0; 
      double m_totalTime      = 0.0; 
      double m_frameTime      = 0.0; 

      double m_frameLimit     = INFINITY;  
      double m_dilation       = 1.0f; 
      int m_pauseCount        = 0U; 

      Clock *m_parent         = nullptr; 
      std::vector<Clock*> m_children; 

   public:
      static Clock Master = Clock(nullptr); 
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void ClockSystemStartup()
{
   // be sure to setup gLastFrameTime;
}

//------------------------------------------------------------------------
void ClockSystemBeginFrame()
{
   double cur_time = GetCurrentTimeSeconds();
   double delta_time = cur_time - gLastFrameTime; 
   Clock::Master.step(delta_time); 
}

