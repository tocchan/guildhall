#include <stdio.h>

void* FileReadToNewBuffer( std::string const& filename, size_t *out_size = nullptr )
{
	FILE* fp = nullptr;
	fopen_s( &fp, filename.c_str(), "r" );
	if (fp == nullptr) {
		return nullptr;  
	}

	// get the size of the file
	fseek(fp, 0, SEEK_END); 
	long file_size = ftell(fp); 

	unsigned char* buffer = new unsigned char[file_size + 1]; 
	if (nullptr != buffer) {
		fseek( fp, 0, SEEK_SET ); 
		size_t bytes_read = fread( buffer, 1, (size_t)file_size, fp ); 
		buffer[bytes_read] = NULL; 

		if (out_size != nullptr) {
			*out_size = (size_t)bytes_read; 
		}
	}

	fclose(fp); 
	return buffer; 
}