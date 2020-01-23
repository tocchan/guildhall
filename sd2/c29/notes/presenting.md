## Swapchain

The class we'll be making and working with.  This is charge of presenting to the user,
and takes the roll of `SwapBuffers` from OpenGL.

```cpp
class SwapChain
{
    public:
        Texture* GetBackBuffer(); // what do we want to render to?
        void Present();     // does the job of `SwapBuffers`


    public:
        RenderContext* m_context; // owning context
        IDXGISwapChain* m_swapchain; 

        Texture* m_backbuffer; 
}; 
```

## Resource Management
First, we'll be creating this based on our needs from the device creation.  Also, just get ontop of releasing our resources; 

```cpp
SwapChain::SwapChain( RenderContext* ctx, IDXGISwapChain* swapchain )
{
    m_context = ctx;   // we'll be using the context from this method, so good to know how made it
    m_swapchain = swapchain; 
}

SwapChain::~SwapChain()
{
    // and we own it, so we should release it; 
    DX_SAFE_RELEASE(m_swapchain); 
}
```

## Presenting

So to present, we first need to be able to get to our backbuffer, so let us start there;

```cpp
Texture* SwapChain::GetBackBuffer()
{
    // we already have it, so just return it; 
    if (nullptr != m_backbuffer) {
        return m_backbuffer; 
    }

    // first, we request the D3D11 handle of the textures owned by the swapbuffer
    ID3D11Texture2D* texHandle = nullptr; 
    m_swapchain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (void**)&texHandle ); 
    // recommend an ASSERT_OR_DIE that this worked.

    // once we have the handle, we'll wrap it in our class to make the interface easy to use
    // we are giving our reference we got from `GetBuffer` to this texture, who will handle
    // it from here on; 
    m_backbuffer = new Texture( m_context, texHandle ); 
    
    return m_backbuffer; 
}
```

Now that we have the backbuffer, we can use it to [clear the screen](./clearing.md).  For now, lets move onto presenting. 

```cpp
void SwapChain::Present()
{
    m_swapchain->Present( 1, // vsync - 0 if you don't want to wait
        0 ); // DXGI_PRESENT option, see docs
}
```

And that's all there is to it.  Now, we just need to fill that buffer; 