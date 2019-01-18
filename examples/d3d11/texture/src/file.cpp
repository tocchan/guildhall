#pragma once

#include "file.h"
#include "log.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <malloc.h>


//------------------------------------------------------------------------
// Helper - File IO Example - Read an entire file to a buffer
// NOTE:  Be sure for free it when you're done.
//------------------------------------------------------------------------
void* FileReadToBuffer( char const *filename, size_t *out_size )
{
   *out_size = 0U;

   // I'm going to use Windows file functions for no particular reason
   // outside of familiarity.  Feel free to use whatever. 
   DWORD file_access = GENERIC_READ;
   DWORD share = FILE_SHARE_READ;
   DWORD options = OPEN_EXISTING;

   HANDLE file_handle = CreateFileA( filename, file_access, share, NULL, options, FILE_ATTRIBUTE_NORMAL, NULL );
   if (file_handle == INVALID_HANDLE_VALUE) {
      Logf( "File [%s] could not be opened.", filename );
      return nullptr;
   }
   
   // Get File Size
   DWORD high_word;
   DWORD lo_word = ::GetFileSize( file_handle, &high_word );
   
   // 64-bit way - use a 64-bit type, but we will never have a 4GB+ file, so ignore it and just using lo-word to prevent warnings
   // size_t size = ((size_t)high_word << 32) | (size_t)lo_word;
   size_t size = (size_t)lo_word;

   // Make a buffer that big
   void *buffer = ::malloc(size + 1U); // I allocae one extra for a null terminator - but do not include it in the size
                                     // just in the cases I load strings it null terminates.
   if (nullptr != buffer) {
      DWORD read = 0U;
      ReadFile( file_handle, buffer, (DWORD)size, &read, nullptr );
      *out_size = read;

      ((char*)buffer)[size] = NULL;
   }

   CloseHandle( (HANDLE)file_handle );
   return buffer;
}