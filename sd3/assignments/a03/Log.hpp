struct log_message_t 
{
   char const* filter; 
   char const* message; 
   callstack_t* callstack; // callstack if there was one
}; 

// NOTE: As with ConsolePrintf, you may use variadic templates instead if you prefer
void LogSystemInit( char const* log_file ); 
void LogSystemShutdown(); // will do a final flush of the log file and properly close donw

// commands exposed; 
// log
// log_enable_all
// log_disable_all
// log_enable
// log_disable
// log_flush_test
// log_thread_test
// log_hook_dev_console 

// Logs a message with the given filter
void Logf( char const* filter, char const* format, ... ); 

// Logs a message and includes the callstack on the next line(s)
void LogCallstackf( char const* filter, char const* format, ... ); 

// confirms all messages have been processed (committed to disk)
void LogFlush(); 

// Filtering
void LogEnableAll();  // all messages log
void LogDisableAll(); // not messages log
void LogEnable( char const* filter );     // this filter will start to eappear in the log
void LogDisable( char const* filter );    // this filter will no longer appear in the log

// Listener Hooks
// Allow other systems to get message events (from the logger thread)
// so it can do somethign with them
// Example would be to call DebuggerPrintf - or to enable the dev console to print them; 

// NOTE - this message is only valid for the lifetime of the callback
// and is not guaranteed to valid after your hook returns, so be sure to 
// copy data you need!
typedef std::function<void(log_message_t)> log_hook_cb; 
void LogHook( log_hook_cb cb ); 
void LogUnhook( log_hook_cb cb ); 


