// Matrix Needs
// Translate
// RotationAroundX
// RotationAroundY
// RotationAroundZ
// RotationEuler( vec3, eRotationOrder ); 

enum eRotationOrder
{
   ROTATION_ORDER_XYZ,     // Unreal/Squirrel Defualt
   ROTATION_ORDER_ZXY,     // Unity/Forseth Defualt

   ROTATION_ORDER_DEFAULT = ROTATION_ZXY; 
}; 

// Transpose
// Invert?
