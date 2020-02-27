//------------------------------------------------------------------------
vec3 Camera::ClientToWorld( vec2 client, float ndcZ = 0 )
{
   // map client to ndc
   vec3 ndc = RangeMap( Vec3(client, ndcZ), 
      Vec3(0,0,0), Vec3(1,1,1), 
      Vec3(-1,-1,0), Vec3(1,1,1) ); 

   // get this to world
   // view        : world -> camera
   // projection  : camera -> clip  (homogeneous ndc)
   // clip -> world
   Mat44 proj = GetProjectionMatrix();
   Mat44 worldToClip = proj;
   worldToClip.PushMatrix( GetViewMatrix() );  

   Mat44 clipToWorld = Invert( worldToClip ); 
   Vec4 worldHomogeneous = clipToWorld.Transform( Vec4( ndc, 1 ) ); 
   Vec3 worldPos = worldHomogenous.XYZ() / worldHomogeneous.w; 

   reutrn worldPos; 
}

//------------------------------------------------------------------------
vec2 Camera::GetOrthoMin()
{
   return ClientToWorld( Vec2(0,0), 0).XY(); 
}

//------------------------------------------------------------------------
vec2 Camera::GetOrthoMax()
{
   return ClientToWorld( Vec2(1,1), 0).XY(); 
}

//------------------------------------------------------------------------
// Inferring Aspect to Set Projections
//------------------------------------------------------------------------
//------------------------------------------------------------------------
vec2 Camera::SetOrthographicProjection( float height, float nz, float fz )
{
   float aspect = GetOutputSize(); // my colorTarget size
   Vec2 extents = Vec2( aspect * height * 0.5f, height * 0.5f ); 
   m_projection = MakeOrthographicProjection( -extents, extents, nz, fz ); 
}

//------------------------------------------------------------------------
vec2 Camera::SetPerspectiveProjection( float fov, float nz, float fz )
{
   float aspect = GetColorTargetSize(); 
   m_projection = MakeProjectionMatrix( fov, aspect, nz, fz ); 
}

//------------------------------------------------------------------------
float Camera::GetOutputAspectRatio()
{
   return GetColorTargetSize().GetRatio(); 
}

//------------------------------------------------------------------------
Vec2 Camera::GetColorTargetSize() const // is needed for depth buffers
{
   if (m_colorTarget != nullptr) {
      return m_colorTarget.GetSize(); 
   } else {
      m_renderContext->GetBackbuffer().GetSize(); 
   }
}

