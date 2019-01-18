#pragma once


#include <stdarg.h>


//------------------------------------------------------------------------
//------------------------------------------------------------------------
void Log( char const *str );
void Logv( char const *format, va_list args );
void Logf( char const *format, ... );
