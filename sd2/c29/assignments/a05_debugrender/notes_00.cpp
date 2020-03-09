// static variables up top
static RenderContext* gDRContext = nullptr; 
static Camera* gDebugCamera = nullptr; 

class DebugRenderSystem
{
   RenderContext* m_context;
   Camera* m_camera; 
   // ... whatever you need
};

static DebugRenderSystem* gDebugRenderSystem = nullptr; 

class DebugRenderObject
{
   public:
      bool IsReadyToBeCulled() const; // 

}


void DebugRenderScreenTo( Texture* tex ) 
{
   RenderContext* ctx = tex->GetRenderContext();
   gDRContext = ctx; 

   Camera camera; 
   camera->SetColorTarget( tex ); 
   vec2 min = vec2::ZERO; 
   vec2 max = tex->GetDimensions(); 
   camera->SetProjectionOrthographic( min, max, -1.0f, 1.0f ); 

   // should I clear?
   camera->SetClearMode( CLEAR_NONE ); 


   std::vector<VertexPCU> vertices; 

   gDRContext->BeginCamera( camera ); 

   foreach (object) {
      AppendObjectToVertexArray( vertices, indices, obj ); 
      if (obj->IsOld()) {
         obj->MarkForGarbage();
      }
   }

   gDRContext->DrawVertexArray( vertices );  

   gDRContext->EndCamera(); 
}


void DebugAddScreenPoint( vec2 pos, float size, rgba start_color, rgba end_color, float duration )
{
   // Make an object
   SomeObjectType* obj = CreateNewObjectForThisScreenPoint( pos, size, start_color, end_color, duration ); 
   AppendObjectToAList( obj ); 
}


void DebugRenderEndFrame()
{
   // foreach object, if ready to culled, remove and delete
}
