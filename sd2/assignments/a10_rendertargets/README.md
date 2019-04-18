A10: Render Targets & Pause Screen
======

## Overview
We're going to incorporate a pause screen into the game with the ability to Resume, Quit to Main Menu, and Quit to Desktoip.  

Pausing should fade the screen to grayscale and darket (or any other full screen effect that can be visibly seen taking effect), before rendering the the UI elements for the pause menu;

The pause menu should occlude clicking or selecting game UI elements, or movign the camera;  It should also fully work; 

New tech needed for this is `RenderTargets`, which you already mostly have the tech for, but we will now expose methods for creating arbitrary new render targets, as well as default to rendering to a secondary render target for a frame to allow us to run post effects;  

## Rubric

### [80%] Required
- [ ] Support creating arbitrary render targets (either fully defined and/or using a source texture to match)
- [ ] `RenderContext` default backbuffer is now a custom render target matching the swap chain
- [ ] Support `RenderContext::CopyTexture`
    - [ ] During `RenderContext::EndFrame`, copy the secondary render target to the swap chain backbuffer before Presenting
- [ ] Support `RenderContext::ApplyEffect( ColorTargetView *out, TextureView2D *in, Material *mat )`
- [ ] Pause Screen Implemented to spec and functional
    - [ ] Pause screen applies a full screen effect of frame up to this point, with UI drawn over the effect
    - [ ] Effect transitions in (fades, desaturates, pixelates, etc...)
    - [ ] Effect transitions out  


### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console

## Resources



## Extras

### Effects
- [ ] X10.00 (10%): ShadowMap

### Utility
- [ ] Debug Render PiP (Picture in Picture)

## Extra Information
