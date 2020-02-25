class Timer
{
   public:
      void SetSeconds( Clock* clock, double timeToWait );    // sets and resets timer, and clock
      void SetSeconds( double timeToWait );                  // keeps current clock, or sets to Master if no clock has been set yet

      void Reset();                                   // reset the timer, but it keeps running

      void Stop();                                    // stops the timer

      double GetElapsedSeconds() const;                  // return amoutn of time accrued on this timer
      double GetSecondsRemaining() const;                // returns amount of time until HasElapsed() will return true (0 or negative numbers means it has already elapsed)

      bool HasElapsed() const;                        // timer has elapsed the timer
      bool CheckAndDecrement();                       // if has elapsed, removes one interval of time and returns true, otherwise returns false
      bool CheckAndReset();                           // if has elapsed, resets and returns true, otherwise returns false; 

      bool IsRunning() const;                         // timer is accruing time


      // OPTIONAL - useful on a timer, but not needed for anything we're doing.

      // Pause and Resume require some extra bookkeeping, but can be usefaul
      // void Pause(); 
      // void Resume(); 

      // an optional way to set the timer if you want to think more in terms of times-per-seconds instead of seconds
      // void SetFrequency( double hz );  



   public:
      Clock* m_clock; // clock timer is based off of

      // other data needed...
      // ...a way to track an interval duration
      // ...a way to track when the timer has elapsed (hopefully can give us remaining time and elapsed time)
}; 