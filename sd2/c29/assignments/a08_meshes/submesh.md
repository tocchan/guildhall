```cpp

class Mesh
{
   public:
      VertexBuffer* m_vbo; // data, layout   
      IndexBuffer* m_ibo;  // data
}

```

--> becomes -->

```cpp
class SubMesh
{
   void update_vertices( void const* data, BufferLayout* vertex_layout, uint vcount, size_t total_bytes )
   {
      // whatever mesh USED to do
   }

   void update_indices(...)
   {
      // whatever mesh USED to do
   }

  public:
      VertexBuffer* m_vbo; // data, layout   
      IndexBuffer* m_ibo;  // data
};

class Mesh
{
   public:
      void update_vertices( void const* data, BufferLayout* vertex_layout, uint vcount, size_t total_bytes )
      {
         // make sure I have at least ONE submesh
         if (get_submesh_count() == 0) {
            add_submesh(); 
         }

         m_submeshes[0].update_vertices( data, vertex_layout, vcount, total_bytes ); 
      }

      void update_indcies(...) 
      {
         // siimilar to update vertices
         // ...
      }

      // new thing
      uint get_submesh_count() const; 
      SubMesh* add_submesh(); 


   public:
      std::vector<SubMesh*> m_submeshes; 
};



Mesh* CreateMeshFromVertexArray( std::vector<VertexLit> const& vertices, std::vector<uint> submeshOffsets )
{
   Mesh* mesh = new Mesh(); 
   uint submeshCount = submeshOffsets.size(); 
   for (uint i = 0; i < submeshCount; ++i) {
      uint startIndex = submeshOffsets[i]; 

      uint vertsInSubmesh = (i == (submeshCount - 1))
         ? vertices.size(0 - startIndex)
         : submeshOffsets[i + 1] - startIndex;  

      SubMesh* submesh = mesh->add_submesh(); 
      submesh->update_vertices( &vertices[startIndex], layout for VertexLit, vertsInSubmesh, sizeof(VertexLit) * vertsInSubmesh );
   }
}

```

## Drawing!
```cpp
void RenderContext::DrawMesh( SubMesh* mesh )
{
   // what used to be DrawMesh goes here
   // ... 
}

void RenderContext::DrawMesh( Mesh* mesh ) 
{
   DrawMesh( mesh->get_submesh(0) ); 
}

void RenderContext::DrawMesh( Mesh* mesh, Material** materialsPerSubmesh, uint materialCount )
{
   for (uint submeshIndex = 0; submeshIndex < submeshCount; ++submeshIndex) {
      BindMaterial( materialsPerSubmesh[submeshIndex] ); 
      Draw( mesh->get_submesh( submeshIndex ) ); 
   }
}
```