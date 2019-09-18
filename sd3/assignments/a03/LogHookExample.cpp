void Logf( char const* filter, char const* message, ... ) 
{

	// vsnprintf(); 
	size_t logobj_size = sizeof(LogObject_T); 
	size_t msg_len = vsnprintf( nullptr, 0, message, args ) + 1U; // for null terminator
	size_t filter_len = strlen( filter ) + 1U; 

	total_size = logobj_size + msg_len + filter_len; 
	byte* buffer = gRingbuffer->LockWrite( total_size ); 	

	 LogObject_T* log = (LogObject_T*)buffer; 
	 log->message = buffer + logobj_size; 
	 log->filter = buffer + logobj_size + msg_len; 

	 vsnprintf( log->message, msg_len, message, args )
	 memcpy( log->filter, filter, filter_len );  

	 gRingbuffer->UnlockWrite( log ); 
	 Signal(); 
}

void DevConsolePrintLog( log_message_t* msg )
{
	DevConsolePritnf( "%s: %s", msg->filter, msg->message ); 
}


std::vector<log_callback_t> m_callbacks; 
std::shared_mutex m_callbackLock; 

void Startup() 
{
	LogSystemInit( "log.txt" ); 

	// other stuff happens

	LogHook( DevConsolePrintLog ); 
	LogHook( SendLogOverNetwork ); 
}