Bloom
======

Bloom is a composite effect, with multiple steps.

- [ ] Output to two targets during your lighting pass
    - [ ] Color, what you were outputting before
    - [ ] Overflow Light, data where color ended up being larger than one.
- [ ] Apply a blur effect to the over flow output.
- [ ] Composite the color output with the blurred image (want to add them together)

## CPP Code
How that can look in code would be...

```cpp
Texture* colorOutput = g_renderer->AcquireRenderTargetMatching( backbuffer ); 
Texture* bloomOutput = g_renderer->AcquireRenderTargetMatching( backbuffer ); 

m_camera.SetColorTarget( 0, colorOutput ); 
m_camera.SetColorTarget( 1, bloomOutput ); 

// Render the scene
g_renderer->BeginCamera( m_camera );
RenderWorld(); 
g_renderer->EndCamera(); 

// blur the bloom
Texture* blurredBloom = g_renderer->AcquireColorTargetMatching( bloomOutput ); 
m_camera.BeginEffect( blurredBloom, bloomOutput, blurEffectShader ); 
m_camera.EndEffect(); 
g_renderer->ReleaseRenderTarget( bloomOutput ); // done with this at this point; 

// composite 
g_renderer->BeginEffect( backbuffer, colorOutput, addEffectShader ); 
g_renderer->SetMaterialTexture( 1, blurredBloom ); // need an extra texture for this effect
g_renderer->EndEffect(); 

// DONE
```

## Overflow Light
When doing lighting, we usually clamped values to 1.  This is normal behaviour for 
color targets in the `UNORM` family.  

So instead of clamping, we'll instead figure out how much over 1 they ended up being, and outputting that (scaled or unscaled)
to a second texture.

```cpp
color = finalColor; // surface color after all lighting has been applied

float3 overflow = max( color.xyz - float3(1,1,1), float3(0,0,0) ); 
bloom = float4( overflow, 1 ); // could scale if you want to allow more range outside of just 1 to 2....
```

So if a scene isn't lit, or is barely lit, the bloom will end up being black.  But for parts with a lot of specular and bright
lights, you'll see the bloom texture start having just those bright spots.


## Blurring Bloom
There a lot of techniques you can do here.  Leaving this as an excercise as it is fairly well documented online. 

- If you want something easy, just go with a simple box-blur, since pass effect.
- Could do a single pass guassian blur as well
- Could do a multi-pass (horizontal and vertical seperate) that is slightly faster, for a little more code.

## Composite
This image effect is dirty simple, ass you're just trying to add two images together.  You can do it with an effect, or just draw a quad over your screen.  I prefer the effect framework as I think the code reads a little more straight forward. 

