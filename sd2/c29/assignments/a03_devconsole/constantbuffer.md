Uniform Buffers/Constant Buffers
======

## What Is It
A **Uniform Buffer** (aka: **Constant Buffer**) is a buffer of data that can 
be bound to the pipeline.  The data is considered immutable during the duration 
of a draw call, but may be updated/changed out in between calls.

It is declared in a shader with `cbuffer`, and should be given a 
register (a bind point) of where it will be looking.

```hlsl 
// buffer holding time information from our game
cbuffer frame_constants : register(b0)          // constant buffer slot 0
{
   float SYSTEM_TIME;
   float SYSTEM_FRAME_TIME;
   float GAME_TIME;
   float GAME_DELTA_TIME;
};
```

The bind point is associated to where the CPU binds the buffer, done with the following code; 

```cpp
//ID3D11DeviceContext* dxContext; 
ID3D11Buffer* uboHandle = ubo->GetHandle();
uint bindPoint = 0;  
uint bufferCount = 1; 

// Bind the constant buffer to be visible from the Vertex and Fragment stages
dxContext->VSSetConstantBuffers( bindPoint, bufferCount, &uboHandle ); 
dxContext->PSSetConstantBuffers( bindPoint, bufferCount, &uboHandle ); 
```

Now those constants will be set for all future `Draw*` calls until something new is set, 
or the state is cleared; 

------

## Getting Started
In general, we'll group data into a uniform buffer based on how often it needs to be changed.

For example...
- Per Frame Data (time)
- Per Camera Data (view/projection matrcies)
- Per Material (material property block)
- Per Object Data (model matrices, material instances)

```cpp
RenderFrame()
	UpdateAndBindFrameData()
	ForEach( Camera )
		UpdateAndBindCameraData(); 
		ForEach( Object )
			UpdateAndBindObjectData();
			DrawObject()
```

To start, lets add time to our shader, and use it; 

First, we'll need to make a `RenderBuffer` to act as our *Uniform Buffer*

```cpp
struct time_data_t
{
	float systemTime; 
	float systemDeltaTime; 
};

RenderContext::Startup()
{
	// RenderBuffer* m_frameUBO; 
	// a UniformBuffer needs to be marked as such, and tends to be
	// updated at least once a frame, so we'll use dynamic memory
	m_frameUBO = new RenderBuffer( this, UNIFORM_BUFFER_BIT, RENDER_MEMORY_DYNAMIC );
}

RenderContext::BeginFrame()
{
	// get our time data
	time_data_t timeData;
	timeData.systemTime = TimeGetSeconds();
	timeData.systemDeltaTime = TimeGetFrameDeltaSeconds(); 

	// copy it to the UBO (Uniform Buffer Object)
	m_frameUBO->Update( &timeData, sizeof(timeData ) );
}

RenderContext::BeginCamera( Camera* cam )
{
	// normal camera startup;
	// ...

	// bind our frame data
	BindUniformBuffer( 0, m_frameUBO ); 
	// BindUniformData( 1, m_cameraUBO ); 
}
```

A *Uniform Buffer* can be bound to specific stages, telling the GPU which stages can see 
it.  For simplicity, we'll just always assume we want both our stages to see it;
 
```cpp
RenderContext::BindUniformBuffer( uint slot, RenderBuffer* ubo ) 
{
	ID3D11Buffer* uboHandle = ubo->GetHandle();
	uint bufferCount = 1; 

	// Bind the constant buffer to be visible from the Vertex and Fragment stages
	dxContext->VSSetConstantBuffers( slot, bufferCount, &uboHandle ); 
	dxContext->PSSetConstantBuffers( slot, bufferCount, &uboHandle ); 
}
```

After this is bound, we'll update the shader and try to use the new data.  A soft learn here is that you are not writing a program to support all shaders, you are writing shaders that are compatible with your program.  So you should plan what data you want to expose and where, and make sure you write shaders to match.  **Do not try to write a program that *just works* with all shaders - the way lies only pain.**

Now slot 0 has time data, so our shader can now do...

```hlsl
cbuffer frame_constants : register(b0)          // constant buffer slot 0
{
   float SYSTEM_TIME;
   float SYSTEM_FRAME_TIME;
}; 

v2f_t VertexFunction( vs_input_t input )
{
	// SYSTEM_TIME is now a constant we can use anywhere in this file

	float radius = 0.5f; 
	float4 position = float4( input.position, 1 ); 
	position.x += radius * cos(SYSTEM_TIME);
	position.y += radius * sin(SYSTEM_TIME); 

	v2f.position = position;  

	// first rest of the output
	// ... 
}
```


## Links
  - https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0


