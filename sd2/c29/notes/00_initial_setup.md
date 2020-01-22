## Add Me as a Collaborator
Ran out of time for this during MP2

## Mag-Lock Doors!
Are locked permantly starting next week, so always bring your key-card.  

## Syllabus

## Project
- Wolfenstein 3D style game. 
- Incursion Gameplay
- Hexen/Heretic Theme (High Fantasy)
- Focus is on making it look good, not the game!

------

## OpenGL to D3D11
- OpenGL 1
- Why not OpenGL 3.3+?
  - *magic*
  - the time cost would not be any less
- Why not Vulkan/D3D12?
  - TTT - Time To Triangle
    - Also used to refer how long it takes to 
      fully take advantage of the hardware, but harder to quantify
- D3D11 Benefits
  - Getting much more comfortable with how we interface with a GPU
  - Memory and synchronization is mostly taken care of for us
  - Very streamlined, usually one way to do the thing you want as they have no legacy support to older versions
  - Sane errors
  - Works reliable with RenderDoc and Visual Studio Visual Debugger
  - I'm very comfortable with it
  

## Pipelines
- Fixed Function Pipeline, OpenGL1
- Programmable Pipelines
  - [Graphics Pipeline](https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-graphics-pipeline)
  - Compute
  - *Raytracing*
  - Mesh Shader Pipeline

## D3D11 Concepts 

- Physical Device
- Device
- Device Context
- SwapChain
  - Display
  - Desktop
- Textures
  - Views (similar to an alias)
    - Shader Resource
    - Color Target

- Host -vs- Client
  - *Host* - The GPU, who we are making requests to
  - *Client* - Our application

------

  ## Window


