// In D3D11, NDC clip space is defined as only 0 to 1 on the z, as opposed to -1 to 1 
// that OpenGL does.  So I have implementations for both;
#define NDC_Z_ZERO_TO_ONE

// These matrices are assuming a "Z-Forward" basis, that is
