class SwapChain
{
   public:
      SwapChain( RenderContext* ctx, Window* wnd ); 
      ~SwapChain(); 

      Texture* GetFrameColorTarget(); 
      void Present(); 

   public:
      Window* m_window; 
      ID3D11SwapChain* m_swapchain;    

      Texture* m_backbuffer; 
}; 