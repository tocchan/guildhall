SD2-A03:  Developement Console
======

## Goal
Get D3D11 renderer to feature parity, and use this to finish out the development console; 

## Rubric
- [ ] **30%**: `RenderContext` SD1 Refactor Complete [Dev Console Renders!]
- [ ] **50%**: DevConsole base features are implemented
- [ ] **20%**: Extras Selected ;

## Final RenderContext Example

```cpp

void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext();

	ctx->BeginCamera( m_camera ); 

	ctx->BindShader( "shader/default_unlit.hlsl" ); 
	ctx->BindTextureViewWithSampler( "image/someImage.jpg" ); 

	// draw a quad wiht a texture, tinted light blue and with 75% opacity. 
	std::vector<VectorPCU> quad; 
	AddVerticesForQuad( &quad, aabb2::FromPoints( vec2(-10.0f), vec2(10.0f) ), rgba( .5f, .5f, 1.0f, .75f ) ); 
	ctx->DrawVertexArray( quad ); 

	ctx->EndCamera(); 
}

```

## Steps

- [ ] Implement `Texture2D` - this replaces how you were 'loading' a `Texture` from SD1
- [ ] Implement `TextureView2D` - this replaces how you were 'using' a `Texture` from SD1
- [ ] Replace `RenderContext::GetOrCreateTexture` with `RenderContext::GetOrCreateTextureView` 
- [ ] Implement `Sampler`
- [ ] Try to 
