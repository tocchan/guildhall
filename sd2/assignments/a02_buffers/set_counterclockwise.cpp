void RenderContext::Startup()
{
	// Temporary
	SetupDefaultRasterState(); 
}

void RenderContext::Shutdown()
{
	DX_SAFE_RELEASE( m_defaultRasterState ); 
}

void RenderContext::SetupDefaultRasterState()
{
	D3D11_RASTERIZER_DESC desc; 

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = TRUE; 
	desc.DepthBias = 0U; 
	desc.DepthBiasClamp = 0.0f; 
	desc.SlopeScaledDepthBias = 0.0f; 
	desc.DepthClipEnable = TRUE; 
	desc.ScissorEnable = FALSE; 
	desc.MultisampleEnable = FALSE; 
	desc.AntialiasedLineEnable = FALSE; 

	// ID3D11RasterizerState *m_defaultRasterState; 
	m_device->CreateRasterizerState( &desc, &m_defaultRasterState );
	m_context->RSSetState( m_defualtRasterState ); 
}