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

void RenderContext::UpdateLayoutIfNeeded()
{
   // buffer_attribute_t const* m_currentLayout; 
   // buffer_attribute_t const* m_previouslyBoundLayout;

   if ((m_previouslyBoundLayout != m_currentLayout) 
      || (m_shaderHasChanged))  {

      ID3D11InputLayout* layout = m_currentShader->GetOrCreateInputLayout( m_currentLayout );
      m_context->IASetInputLayout( layout );

      m_previouslyBoundLayout = m_currentLayout; 
      m_shaderHasChanged = false; 
   }
}


void RenderContext::DrawMesh( GPUMesh* mesh )
{
   BindVertexBuffer( 0, mesh->GetVertexBuffer() );  

   // m_context->IASetInputLayout
   // this needs both a shader and a vertex format
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

std::vector<VertexPCU> m_vertices; 
AppendAABBToVerts( m_vertices, 
   aabb2::FromPoints( vec2(-1.0f), vec2(1.0f) ), 
   aabb2::ZERO_TO_ONE ); 

void Game::Startup()
{
   // GPUMesh* m_meshCube;
   m_meshCube = new GPUMesh(); 

   std::vector<VertexPCU> vertices;
   std::vector<uint> indices; 
   AppendIndexedCubeToVerts( vertices, indices, 
      aabb3(vec3(-1.0f), vec3(1.0f)) ); 

   // MeshUtilAddBounds( m_meshCube, aabb3( vec3(-1.0f), vec3(1.0f) ) ); 
   m_meshCube->UpdateVertices( vertices ); 
   m_meshCube->UpdateIndices( indices );  

}

void GameObject
{
   public:
      // 2D thing
      SpriteDefinition* definition; // equivalent of this, 3d object
      vec2 position;
      float rotation; 

      // 3D equivalent
      GPUMesh* m_mesh; 
      vec3 position; 
      vec3 eulerRotation; 
      
      // 3D equivalent using transform
      GPUMesh* m_mesh;
      transform3 m_transform; 
};

void Game::Render()
{
   // normal render things
   // ...

   mat44 model = m_cubeTransform->ToMatrix(); 

   for (uint i = 0; i < numObjects; ++i) {
      GameObject* go = m_gameObjects[i]; 
      g_theRenderer->SetModelMatrix( go->m_transform.AsMatrix() ); 
      g_theRenderer->DrawMesh( go->m_mesh ); 
   }
}
```




## Removing `VertexBuffer m_immedateVBO`

```cpp
void RenderContext::DrawVertexArray( ... )
{
   // Mesh m_immediateMesh; 
   m_immediateMesh->UpdateVertex( vertexCount, vertices ); 
   m_immediateMesh->UpdateIndices( 0, nullptr );  // ClearIndices()

   DrawMesh( m_immediateMesh ); 
}
```