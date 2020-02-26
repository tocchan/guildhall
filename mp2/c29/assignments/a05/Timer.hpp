class Timer
{
   public:
      void SetSeconds( Clock* clock, double timeToWait );    // sets and resets timer, and clock
      void SetSeconds( double timeToWait );                  // keeps current clock, or sets to Master if no clock has been set yet

      void Reset();                                   // reset the timer, but it keeps running

      void Stop();                                    // stops the timer

      double GetElapsedSeconds() const;               // return amount of time accrued on this timer
      double GetSecondsRemaining() const;             // returns amount of time until HasElapsed() will return true (0 or negative numbers means it has already elapsed)

      bool HasElapsed() const;                        // timer has elapsed the timer
      bool CheckAndDecrement();                       // if has elapsed, removes one interval of time and returns true, otherwise returns false
      int  CheckAndDecrementAll();                    // returns how many intervals have passed and removes them  
      bool CheckAndReset();                           // if has elapsed, resets and returns true, otherwise returns false; 

      bool IsRunning() const;                         // timer is accruing time


      // OPTIONAL - useful on a timer, but not needed for anything we're doing.

      // Pause and Resume require some extra bookkeeping, but can be useful
      // void Pause(); 
      // void Resume(); 

      // an optional way to set the timer if you want to think more in terms of times-per-seconds instead of seconds
      // void SetFrequency( double hz ) { SetSeconds( 1.0 / hz ); }



   public:
      Clock* m_clock; // clock timer is based off of

      // other data needed...
      double m_startSeconds    =  0.0; 
      double m_durationSeconds = -1.0;    // negative means stopped

      // ...a way to track an interval duration
      // ...a way to track when the timer has elapsed (hopefully can give us remaining time and elapsed time)
}; 


// Impl Notes
bool Timer::Set( Clock* clock, double timeSeconds )
{
   m_clock = clock; 
   m_startSeconds = clock->GetTotalSeconds(); 
   m_durationSeconds = timeSeconds; 
}

bool Timer::HasElapsed()
{
   double currentTime = m_clock->GetTotalSeconds(); 
   return (currentTime > (m_startSeconds + m_durationSeconds)); 
}

bool Timer::CheckAndDecrement()
{
   if (HasElapsed()) {
      // remove an interval of time
      m_startSeconds += m_durationSeconds; 
      return true; 
   } else {
      return false; 
   }
}

bool Timer::CheckAndReset()
{
   if (HasElapsed()) {
      // remove an interval of time
      m_startSeconds = m_clock->GetTotalSeconds(); 
      return true; 
   } else {
      return false; 
   }
}


// Example - Without Timer
void Physics2D::SetClock( Clock* clock )
{
   m_clock = clock; 
}

void Physics2D::Update()
{
   // fixed delta time (120hz), or a single frame is (1.0 / 120.0) seconds

   double dt = m_clock->GetLastDeltaSeconds(); 
   double totalTime = dt + m_accumatedTime; 

   double steps = totalTime / m_fixedDeltaTime; 
   int stepCount = (int)steps; 
   m_accumulatedTime = (steps - (double)stepCount) * m_fixedDeltaTime; 

   // do this as many times as I need to fill actual delta time
   for (int i = 0; i < i < stepCount; ++i) {
      AdvanceSimulation( m_fixedDeltaTime ); 
   }
}

// WITH TIMER
void Physics2D::Startup()
{
   m_stepTimer.Set( m_clock, m_fixedDeltaTime ); 
}

void Physics2D::SetClock( Clock* clock )
{
   m_clock = clock; 
}

void Physics2D::Update()
{
   // fixed delta time (120hz), or a single frame is (1.0 / 120.0) seconds

   // first check of while
   // startTime = 4.0 seconds
   // currentTime = 4.43 seconds
   // fixedDelta = 0.1

   // => endTime = 4.1

   // after check
   // startTime = 4.1
   // endTime = 4.2   -> true

   // startTime = 4.2
   // endTime = 4.3    -> true

   // startTime = 4.3
   // endTime = 4.4     -> true

   // stsartTime = 4.4
   // endTime = 4.5     -> fails,  startTime stays 4.4


   while (m_stepTimer.CheckAndDecrement()) {
      AdvanceSimulation( m_fixedDeltaTime ); 
   }


   // next frame
   // currentTime = 4.62 
   // startTime = 4.4
   // endTime = 4.5
   // 4.62 >= 4.5    -> true

   // -> 4.6  -> true

   // -> 4.7   // false, stop
}


