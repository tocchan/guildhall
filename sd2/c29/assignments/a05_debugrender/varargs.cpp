
std::string Stringv( char const* format, va_list args )
{
   char buffer[1024]; 
   vsnprintf_s( buffer, 1024, format, args ); 
   return buffer; 
}

std::string Stringf( char const* format, ... ) 
{
   va_list args; 
   va_start(args, count); 
   std::string s = Stringv( format, args );
   va_end(args); 
   return s; 
}


void DebugAddWorldTextf( mat44 basis, vec2 pivot, rgba color, float duration, eDebugRenderMode mode, char const* format, ... )
{
   va_list args; 
   va_start(args, format); 
   std::string str = Stringv( format, args ); 

   // do other stuff for debug rendering
   // ..
}

