C29.SD2.A09 Render Targets
======

## Tasks
- [ ] `NamedProperties`
    - [ ] Switch named properties over to use `TypedProperty` instead of just strings
    - [ ] Add ability to subscribe methods to your event system. 
    - [ ] Add ability to unsubscribe an object from the event system (unsubscribes all methods on that object)

- [ ] Color Transform (ex: Grayscale) Effect
    - [ ] Create/Recycle a color target matching your swapchain's output.
    - [ ] Render game as normal
    - [ ] Create/Recycle another match target
    - [ ] Apply an effect on the original image, outputting to this new texture
        - [ ] Effect applies a per-pixel color transform using a matrix. 
        - [ ] Make it so you can specify a "strength" for this transform, where 0 
              has no effect, and 1 is the full effect so you can blend between them.
        - [ ] Be able to specify a tint and tint power that pixels trend toward (useful for fades)
              - `0` would have no effect
              - `1` the resultant color would be the tint 
        - [ ] **Optional Challenge: Have all the above be done with a single mat44.**
    - [ ] Copy this final image to the swapchain before present
   
- [ ] Bloom Effect
    - [ ] Set your normal color target, and a secondary "bloom" target
        - [ ] Camera can set set tertiary render targets
        - [ ] Shader has a secondary output specified
    - [ ] When done, be able to blur the bloom target
        - [ ] Create/Recycle a matching color/render target
        - [ ] Run a guassian blur pass N times, each pass consisting of one horizontal and one vertical pass
            - [ ] Each step in a pass will swap out the src/dst target and render a full screen blur shader
                - [ ] Run once horizontally
                - [ ] Run once vertically
    - [ ] Take the result of the blur, and the normal color output, and combine them
          into the final image.
    - [ ] 
    - [ ] Be able to toggle blur on-and-off to see it working or not
        - [ ] Disabling the blur just doesn't run the blur and composite steps;

- [ ] Texture Pool
    - [ ] Be able to ask your `RenderContext` for a temporary render target of a given resolution and size.
        - [ ] Search for an existing free texture first, and return it if you find one.
        - [ ] If there are none available, create and return a new one.
    - [ ] Be able to relinquish back these temporary textures back to the `RenderContext` when you're done with them.
        - This will allow them to be reused.
    - [ ] Add a `RenderContext::GetTotalTexturePoolCount()` to tell you how many textures have been created this way.
    - [ ] Add a `RenderContext::GetTexturePoolFreeCount()` to tell you how many are currently in the pool to be recycled
    - [ ] Debug render these counts to the screen to you can make sure you're properly recycling during this assignment.
        - At any given time you likely will not have more than 3 textures in use at once, meaning your pool should never exceed that count.  This can really depend on your scene though.  For eaxmple, in this assignment for bloom... 
          1. Camera color output
          2. Camera bloom target
          3. Temporaries
             - Blurring secondary output
             - Composite output 
          

## Notes
- [./named_properties.md](./named_properties.md)
- [./render_targets.md](./render_targets.md)
- [./object_pool.md](./object_pool.md)
- [./image_effects.md](./image_effects.md)
