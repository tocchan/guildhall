## `Index Buffer`

Just a render buffer, that contains indices.   Indices can be `unsigned shorts` or `unsigned ints`.  We are just going to assume `unsigned int`.

1. Make a render buffer
   - Make sure it can bind as an index buffer: `D3D11_BIND_INDEX_BUFFER`
2. Update same way as UBO or Vertex Buffer
3. To use it...
   a. Bind it: `ID3D11RenderContext::IASetIndexBuffer`
   b. Make sure the vertex buffer it is referring to is bound too. 
   - https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
   b. Draw using it: `ID3D11DeviceContext::DrawIndexed`
      - Wrap with `RenderContext::DrawIndexed`
      - `DrawIndexed( 6, 0, 0 );` - For drawing a quad with 6 unique indices, 4 unique vertices


## `GPUMesh`

This just wraps a vertex buffer and an index, and I can just draw it; 

See [`GPUMesh.hpp`](./GPUMesh.hpp) for details; 

```cpp

void RenderContext::DrawMesh( GPUMesh* mesh )
{
   BindVertexBuffer( 0, mesh->GetVertexBuffer() );  
   UpdateLayoutIfNeeded(); // based on current vertex buffer and current shader

   bool hasIndices = mesh->GetIndexCount() > 0; 

   if (hasIndices) {
      BindIndexBuffer( mesh->GetIndexBuffer() ); 
      DrawIndexed( mesh->GetIndexCount(), 0, 0 ); 
   } else {
      Draw( mesh->GetVertexCount(), 0, 0 ); 
   }
}

```

### Optional - Update `DrawVertexArray` and add `DrawIndexedArray`

You can convert your `m_immediateVBO` to an `m_immediateMesh`


## For A04!

Your cube is a `GPUMesh`

```cpp

void Game::Startup()
{
   // GPUMesh* m_meshCube;
   m_meshCube = new GPUMesh(); 
   m_meshCube->AddVertices( 24, cubeVerts ); 
   m_meshCube->AddIndices( 36, indices );  
}

void Game::Render()
{
   // normal render things
   // ...

   mat44 model = m_cubeTransform->ToMatrix(); 
   g_theRenderer->SetModelMatrix( model ); 
   g_theRenderer->DrawMesh( m_meshCube ); 
}
```