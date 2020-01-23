## Render Target Views
So, we have a texture for the backbuffer.  But the only way we can 'draw' to a texture is if it is in the correct layout/format.  This is handled with `views` in D3D11.  

You create the view of the texture, hinting on how you want to use it, and D3D11 will make sure the texture is correctly setup to be used as such; 

Our first use case is clearing, and for that we'll want a `ID3D11RenderTargetView`. 

First, our objects;

## `Texture` and `TextureView`

```cpp
class Texture
{
    public:
        Texture( RenderContext* owner, ID3D11Texture2D* handle ); // constructor we need for swapchain
        ~Texture(); 

        TextureView* GetRenderTargetView(); 


    public:
        RenderContext* m_context; // owning context

        // TODO - temp - for now we only have one view type, so we'll hard code to that
        // case, but in A03 we'll have multiple view types so we'll need to revisit this
        TextureView* m_rtv; 

        // all texture types inherit from m_handle, 
        // and we'll be using `Texture` as basically an alias for all
        // texture types - more similar to more modern apis; 
        union {
            ID3D11Resource* m_handle; 
            ID3D11Texture2D* m_tex2D; 
        }; 
}; 
```

```cpp
// This class is only used by the engine.  Texture is going to be our front-facing interface,
// and views are just a concept we need for the API, so having this be a little ugly doesn't matter.
class TextureView
{
    public:
        TextureView();
        ~TextureView(); 

    public:
        // TODO in later assignment
        // add some identifying information about this type of view

        union {
            ID3D11View* m_handle; 
            ID3D11RenderTargetView* m_rtv; 
            // more to come in later classes
        }; 
}
```

## Make the View
This class is fairly straight forward for now.  Setup the pointers
in the constructor and make sure you release the D3D11 handle (`m_handle`) when
it deconstructs.

The interesting bit is going to be `GetRenderTargetView`

```cpp
TextureView* Texture::GetRenderTargetView() 
{
     // this uses lazy instantiation, meaning we'll only
     // make a view the first time it is requested.  As there
     // are many types of views, but most textures will at most use one or two.

    // only need to create it once
    if (nullptr != m_rtv) {
        return m_rtv; 
    }

    // now we create it.  If we need a special type of view
    // we would have to fill out a `D3D11_RENDER_TARGET_VIEW_DESC`, but 
    // in this case, the default will be fine; 

    // get the device, since we're creating something
    ID3D11Device* dev = m_context->m_device; 
    ID3D11RenderTargetView* rtv = nullptr; 
    dev->CreateRenderTargetView( &m_handle, nullptr, &rtv ); 
    if (nullptr != rtv) {
        // great, we made it, so make OUR object for it
        m_rtv = new TextureView();
        m_rtv->m_rtv = rtv;  // setup the member
    }

    return m_rtv; 
}
```

In the future, we'll need to add some identifying information on the view so it knows
what kind it is, but for now we only have one, so we'll keep it simple; 

## Clearing
Now that we have a color target, we can clear the screen;  

Clearing the screen is a *command*, so it is issued by the `ID3D11DeviceContext`.

```cpp
// very pseudo code
void RenderContext::BeginCamera( Camera* cam )
{
    if (cam should clear color) {
        Texture* target = m_swapchain->GetBackBuffer(); 
        TextureView* rtv = target->GetColorTargetView(); 

        float color[4]; // R, G, B, A
        fill color from camera clear color

        m_context->ClearRenderTargetView( rtv->m_rtv, color ); 
    }

    // rest of setup
};
```