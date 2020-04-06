void RenderContext::DrawMesh( GPUMesh* mesh ) 
{
   // bind a bad stride
   // 24 byte stride
   // 36 byte
   BindVertexBuffer( mesh->GetVertexBuffer(), mesh->GetVertexLayout() ); 

   if (mesh->GetUsesIndices()) {
      Draw( 0, vertexCount ); 
   } else {
      BindIndexBuffer( mesh->GetIndexBuffer() ); 
      DrawIndexed( 0, indexCount ); 
   }
}

// This is a "identity" TBN, or "tangent - bitangent - normal" space.
// tangent -> (1, 0, 0)
// bitangent -> (0, 1, 0)
// normal -> (0, 0, 1)


// normal to color
vec3 color = (normal + vec3(1.0f)) * 0.5f; 

(0, 0, 1) -> (.5, .5, 1)