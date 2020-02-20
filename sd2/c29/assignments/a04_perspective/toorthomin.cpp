vec3 Camera::GetOrthoMin() // technically this is GetVisibleWorldMin()
{
   vec4 ndc( -1, -1, 0, 1 ); // bottom left of renderable space
   mat44 view_projection = projection * view;  // world to renderable space
   mat44 clip_to_world = view_projection.GetInvert(); // renderable space to world

   vec4 world = clip_to_world * ndc;   // convert our bottom left corner
   world /= world.w;                   // find the representitive point (w == 1)
   return world.xyz();                 // return the world point
}