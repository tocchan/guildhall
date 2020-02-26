class Clock
{
   public:
      Clock();                   // defaults to being owned by master
      Clock( Clock* parent );    // passing nullptr will create a root clock (root clock is something you'd have to advance yourself)
      ~Clock();                  // be sure to correcty re-parent my children to my parent

      void Update( double deltaSeconds );    // usually do not need to call unless you create a new root clock
      void Reset();                       // set total time back to 0.0, does not reset children

      // Controls
      void Pause(); 
      void Resume(); 
      void SetScale( double scale ); 

      // accessors
      double GetTotalElapsedSeconds() const; 
      double GetLastDeltaSeconds() const; 

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
      // ...current time state (current time, delta, etc...)
      // ...track pause state
      // ...track scale
      // ...know my parent
      // ...know my children

   public: // accessor for the master clock of our engine
      static void SystemStartup();  // create/reset master clock
      static void SystemShutdown();
      static void BeginFrame();     // advance master clock (which immediately propogates to children)

      static Clock* GetMaster(); 
};




// Example Usage
void Game::Startup()
{
   m_game_clock = new Clock(); // automatically is master
}

void Game::Update()
{
   double dt = m_game_clock->GetLastDeltaSeconds(); 
   double totalTime = m_game_clock->GetTotalElapsedSeconds(); 
}

void RenderContext::Setup( Clock* game_clock )
{
   m_game_clock = game_clock; 
   if (m_game_clock == nullptr) {
      m_game_clock = Clock::GetMaster();
   }
}

void RenderContext::BeginFrame()
{
   // update frame ubo with with time
   time_data.system_time = master_clock->GetTotalElapsedSeconds();
   time_data.game_time = game_clock->GetTotalElapsedSeconds(); 
}

void DevConsole::Startup()
{
   m_clock = new Clock();
}

void DevConsole::Render()
{
   double seconds = m_clock->GetTotalElapsedSeconds();
   int iseconds = (int)seconds;
   if ((i % 2) == 0) {
      DrawCarrot();
   }
}

void DevCosole::OnInputChanged()
{
   m_clock->Reset(); 
}