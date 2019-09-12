```cpp

//------------------------------------------------------------------------
static void LogThread()
{
   // Create log file (logs/log.txt) (write mode)
   // TODO extra: give log file a timestamp
   // I prefer "log.YYYYMMDD_HHmmss.txt"
   // which allows you to sort alphabetically; 
   // ...

   while (log system running) {
      log_message_t* msg; 
      while (read-lock the next message) {
         ProcessLogMessage( msg );
         read-unlock the message;  // frees memory to be resused in the ring-buffer
      // ...

      // wait on semaphore or condition (sleeps thread till more work is ready)
      // ...
   }

   // close log file if open...
   // ...
}

//------------------------------------------------------------------------
static void ProcessLogMessage( log_message_t* msg ) 
{
   // Order of these two don't matter...

   // give every hook a shot at it...
   // ...

   // write to the log file...
   // ...
}

//------------------------------------------------------------------------
bool LogSystemInit( char const* log_file )
{
   // TODO extra:  Find log files in log_file folder, 
   // and delete anything but the LOG_HISTORY_COUNT newest

   // setup the ring buffer the system will use; 

   // Start log thread; 
}

//------------------------------------------------------------------------
void Logf( char const* tag, char const* format, ... )
{
   // do not work if the message is currently filtered; 
   // you could alternatively do this on the thread if you're
   // allowing for seperate filters for console/files/etc...
   if (IsFiltered(tag)) {
      return; 
   }

   // lock enough space to write log information; 
   // look into `vsnprintf` for determining buffer size needs
   // for the format/arguments...
   size_t bytes_needed = 0; 
   // need space for the log_message_t, the format and tag; 
   // for callstack version - also enough room for a callstack
   // ...

   // allocate it using a thread-safe ring-buffer
   byte* buf = (byte*) gMessageBuffer->lock_write( bytes_needed ); 
   // sub allocate and finalize the message
   // ...

   // get callstack, allocated into buf
   // ...

   gMessageBuffer->unlock_write( buf ); // mark us as finished writing - so worker thread can pick it up
}

//------------------------------------------------------------------------
void LogFlush()
{
   // request a flush
   gFlushRequested = true; 

   // signal smeaphore and wait until the flush has been serviced
   // Be sure to flush the file operations using fflush or similar
   // on the thread... once done, mark the flush as finished
   // ...

   // wait until the thread marks this back to false when it is done
   // (not perfect, but good enough for a debug tool)
   while (!gFlushRequested) {
      ThreadYield(); 
   }

   // we can get a lot fancier here if you want practice
   // One option is to pass a local bool as a message to the log thread, 
   // allowing each flush request to be serviced explicitly...

   // you can also pass or use a semaphore for signaling it is finished so we can wait on it.  
   // But again, for a debug tool, just spinning will be fine...
}

//------------------------------------------------------------------------
// filtering is done using a bool and a container of filters
// bool isWhitelist;
// container<string> filters; 
//
// Filters use a "reader-writer lock", also referred to as a "shared_mutex" in C++11+; 

// Enabling or Disabling All changes the mode you're in, effecting the behaviour of
static bool IsFiltered( char const* tag ) 
{
   // shared lock (not editing)
   // depending on mode and if it exists will tell us if we're filter
   // Filtered messages 
}

void LogEnableAll()
{
   // Enabling all is setting yourself to a "blacklist" mode.
   // meaning filters you're tracking are targs you want to ignore
   // ...
}

void LogDisableAll()
{
   // Disable All sets yourself to a "whitelist" mode, meaning your 
   // tracked filters are now the tags you allow through; 
   // ...
}

// LogEnable & LogDisable behaviour depends on which mode we're currently in...
```