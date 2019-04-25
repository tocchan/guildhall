A10: Render Targets & Pause Screen
======

## Overview
We're going to incorporate a pause screen into the game with the ability to Resume, Quit to Main Menu, and Quit to Desktoip.  

Pausing should fade the screen to grayscale and darket (or any other full screen effect that can be visibly seen taking effect), before rendering the the UI elements for the pause menu;

The pause menu should occlude clicking or selecting game UI elements, or movign the camera;  It should also fully work; 

New tech needed for this is `RenderTargets`, which you already mostly have the tech for, but we will now expose methods for creating arbitrary new render targets, as well as default to rendering to a secondary render target for a frame to allow us to run post effects;  

## Rubric

### [90%] Required
- [ ] 10%: Support creating arbitrary render targets (either fully defined and/or using a source texture to match)
- [ ] 10%: `RenderContext` default backbuffer is now a custom render target matching the swap chain
- [ ] 10%: Support `RenderContext::CopyTexture`
- [ ] 10%: During `RenderContext::EndFrame`, copy the secondary render target to the swap chain backbuffer before Presenting
- [ ] 20%: Support `RenderContext::ApplyEffect( ColorTargetView *out, TextureView2D *in, Material *mat )`
- [ ] Pause Screen Implemented to spec and functional
    - [ ] 10%: Pause screen applies a full screen effect of frame up to this point, with UI drawn over the effect
    - [ ] 05%: Effect transitions in (fades, desaturates, pixelates, etc...)
    - [ ] 05%: Effect transitions out  
    - [ ] 10%: Pause screen consumes all input 


### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console

## Resources

Once you have the base functions working, you can make life easier on yourself by adding some common helpers that use it.  For example;

```cpp
// simpler ApplyEffect in place
void RenderContext::ApplyEffect( Texture *tex, Material *mat ) 
{
	// I like having a pool of scratch textures I can use for this (you will probably only need one)
	// that gets recreated if the size needs change)
	Texture *scratch = GetOrCreateScratchTextureMatching( tex );

	ColorTargetView *ctv = scratch->CreateColorTargetView(); 
	TextureView2D *srv = scratch->CreateTextureView(); 
	
	ApplyEffect( ctv, srv, mat ); 
	CopyTexture( tex, scratch ); // copy scratch back to tex so tex is left with the effect

	delete ctv; 
	delete srv; 
	// don't delete scratch - handled by RenderContext
}

// Apply multiple effects in order
void RenderContext::ApplyEffects( Texture *tex, std::vector<Material*> effects ) 
{
	// similar to above, but just swap back and forth between the two textures
	// and if you stop on an odd effect (the result is in scratch), do a copy, otherwise nothing
	// ...
}

// Just apply the effect to the current backbuffer on the camera - assuming the backbuffer
void Camera::ApplyEffect( Material *mat ) 
{
	m_context->ApplyEffect( m_colorTargetView ); 
}
```

## Extras

### Effects
- [ ] X10.00: (10%) Depth of Field Blur
- [ ] X10.01: (10%) ShadowMaps

### Utility
- [ ] X10.10: (05%) Debug Render PiP (Picture in Picture)

### Additional Stages
- [ ] X10.20: (05%) Implement a shader using the Tesselation Stages (displacement mapping is a good starter)
- [ ] X10.21: (05%) Implement a shader using the Geoemetry Stage 

## Extra Information
