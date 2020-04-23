```cpp

#include "ThirdParty/MikkT/mikktspace.h"

static int GetNumFaces( SMikkTSpaceContext const* context )
{
   // if you had index buffer
   std::vector<VertexLit>& vertices = *(std::vector<VertexLit>*)(context->m_pUserData);
   return vertices.size() / 3; 
}

//-----------------------------------------------------------------
static int GetNumberOfVerticesForFace( SMikkTSpaceContext const* pContext, const int iFace )
{
   return 3; 
}

//-----------------------------------------------------------------
static void GetPositionForFaceVert( const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert )
{
   std::vector<VertexLit>& vertices = *(std::vector<VertexLit>*)(context->m_pUserData);
   int indexInVertexArray = iFace * 3 + iVert; 

   // get our posistion
   Vec3 ourPos = vertices[indexInVertexArray].position; 

   fvPosOut[0] = outPos.x; 
   fvPosOut[1] = outPos.y; 
   fvPosOut[2] = outPos.z; 
}

//-----------------------------------------------------------------
static void GetNormalForFaceVert( const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert )
{
   // TODO: left to reader...
}

//-----------------------------------------------------------------
static void GetUVForFaceVert( const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert )
{
   // TODO: left to reader...
}

//-----------------------------------------------------------------
static void SetTangent( const SMikkTSpaceContext * pContext, 
   const float fvTangent[], 
   const float fSign, 
   const int iFace, const int iVert)
{
   std::vector<VertexLit>& vertices = *(std::vector<VertexLit>*)(context->m_pUserData);
   int indexInVertexArray = iFace * 3 + iVert; 
   

   vertices[indexInVertexArray].tangent = vec4( fvTangent[0], fvTangent[1], fvTangent[2], fSign ); 
}

   
//-----------------------------------------------------------------
// Assume I'm using only vertex array - not indexed array
void GenerateTangentsForVertexArray( std::vector<VertexLit>& vertices )
{
   SMikkTSpaceInterface interface; 

   // How does MikkT get info
   interface.m_getNumFaces = GetNumFaces; 
   interface.m_getNumVerticesOfFace = GetNumberOfVerticesForFace;

   interface.m_getPosition = GetPositionForFaceVert; 
   interface.m_getNormal = GetPositionForFaceVert; 
   interface.m_getTexCoord = GetPositionForFaceVert; 

   // MikkT telling US info
   interface.m_setTSpaceBasic = SetTangent; 
   interface.m_setTSpace = nullptr; 


   // Next, the context!  
   // Encapsulate ONE instance of running the algorithm
   SMikkTSpaceContext context; 
   context.m_pInterface = &interface; 
   context.m_pUserData = &vertices; 


   // RUN THE ALGO
   genTangSpaceDefault( &context ); 
}

