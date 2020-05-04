C29.SD2.A09 Render Targets
======

## Tasks
- [ ] `NamedProperties`
    - [ ] Switch `EventArgs` to use `NamedProperties` instead `NamedStrings`
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
        - [ ] Be able to specify a tint and tint strength that pixels trend toward (useful for fades)
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
        - [ ] Can use any blur algorithm you can find online, some suggestions...
            - Simple single pass box blur
            - Single pass guassian blur
            - Multiple pass guassian blur
    - [ ] Take the result of the blur, and the normal color output, and combine them
          into the final image.
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
- [./image_effects.md](./image_effects.md)
- [./multiple_targets.md](./multiple_targets.md)
- [./bloom.md](./bloom.md)


## Q&A From Class

### Subscribe Methods to Evente System

```cpp
// what you have - subscribe C function

static bool GlobalOnClick( EventArgs& args ) 
{
  g_theGame->DoAThing(); 
  // do something on click
}

bool Game::OnClick( EventArgs& args ) 
{
  // game method for click
  // do cool things...
  // ...
  DoAThing(); 
}


void Game::Init()
{
  g_theEventSystem->SubscribeToEvent( "Clicked", GlobalOnClick ); 
  g_theEventSystem->SubscribeMethodToEvent( "Clicked", this, &Game::OnClick ); 
}

void Game::Deinit()
{
  g_theEventSystem->UnsubscribeObject( this ); 
}
```
 
So how do we do this?

1. If you did `Delegate`, you can just switch your internals of event system to use degate, instead of C function pointer.
   - May need to update `Delegate` to also be able to define return type.

2. If you don't have delegates, you are going to have to do something similar to the delegate class.  Which is give yourself identifiable `std::function`.  See notes delegate on how to make `std::function` identifiable. The notes for that are in `mp2/c29/assignments/overflow/delegate.md`


### Delegates are Templated, how to use in Event System

Returning return types in your delegate...

```cpp
// This takes callbacks that return bool - stop calling callbacks if any one returns bool (short circuit)
// More useful for event system.
template <typename ...ARGS>
class ConsumableDelegate 
{
  // std::function<bool(ARGS...)>
  // bool (*func_ptr)(ARGS...)

  void UnsubscribeObject( void* objPtr ) 
  {
    foreach (sub in subscriptions) {
      if (sub.objectPointer == objPtr) {
        // remove myself
      }
    } 
  }

  // update rest of class to use RET_TYPE where it makes sense
  bool Invoke( ARGS... const& args ) 
  {
    // passes
    for each( callback ) {
      if (callback(args...)) {
        return true; 
      }
    }

    return false; 
  }
};
```

```cpp
std::map<std::string,ConsumableDelegate<EventArgs&>> m_eventCallbacks; 

void FireEvent( std::string const& evt, EventArgs const& args )
{
  ConsumableDelegate<EventArgs&>* delegate = FindInMap(evt);
  if (delegate != nullptr) {
    delegate->Trigger( args ); 
  } 
}
```

------
## Color Tonemapping with a Matrix

Three common ways to do it...

```cpp
// 1. hardcode the equation
float lum = .2126f * input.r + .7152f * input.g + .0722f * input.b; 
return float4( lum, lum, lum, input.a ); 



// 2. transform

// Grayscale Matrix
/* tonemap (grayscale) matrix = {
  0.2126, 0.7152, 0.0722, 0, 
  0.2126, 0.7152, 0.0722, 0, 
  0.2126, 0.7152, 0.0722, 0, 
  0,      0,      0,      1
} 
*/

/* Sepia
newRed = 0.393*R + 0.769*G + 0.189*B
newGreen = 0.349*R + 0.686*G + 0.168*B
newBlue = 0.272*R + 0.534*G + 0.131*B

tonemap (sepia) matrix = {
  0.393,  0.769,  0.189, 0, 
  0.349,  0.686,  0.168, 0, 
  0.272,  0.534,  0.131, 0, 
  0,      0,      0,     1
}
*/

/* Fade
tonemap (fade) matrix = {
  1 - time,  0,           0,          time * tint_color.r,
  0,         1 - time0,   0,          time * tint_color.g,
  0,         0,           1 - time0,  time * tint_color.b,
  0,         0,           0,          1 
}

/* Tint
tonemap (tint) matrix = {
  tint.r,    0,           0,          0, 
  0,         tint.g,      0,          0,
  0,         0,           tint.b      0,
  0,         0,           0,          tint.a 
}
*/
// a float TONEMAP_STRENGTH
return lerp( input, TONEMAP_MATRIX * input, TONEMAP_STRENGTH );  


3. LUT (Look-Up Texture)
// this is a 3d texture (texLut is a texture3d)
float3 outputColor = texLut.Sample( someSampler, input.color.rgb ); 
return float4( outputColor, input.color.a ); 
```

### Animating the ToneMap
```cpp
// IN GAME CODE
void Game::Render()
{
  // finish rendering your image
  // ...

  // apply the effect
  mat44 tintMatrix = TonemapTint( rgba::BLUE );
  mat44 finalTint = LerpComponents( mat44::IDENTITY, tintMatrix, howMuchTintIWant ); // howMuchTintIWant between 0 and 1

  mat44 grayscaleTonemap = TonemapGrayscale();  // returns a tonemap matrix for grayscale
  mat44 tonemapMatrix = finalTint * grayscaleTonemap; 

  g_theRenderer->BeginEffect( dst, colorOuput, tonemapEffect ); 
  g_theRenderer->SetMaterialData( &tonemapMatrix, sizeof(mat44) ); 
  g_theRenderer->EndEffect(); 
}
```

```cpp
mat44 LerpComponents( mat44 a, mat44 b, float t )
{
  mat44 ret;
  for (int i = 0; i < 16; ++i) {
    ret[i] = Lerp( a[i], b[i], t ); 
  }
  return ret; 
}
```



