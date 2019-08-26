#pragma once

#define MAX_TRACE 64

class Callstack 
{
	public:
		void* m_trace[MAX_TRACE];	// execution pointers representing where we are in code; 
		uint m_depth = 0;			// how full is the above array
};

// skip frames is the number of frames from where we are to skip (ie, ignore)
Callstack CallstackGet( uint skip_frames = 0 ); 

// Convert a callstack to strings
// with one string per line
// Strings should return in this format...
// filepath.cpp(line,offset): function_name
// for example
// Engine/Image.cpp(127,8): Image::LoadFromFile
std::vector<std::string> CallstackToString( Callstack const& cs ); 