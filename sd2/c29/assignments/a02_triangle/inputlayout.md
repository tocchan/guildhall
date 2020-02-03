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
