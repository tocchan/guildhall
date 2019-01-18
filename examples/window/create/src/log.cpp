#include "log.h"
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#define FIXED_PRINT_BUFFER_SIZE (512)

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void Log( char const *str ) 
{
   OutputDebugStringA( str );
   OutputDebugStringA( "\n" );
}

//------------------------------------------------------------------------
void Logv( char const *format, va_list args ) 
{
   size_t size_needed = _vscprintf( format, args ) + 1U;

   char fixed_buffer[FIXED_PRINT_BUFFER_SIZE];
   char *buffer = fixed_buffer;

   if (size_needed >= FIXED_PRINT_BUFFER_SIZE) {
      buffer = new char[size_needed];
   }

   vsprintf_s( buffer, size_needed, format, args );
   Log( buffer );

   if (buffer != fixed_buffer) {
      delete[] buffer;
   }
}

//------------------------------------------------------------------------
void Logf( char const *format, ... ) 
{
   va_list args;
   va_start( args, format );

   Logv( format, args );

   va_end(args);
}