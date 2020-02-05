- buffer attribute t
- binding vertex buffers
- hooking input layout to the vertex shader (entry point)

## Drawing in a Modern API

GPUs can not draw from CPU memory directly, but instead must draw from GPU memory.  We now have a way 
too allocate GPU memory and fill it with vertices, so next we need figure out how to get the shader to use this.

So for example, `DrawVertexArray` now looks similar to this;

```cpp
void RenderContext::DrawVertexArray( VertexPCT const* vertices, uint count )
{
	// BindShader( shader ); // already done before this call
	size_t byteSize = count * sizeof(VertexPCT); 
	size_t elemSize = sizeof(VertexPCT); 

	// update gpu buffer with our CPU data
	m_immediateVBO->Update( vertices, byteSize, elemSize ); 

	// make sure that VBO is bound as an input
	BindVertexBuffer( vbo ); // vbo - vertex buffer object
	
	// draw our vertices
	Draw( count ); 				
```

During draw, we need to tie the vertex buffer to the shader, using an object called an `ID3D11InputLayout` 

```cpp
void RenderContext::Draw( int vertCount, int offset )
{
	ID3D11InputLayout* layout = ???; 
	m_deviceContext->IASetInputLayout( layout ); 
	m_deviceContext->Draw( vertCount, offset ); 
}
```

So what is a layout and how do we make one?


### `ID3D11InputLayout`

- Shader
- Creating an Input Layout (what is needed?)
- Describing our Vertex in a way D3D11 can understand
  - `buffer_attribute_t`
  - `VertexBuffer`
- `Shader::GetOrCreateInputLayout( buffer_attribute_t const* layout )`

Creating the layout involves filling out a `D3D11_INPUT_ELEMENT_DESC` array describing your vertex
format to the D3D11 device.  The format is very versatile, allowing for multiple ways to describe 
vertices (array of structs, struct of arrays, instance data, arrays, etc...)

```cpp
struct D3D11_INPUT_ELEMENT_DESC
{
	LPCSTR SemanticName;						// semantic name in shader of the data we're bindnig to; 
	UINT SemanticIndex;							// If you have an array, which index of the area are we binding to
	DXGI_FORMAT Format;							// What data is here
	UINT InputSlot;								// Which input slot is the data coming from (where you bind your stream)
	UINT AlignedByteOffset;						// where the data appears from start of a vertex
	D3D11_INPUT_CLASSIFICATION InputSlotClass;	// type of data (vertex or instance)
    UINT InstanceDataStepRate;					// used in instance rendering to describe when we move this data forward
};
```

**Instance Rendering** is a more advanced topic we'll cover as an optional topic in SD2.  Feel free to pick my brain after class if you're curious.

So to describe our `VertexPCU`, which I'm assuming looks similar to the following; 

```cpp
struct VertexPCU
{
	Vec3 position;
	RGBA8 color; 
	Vec2 uv; 
};
```

A D3D description would be;

```cpp
D3D11_INPUT_ELEMENT_DESC d3d_desc[3]; // we'll describe three elements

// position
d3d_desc[0].name = "POSITION"; // should match the shader input
d3d_desc[0].semanticIndex = 0; // we only have one - so "first" index of the array
d3d_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 3 channel float (32-bit floats)
d3d_desc[0].InputSlot = 0; 		// comes from the first vertex buffer we're binding
d3d_desc[0].AlignedByteOffset = offsetof(VertexPCU, position);  // how far is position from the start of VertexPCU (0)
d3d_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // this is vertex data
d3d_desc[0].InstanceDataStepRate = 0; // only matters for instance data

// color
d3d_desc[1].name = "COLOR"; // should match the shader input
d3d_desc[1].semanticIndex = 0; // again, first index of "COLOR" semantic (non-array)
d3d_desc[1].Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 4 channels, 8 bits per channel.  Value is a "unsigned normal", ie, treat values 0 to 255 as 0 to 1
d3d_desc[1].InputSlot = 0; 		
d3d_desc[1].AlignedByteOffset = offsetof(VertexPCU, color);  // color has a different offset
d3d_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; 
d3d_desc[1].InstanceDataStepRate = 0; 

// uv
// ??? - left as excercise to the reader
```

