```cpp
// old way

void Game::Startup()
{
	CPUMesh cube; 
	AddCubeToMesh( &cube, ... ); 
	m_cube_mesh = GPUMesh::FromCPUMesh<VertexLit>( ctx, cube ); 
}

void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext(); 

	ctx->BeginCamera( m_camera ); 

	// Where?
	ctx->SetModelMatrix( mat44::IDENTITY ); 

	// How?
	ctx->BindShader( m_lit_shader ); 
	ctx->BindTextureView( 0, m_color_texture ); 
	ctx->BindTextureView( 1, m_normal_texture ); 
	ctx->BindTextureView( 2, m_emissive_texture ); 
	ctx->BindSampler( 0, m_linear_sampler ); 
	ctx->BindUserUniformBuffer( m_uniform_buffer ); // needed for some extras; 

	// What am I drawing
	ctx->Draw( m_cube_mesh ); 

	ctx->EndCamera(); 
}

// new
void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext(); 

	ctx->BeginCamera( m_camera ); 

	// Where?
	ctx->SetModelMatrix( mat44::IDENTITY ); 

	// How?
	ctx->BindMaterial( m_couch_material ); 

	// What am I drawing
	ctx->Draw( m_cube_mesh ); 

	ctx->EndCamera(); 
}

```