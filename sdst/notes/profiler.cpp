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

// Output
Frame took 8.3ms (12084 hpc)
Physics Corection took 4.2ms (6301 hpc)
**/

class LogProfileScope
{

};

// PROFILE_LOG_SCOPE???

// Helpful Function to make values more human readable; 
// if time is < 1ms, display in micro seconds, ex 0.0000124283 -> "12.428us"
// if time is < 1s, display in ms (with 3 values after decimal), ex: 0.23434315 -> "234.343ms"
// otherwise, display in seconds (with 3 values after decimal) -> ex 120482.294719 -> "120482.295 s"
std::string GetProfileTimeString( double seconds );

