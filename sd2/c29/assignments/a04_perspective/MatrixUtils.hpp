// A03
Mat44 MakeOrthographicProjectionMatrixD3D( 
	float minX, float maxX, 
	float minY, float maxY, 
	float minZ, float maxZ ); 

// A04
Mat44 MakePerpsectiveProjectionMatrixD3D(
	float fovDegrees,
	float aspectRatio,  
	float nearZ, 
	float farZ ); 


// More Utility
void MatrixTranspose( Mat44& mat );          // A04
void MatrixInvertOrthoNormal( Mat44& mat );  // A04
void MatrixInvert( Mat44& mat );             // A04   
