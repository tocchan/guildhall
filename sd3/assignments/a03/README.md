SD3.a03 : Log System and Allocators
======

## Grading
[ ] Log system can be initialized with a given log file name. 
    [ ] All log functions do nothing if `LogSystem` is not initialized, but this is not an error. 
[ ] All logging happens on a dedicated logging thread.
[ ] Log thread test works without error/crashing
[ ] Log filtering works (see test below)
[ ] Log flushing works (see test below)
[ ] Log system can be subscribed to - shown using the dev console; 

[ ] `AsyncRingBuffer` implemented
    [ ] Used to pass information to the logging thread
    [ ] If your `DevConsole` isn't already thread safe - can be used to remedy that; 

### Extra
[ ] Keep a history of log files around (attach a timestamp to them)
[ ] Keep only N most recent log files; 
[ ] Support a seperate filter for the dev console -vs- file  
[ ] Support filter colors (useful for dev conssole) 
    [ ] Output logs using HTML (allows coloring, tables, etc)
    [ ] HTML file includes a way to live filter (javascript is your friend)


## Notes
...

## Grading & Tests

### Log Thraed Test

This test is both to hit it from multiple threads to make sure it doesn't break, as well as make sure
that logging does not slow down your simulation noticably.  

```cpp
#define LOG_MESSAGES_PER_THREAD_TEST   (1024)

static void LogTest() 
{
   std::thread::id this_id = std::this_thread::get_id(); 
   size_t hash_id = std::hash<std::thread::id>{}( this_id );
   char const* format = "Thread[%llu]: Printing Message %u"; 
     
   for (uint i = 0; i < LOG_MESSAGES_PER_THREAD_TEST; ++i) {
      if (RandomCoinFlip()) {
         LogCallstackf( "debug", format, hash_id, i ); 
      } else {
         Logf( "debug", format, hash_id, i ); 
      }
   }
}

COMMAND(LogThreadTest, "Throw a bunch of threads at the sytem; ")
{
   // leave one thread free (main thread)
   uint core_count = std::thread::hardware_concurrency() - 1; 
   for (uint i = 0; i < core_count; ++i) {
      std::thread test_thread( LogTest ); 
      test_thread.detach(); 
   }
}
```

### Filters and Flushes
These commands should always be available with the `Log` system; 

Test I will be running;

1. Filter Test
   1. Enable all filters
   2. Log a message with filter: `Log cf "Hello World"`
   3. Disable all filters
   4. Log a message with filter: `Log cf "Filtered Test"`
   5. Enable just the filter `test`
   6. Log a message with filter `log cf "This message should not show up"`
   7. Log a message with filter `log test "This message should show up"`
   8. Enable all filters again and disable `test`
   9. Log a message with filter `log cf "This message should now show up"`
   10. Log a message with filter `log test "But this one should not"`

2. Flush Test
   1. Put a break point in `log_flush_test` at the line after `LogFlush`
   2. Run `log_flush_test`
   3. When breakpoint is hit, open file and make sure flush text is there; 

```cpp
COMMAND(log_enable_all) 
{
   // enable all filters
   LogEnableAll(); 
}

COMMAND(log_disable_all) 
{
   // 
   LogDiableAll(); 
}

COMMAND(log_enable) 
{
   // TODO - enable a single filter
}

COMMAND(log_disable) 
{
   // TODO - disable a single filter
}

COMMAND(log, "Log a message with a filter") 
{
   // however you get arguments off yoru command line
   char const* filter = args.get_next_string(); 
   char const* message = args.get_next_string(); 

   // proper error checking
   // TODO

   // Log the message
   Logf( filter, message ); 
}

COMMAND(log_flush_test, "Log and flush test") 
{
   Logf( "debug", "This message was logged." ); 
   LogFlush(); 

   // put breakpoint here, open log file and make sure above line was written; 
   int i = 0; 
   i = i; 
}

```

### Hook Test
Finally, it is useful to have the log show up in multiple places, most notably your in-game command console.

You may or may not want this as default behaviour... either way, have a way to change the behaviour at run-time; 


**Note**: the hooked function runs on the loggers thread, and needs to copy the data 
to the hooked system in a thread safe fashion.  An `AsyncQueue` or `RingBuffer` work well for this; 

```cpp
COMMAND(log_hook_dev_console)
{
   bool hook = args.get_next_bool(); 

   if (hook) {
      // TODO: Hook the dev console
   } else {
      // TODO: Unhook the dev console
   }

}
```