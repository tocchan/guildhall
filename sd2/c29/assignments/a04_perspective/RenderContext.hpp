enum eCompareFunc
{
   COMPARE_FUNC_NEVER,           // D3D11_COMPARISON_NEVER
   COMPARE_FUNC_ALWAYS,          // D3D11_COMPARISON_ALWAYS
   COMPARE_FUNC_LEQUAL,          // D3D11_COMPARISON_LESS_EQUAL
   COMPARE_FUNC_GEQUAL,          // D3D11_COMPARISON_GREATER_EQUAL
};


// RenderContext encompasses
// the device and immediate context of D3D11
// controlling drawing
class RenderContext
{
   public:
      RenderContext();                          // A01
      ~RenderContext();                         // A01

      void Startup( Window* defaultWindow );    // A01
      void Shutdown();                          // A01

      void BeginFrame();                        // A01
      void EndFrame();                          // A01

      Texture* GetFrameColorTarget();           // A01 // GetDefaultColorTarget

      void ClearColor( Texture* colorTarget, RGBA color );        // A01
      void ClearDepth( Texture* depthTarget, float depth );       // A04

      void BeginCamera( Camera* camera );       // A01, A02, A03
      void EndCamera();                         // A01

      // Binding Inputs
      void BindVertexBuffer( VertexBuffer* vbo );  // A02

      // Binding State
      void BindShader( Shader* shader );        // A02
      void BindShader( char const* filename );  // A02

      void BindTexture( uint slot, Texture* tex );     // A03
      void BindSampler( uint slot, Sampler* sampler ); // A03

      void BindConstantBuffer( uint slot, RenderBuffer* cbo ); // A03

      void SetBlendMode( eBlendMode mode );     // A03

      void SetDepthTest( eCompareFunc compare, bool writeDepthOnPass ); // A04

      // Resource Creation
      Shader* GetOrCreateShader( char const* filename ); // A02

      // Drawing (Raw)
      void Draw( int vertexCount, int vertexOffset = 0 );   // A02
      void DrawIndexed( int indexCount, int indexOffset = 0, int vertexOffset = 0 ); // A04

      // Drawing (Helpers)
      void DrawVertexArray( uint vcount, VertexPCT const* vertices ); // A02, A03
      void DrawMesh( GPUMesh* mesh );     // A04  

      // Database Functions
      Texture* GetOrCreateTexture( std::string const& filename ); // A03
      Shader* GetOrCreateShader( std::string const& filename );   // A03

   private:

      ID3D11DepthStencilState* m_currentDepthStencilState = nullptr; 

};


//-------------------------------------------------------------------------------
// Clearing Depth
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void RenderContext::ClearDepth( Texture* depthStencilTexture, float depth )
{
    TextureView* view = depthStencilTexture->CreateOrGetDepthStencilView(); 
    ID3D11DepthStencilView* dsv = view->GetAsDSV(); 
    m_Context->ClearDepthStencilView( dsv, D3D11_CLEAR_DEPTH, depth, 0 ): 
}

//-------------------------------------------------------------------------------
void RenderContext::SetDepthTest( eCompareFunc func, bool writeDepthOnPass ) 
{
   if (State Matches) {
      // just bind m_currentDepthStencilState
   } else {
      // Release Old State, make a new one (see below)

      D3D11_DEPTH_STENCIL_DESC desc; 
      memset( &desc, sizeof(desc), 0 ); 
      
      // TODO: fill out desc based on func and writeDepthOnPass - keep everything else default
      // ...

      // TODO: release old one
      // ...

      m_device->CreateDepthStencilState( desc, &m_currentDepthStencilState ); 
   }

   m_context->OMSetDepthStencilState( m_currentDepthStencilState );
}