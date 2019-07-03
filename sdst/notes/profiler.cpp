/**
 Intended behaviour;

void Foo() 
{ 
   PROFILE_LOG_SCOPE("Frame"); 

   UpdateInput();
   UpdateSim();
   {
      PROFILE_LOG_SCOPE("Physics Correction"); 
      Correct();
   }

   Render(); 

   EndFrame(); 
}

void Foo2()
{
   uint64_t hpc_start = GetCurrentHPC(); 
   Render();
   uint64_t duration = GetCurrentHPC() - hpc_start; 
   DebuggerPrintf( "Render took %llu hpc", duration ); 
}



// Output
Frame took 8.3ms (12084 hpc)
Physics Corection took 4.2ms (6301 hpc)
**/

class LogProfileScope
{
   LogProfileScope( char const *name )
   {
      m_name = name; 
      m_start_time = GetCurrentHPC(); 
   }

   ~LogProfileScope()
   {
      uint64_t duration = GetCurrentHPC() - m_start_time;
      DebuggerPrintf( "%s took %s (%llu hpc)", m_name, GetProfileTimeString(duration), duration ); 
   }

   uint64_t m_start_time; 
   char const* m_name;
};

// PROFILE_LOG_SCOPE???

// Helpful Function to make values more human readable; 
// if time is < 1ms, display in micro seconds, ex 0.0000124283 -> "12.428us"
// if time is < 1s, display in ms (with 3 values after decimal), ex: 0.23434315 -> "234.343ms"
// otherwise, display in seconds (with 3 values after decimal) -> ex 120482.294719 -> "120482.295 s"
std::string GetProfileTimeString( double seconds );

#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)

#define PROFILE_LOG_SCOPE( tag )  LogProfileScope COMBINE(__scopeLog, __LINE__) ## ( tag )

void Foo()
{
   // LogProfileScope name( "Check" );  // Check took 0.000us (2 hpc)
   PROFILE_LOG_SCOPE( "Check" ); 
}