`offsetof` is from `<cstddef>`

Once we have a description, we can make an `ID3D11InputLayout`

```cpp
ID3D11InputLayout* Shader::GetOrCreateInputLayout( /*buffer_attribute_t const* attributes*/ )
{
	if (m_layout != nullptr) {
		return m_layout; 
	}

	// fill in the description, like above
	// TODO - make this large enough to match the attributes sent in.  Will matter when
	// we introduce a new vertex format; 
	D3D11_INPUT_ELEMENT_DESC d3dDesc[3]; // we'll describe three elements
	// ...

	// The other half is the shader - this is done for us, be we must keep the
	// byte code for the vertex shader
	void const* vertexByteCode = m_vertexStage.GetByteCode();
	size_t* vertexByteCodeSize = m_vertexStage.GetByteCodeSize(); 

	// create the layout
	ID3D11Device* dev = m_owner->GetD3DDevice();
	dev->CreateInputLayout( d3dDesc, vertexByteCode, vertexByteCodeSize, &m_layout );

	return  m_layout; 
}
```

```cpp
void RenderContext::Draw( int vertCount, int offset )
{
	ID3D11InputLayout* layout = m_currentShader->GetOrCreateInputLayout( /*VertexPCU::LAYOUT*/ );  
	m_deviceContext->IASetInputLayout( layout ); 
	m_deviceContext->Draw( vertCount, offset ); 
}
```

At this point we should be able to see something when compiling with the new shader; 

------

### `buffer_attribute_t`
If you want to go platform agnostic, I suggest a struct like this;

```cpp
enum eBufferFormatType
{
	// add types as you need them - for now, we can get by with just this
	BUFFER_FORMAT_VEC2, 				
	BUFFER_FORMAT_VEC3, 
	BUFFER_FORMAT_R8G8B8A8_UNORM,
};

struct buffer_attribute_t
{
	std::string name; 			// used to link to a D3D11 shader
	// uint location; 			// used to link to a GL/Vulkan shader 
	eBufferFormatType type;		// what data are we describing
	uint offset; 				// where is it relative to the start of a vertex

	buffer_attribute_t( char const* n, eBufferFormatType t, uint offset o ) { IMPLEMENT_ME(); }
	buffer_attribute_t() { IMPLEMENT_ME(); }
};
```

We can then define our vertex layout with the vertex itself by doing this;

```cpp
struct VertexPCU
{
	Vec3 position;
	RGBA8 color;
	Vec2 uv; 

	static buffer_attribute_t LAYOUT[]; 
};

// cpp side
//------------------------------------------------------------------------
STATIC buffer_attribute_t const VertexPCU::LAYOUT[] = {
   buffer_attribute_t( "POSITION",  BUFER_FORMAT_VEC3,      		offsetof(VertexPCU, position) ), 
   buffer_attribute_t( "COLOR",     BUFER_FORMAT_R8G8B8A8_UNORM, 	offsetof(VertexPCU, color) ), 
   buffer_attribute_t( "UV",        BUFER_FORMAT_VEC2,      		offsetof(VertexPCU, uv) ), 
   buffer_attribute_t() // end - terminator element; 
};
```

Now, you can have it create a layout by converting this to a `D3D11` description, making it easy to add
new formats in the future.

Places to update;

1. Add a `LAYOUT` to your vertex type
2. When `Shader::GetOrCreateInputLayout` is called, pass in your vertex type layout to it. 
3. Store off a pointer to the attribute list last used to create an input layout.  If they match, just return the old layout.
4. If they don't match, destroy the old one and recreate a new one. 
   - D3D will cache input layouts, so don't worry about creating your own cache.




