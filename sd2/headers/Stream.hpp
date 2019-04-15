enum eEndianness 
{
   LITTLE_ENDIAN,
   BIG_ENDIAN
};
eEndianness GetLocalEndianness(); 


class Stream 
{  
   public:
      Stream( eEndianness streamEndianness ); 
      virtual ~Stream();

      // @return - number of bytes written; 
      virtual size_t WriteBytes( void const *data, size_t bytesToWrite ); 
      virtual size_t ReadBytes( void *out, size_t bytesToRead ); 

      virtual size_t GetReadableByteCount() const = 0U;  
      virtual size_t GetWritableByteCount() const = 0U;  // for files, feel free to always return a very large number

      virtual void Flush() = 0U; // guarantees writes (useful for network or file streams)

      // Helper - converts bytes to correct endianess before calling WriteBytes
      size_t WriteEndianAwareBytes( void const *data, size_t bytesToWrite ); 
      size_t ReadEndianAwareBytes( void *out, size_t bytesToRead ); 

      // post setup (or switching halfway through a read/write (if say, a datablock is encoded data in another format)
      void SetEndianness( eEndianness byteOrder ); 
      eEndianness GetEndianness() const; 

   public: // other helpers for common data types, or templated helpers
}; 

// Useful to  have a templated ToStream and FromStream, but I hate
// defaulting it since stream writing is inherently not endian aware except for 
// basic types - so leaving this blank (an error will occur if someone tries to 
// use them for a type that didn't specialize)
template <typename T>
bool ToStream( Stream *out, T const &data ); 

template <typename T>
T FromStream( Stream *in, T const &defValue );  