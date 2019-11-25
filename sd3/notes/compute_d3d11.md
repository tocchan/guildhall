Compute Shaders (D3D11)
======

## Intial Information Dump

### Reference
- Warps/Wavefronts: https://blogs.msdn.microsoft.com/nativeconcurrency/2012/03/26/warp-or-wavefront-of-gpu-threads/
- Basic Concepts in GPU Computing: https://medium.com/@smallfishbigsea/basic-concepts-in-gpu-computing-3388710e9239
- GPU Flow Control Idioms: https://developer.nvidia.com/gpugems/GPUGems2/gpugems2_chapter34.html
- GPU Optimization Fundamentals: https://olcf.ornl.gov/wp-content/uploads/2013/01/optimization-fundamentals.pptx
- Occupancy Calculator: https://docs.nvidia.com/cuda/cuda-occupancy-calculator/index.html
- NVIDIA Turing Architecture White Paper: https://www.nvidia.com/content/dam/en-zz/Solutions/design-visualization/technologies/turing-architecture/NVIDIA-Turing-Architecture-Whitepaper.pdf

### Terms
- **Single Instruction Multiple Thread (SIMT)** - Architecture of the GPU.  The processor is running multiple threads in lockstep, so a single instruction is run across the entire *Warp*, usually 16 or 32 threads wide in modern architecture.
- **Warp** - Smallest executable unit of code
  - aka: **Wavefront**
- **Streaming Multiprocessor (SM)** A cluster of cuda cores.  
- **CUDA Core** - A single core as part of a scalar machine; 
- **Instruction Level Parallelism (ILP)**: When multiple lines of a program are actually run in parallel;  See: https://www.computerhope.com/jargon/i/ilp.htm
- **Diverged** - When different threads in the same warp end up taking different execution paths.
  - All threads will run all code that is executed in that warp (other threads will no-op (nop) while they wait)

## My understanding
- Make a the group thread size a multiple of the warp size (a multiple of 32 is usually fine, I usually go for 128/256 sized groups)
- Each Warp can share data freely amongst its threads
- Each group will run on a single SM, and sharing data amongst a group is "fairly quick"
  - *SMEM* - shared memory - shared across the SM
  - *GMEM* - global gpu memory
- Sharing data across groups is slower (they share an L2 Cache), but doable in a pinch (atomic counters, etc...)

------

## Using
A lot of the information above is more useful when you get to optimizing your shaders, for now, lets just get to using a Compute Shader, since
using something poorly is better than not using something at all. 

### Concept
- Doing the physics step for a lot of particles; 
  - Divide and Conquer (a concurrent for loop)
    - Like Job Scheduling
    - Even Cuts (!!)

- GPUs are great for work that requires very little data sharing (it can share, but we want to reduce serial work)

- GPU is like the even cuts approach.  
  - Break up your problem spatially
    - 1D : Physics Updates, Neural Networks
    - 2D : Image Processing, 2D Spatial Hashing, 
    - 3D : Light Bucketing (Forward+/Deferred+ Rendering)

### Terms
- `Dispatch` - Runs a compute job, specifying a 3D grid of groups to process; 
- `Group` - One cell of the dispatch - will run a **Thread Group**, which is specifying by the shader
- `Thread Group` - How to break down a group spatially.  Thread Group Size area should be multiple of 32 (128-256 is a recommended starting point)
- `Thread` - One worker or execution of the program.
- `Unordered Access Viwe (UAV)`: A resource with read/write acesss

So you dispatch the entire job, and write a single thread.  

## Clear Screen Compute Job
A very basic "Hello World" compute shader that does nothing but clears the screen; 


### GPU Side

```c
// clear.hlsl

//--------------------------------------------------------------------------------------
// Compute shaders do not have "streaming" inputs and outputs like a traditional shader
// the only per instance data is your location with the dispatch/group/thread_group
struct compute_input_t
{
   uint3 group_id : SV_GroupID;              // What is my index within the context of a the dispatch call
   uint  group_idx : SV_GroupIndex;          // Flattend version of the top (z * (width * height) + y * width + x)
   uint3 group_thread_id : SV_GroupThreadID; // My thread within the context of the group
   uint3 dispatch_id : SV_DispatchThreadID;  // My thread within context of the entire dispatch [5x5x1, with threads 5,5,1]  If I was in 2, 2, 0 group and 3, 3, 0 thread, I would be in 13 13 0 thread id
}; 

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

// we pass data to the shader through a combination 
// of constant buffers, textures, and structued buffers
cbuffer CLEAR_CONSTANTS : register(b0)
{
   float4 CLEAR_COLOR;
};

RWTexture2D<unorm float4> tOutput : register(u0);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
[numthreads(16, 16, 1)]
void Main( compute_input_t input ) // semeantic of what I'm returning
{
   tOutput[input.dispatch_id.xy] = CLEAR_COLOR; 
}
```

### CPU Side
Shader compiliation is identical, except your compiling now for compute; 

- Compile using `ID3D11Device::CreateComputeShader`, instead of `CreateVertexShader` or `CreatePixelShader` that you were doing before
- Use the target `"cs_5_0"` now when compiling the shader to byte code (look for where you are calling `D3DCompile`)


```cpp
ID3D11DeviceContext* context;

context->CSSetShader( my_compute_shader );
context->CSSetConstantBuffers( 0, my_clear_constants );     // constant buffers do not change - just be sure to set them to this stage
context->CSSetUnorderedAccessView( 0, my_clear_target );    // to output to a texture, it must be bound as an UAV (Unordered Access view)

// so if we want to clear our target, and we know the thread size for our job was 16x16x1, we can 
// determine our group count with simple math;
ivec3 thread_group_size = ivec3( 16, 16, 1 );  // compiled into the shader 
ivec3 total_thread_size = ivec3( my_clear_target->get_dimensions(), 1 ); // I want a thread per pixel
ivec3 group_count = (total_thread_size + (thread_group_size - ivec3(1, 1, 0))) / thread_group_size; // a divide with a round up; 

// run the compute shader
context->Dispatch( group_count.x, group_count.y, group_count.z ); 
```

### Creating a UAV
- Texture must be GPU Writable (same things you do for a render target)
- Be sure to set an additional bind flag; `D3D11_BIND_UNORDERED_ACCESS`
- Create an unordered access view for it; 

```cpp
D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
MemZero( &desc );

desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;;
desc.Texture2D.MipSlice = 0;

// NOTE:  Depth Stencil Buffers need to have a different view
// than their normal D24S8, otherwise it will fail
// we we'll cast it to an R24_UNORM, and ignore stencil
desc.Format = DXFormatFromTextureFormat( format );
if (format == TEXTURE_FORMAT_D24S8) {
   desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
}

// ID3D11UnorderedAccessViwe* dx_uav; 
result = dd->CreateUnorderedAccessView( dx_resource, &desc, &dx_uav );
CheckD3DResult( result ); // just some error reporting that is removed in Release
```