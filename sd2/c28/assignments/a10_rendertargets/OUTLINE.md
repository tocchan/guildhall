A10 Suggested Order
======

## Get a Working Color Target
- [ ] Create a `ColorTargetView` matching your current backbuffer
    - [ ] Create a `Texture2D` to use as a color target matching your swapchain
    - [ ] Create a `ColorTargetView` for it
- [ ] Switch your default backbuffer to be this instead
- [ ] End of frame, copy this texture the swap-chain texture
    - [ ] `RenderContext::CopyTexture(...)` will be needed for this.  Look into `ID3D11DeviceContext::CopyResource` for this; 

## Apply Effect
- [ ] Implement `RenderContext::ApplyEffect(...)`
    - *May want to have a `m_effectCamera` on `RenderContext` to manage setting everything up...*
    - [ ] Set Outputs
    - [ ] Set Viewport & Scissor
    - [ ] Set Material
    - [ ] Set input texture (src) to slot 0
    - [ ] `Draw(3)` to draw a single triangle (will be using vertex ID trick to generate the poitns)
- [ ] Implement the `tonemap.hlsl` effect to help test this.  Single material
- [ ] TEMPORARY FOR TESTING
    - [ ] Create a secondary texture matching your backbuffer
    - [ ] Apply effect into this texture (you can not read/write to the same texture)
    - [ ] Copy the result back to the original texture.
    - *Note, you can apply multiple effects by ping-ponging between these two buffers as src/destination.  Just be sure to copy the final result to `dst` if you don't end there naturally.*
    - [ ] Verify that yous ee this effect

## Pause Screen
- [ ] Implement the game pause screen.
- [ ] When paused, apply the pause effect before rendering the pause screen UI
    - [ ] Pause screen should eat *all input* while open
- [ ] Implement `Resume`, `Quit to Main Menu` and `Quit to Desktop` options.
- [ ] Upon resuming, stop applying the effect
- [ ] Add a transition time to the effect to the effect enters and leaves gradually
- [ ] Test going back to main menu and then going back into game


## Additional Notes
- When creating a Texture2D meant to be used as a color target, make sure you apply the correct usage flags;  
    - `BUFFER_USAGE_COLOR_ATTACHMENT` allows you to create a `ColorTargetView` for it
    - `BUFFER_USAGE_TEXTURE`: allows you to create a `TextureView` for it
    - Since we both draw to, and use our backbuffer in this assignment, we will want to use both; 
- Previously, you created a `ColorTargetView` directly for the swapchain.  You may now want to directly create a texture from it instead, since all you need to do is copy to this at the end of your frame (this may help you find places )

- If you want to create a `TextureView` for your depth - it is more hands on.  The texture itself should be created as `DXGI_FORMAT_R24G8_TYPELESS`...
  - Creating a `DepthStencilView` should use the format `DXGI_FORMAT_D24_UNORM_S8_UINT`
  - Creating a `TextureView` should use the format `DXGI_FORMAT_R24_UNORM_X8_TYPELESS`
    - Meaning only "red" is readable from a depth-stencil
