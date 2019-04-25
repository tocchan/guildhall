Texture Errata
======

A slightly cleaner design for handling texture views.

Instead of keeping track of views ourselves - we'll instead let textures hold onto all their views, with the front-end supporting just using Textures.

This *does not* remove previous code of setting views, but instead just gives another option.  All rendering calls still should be working on views (for exmaple, Materials still hold onto shader resource views), since a view is much more specific than the texture itself, but it can give a nice shorthand in the common case.


## Preliminary Change
I am going to rename `TextureView` to `ShaderResourceView` to be more inline with D3D11s naming, as well as `TextureView` makes more sense as a base class all views share;  *Note: Technically in my engine it is the only class I have, as all views contain the same data.  The specific type names just help with compiler/type checking*. 


## Goal

High level, we want to just think in terms of textures - meaning other team members don't have to worry too much about the view when they want to do something - just worry about the texture; 

```cpp
void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext(); 
	Texture2D *backbuffer = App::GetBackbuffer(); 

	// Draw our world to the back buffer
	m_camera->SetColorTarget( texture ); 
	ctx->BeginCamera( m_camera ); 
	RenderScene(); 
	ctx->EndCamera();

	// Apply an effect
	m_scratchTexture = GetOrCreateScratchTexture(); 
	ctx->ApplyEffect( m_scratchTexture, backbuffer, m_grayscale ); 
	ctx->CopyTexture( backbuffer, m_scratchTexture ); // copy back so the backbuffer has the effect
}
```

But the rendre systems themselves would...

```cpp
void RenderContext::ApplyEffect( Texture2D *dst, Texture2D *src, Material *mat )
{
	Applyeffect( dst->GetOrCreateColorTargetView(), src->GetOrCreateShaderResourceView(), material ); 
}

void RenderContext::ApplyEffect( ColorTargetView *dst, ShaderResourceView *src, Material *mat )
{
	// ... what you had before
}
```

## Work
So idea is, Textures are wholey responsible for creating their views - and they cache them; 

A good first version of this is...

```cpp
// technically, class Texture could do this, but just in case you don't support 1D and 3D textures
class Texture2D 
{
	public:
		ShaderResourceView* GetOrCreateShaderResourceView(); 
		ColorTargetView* GetOrCreateColorTargetView(); 
		DepthStencilTargetView* GetOrCreateDepthStencilTargetView(); 

	private:
		eTextureUsageBits m_usage; // controls what views can be made	

		// the three views - 
		ShaderResourceView *m_srv = nullptr; 
		ColorTargetView *m_ctv = nullptr; 
		DepthStencilView *m_dsv = nullptr; 		

		// ...
		// whateve relse was on a texture
};
```

Create the views when needed, and store them around.  Be sure to destroy them when the texture goes away. 


### More Flexibility
This way grows better if you plan to do more advanced rendering techniques where you could potentially need multiple views of the same type.  This is not a common usage case, so would not recommend going down this route unless you are thinking of a rendering thesis or DFS.  

The idea is to be able to fully describe the view, and cache them off, but still maintain old functionality of returning a good default view when the user doesn't care; 

```cpp
struct TextureViewCreateInfo
{
	eTextureUsageBit m_viewType; // which type of view would this be?
	// other options, for exmaple, start mip layer and count, start array and count, format, etc...

	uint GetHash() const; // a unique value identifying this create info.  For now, can just return m_viewType
};
```

Also, we need a common way to think about views...

```cpp
class TextureView
{
	private:
		eTextureUsageBit m_viewType;  

		// aliased handles
		union {
			ID3D11View *m_view; 			// base type...
			ID3D11ShaderResourceView *m_srv; 
			ID3D11RenderTargetView *m_rtv; 
			ID3D11DepthStencilView *m_dsv; 
			// there are other views, but these are the three we'll care about
		}; 

		// other data you may want (look at what is common between ColorTargetView, TextureView2D, and DepthStencilTargetView)
		// ...
};
```

This allows us to have `Texture2D` look like this; 

```cpp
class Texture2D
{
	public:
		// if no create info is supplied - construct a defualt one to use.
		ShaderResourceView* GetOrCreateShaderResourceView( TextureViewCreateInfo const *ci = nullptr ); 
		ColorTargetView* GetOrCreateColorTargetView( TextureViewCreateInfo const *ci = nullptr ); 
		DepthStencilTargetView* GetOrCreateDepthStencilTargetView( TextureViewCreateInfo const *ci = nullptr ); 

	private:
		eTextureUsageBits m_usage; // controls what views can be made	

		// All views
		// will search for a view matching this info, or create one and add it to the map...
		TextureView* GetOrCreateView( TextureViewCreateInfo const *info );
		std::map<uint,TextureView*> m_views; 

		// ...
		// whateve relse was on a texture
};
```
