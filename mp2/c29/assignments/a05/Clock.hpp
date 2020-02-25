class Clock
{
   public:
      Clock();                   // defaults to being owned by master
      Clock( Clock* parent );    // passing nullptr will create a root clock
      ~Clock();                  // be sure to correcty re-parent my children to my parent

      void Update( double deltaTime );    // usually do not need to call unless you create a new root clock
      void Reset();                       // set total time back to 0.0

      // Controls
      void Pause(); 
      void Resume(); 
      void SetScale( double scale ); 

      // accessors
      double GetTotalElapsedTime() const; 
      double GetFrameElapsedTime() const; 

      double GetScale() const; 
      bool IsPaused() const; 

      // OPTIONAL - but useful
      // void SetFrameLimits( double minFrameTime, double maxFrameTime ); // useful if you never want a simluation jumping too far (for exapmle, while stuck on a breakpoint)

   public:
      // used more internally
      void SetParent( Clock* clock ); 
      // void AddChild( Clock* clock );  // can implement both, or just one or the other can 


   public:
      // need a way to...
      // ...track pause state
      // ...track scale
      // ...know my parent
      // ...know my children

   public: // accessor for the master clock of our engine
      static Clock* GetMaster(); 
};

void ClockSystemStartup();    // create/reset master clock
void ClockSystemShutdown();   
void ClockSystemBeginFrame(); // update master clock