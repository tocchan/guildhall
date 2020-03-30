struct VertexMaster 
{
   vec3 position;
   vec2 uv;
   rgba color;

   vec3 normal;

   // other things?
};



void AppendQuadToArray( std::vector<VertexMaster> &out, ... )
{
   // lots of code
}


void ConvertMasterToPCU( std::vector<VertexPCU>& output, std::vector<VertexMaster> const& input )
{
   foreach (VertexMaster const& v : input) {
      output.push_back( VertexPCU(v) ); 
   }
}

void AppendQuadToArray( std::vector<VertexPCU> &out, ... ) 
{
   AppendQuadToArray( master_verts ); 
   ConvertMasterToPCU( quad, master_verts ); 
}


void LoadOBJFile( std::vector<VertexLit>& out, char const* filename ); 




// Other options for vertex types
// multiple uv channels 
// VertexFont -> vec2 position, vec2 uv, color, glyph_offset, 

// VertexLit & VetexPCU, VertexFont
// Skeletal Animation or Vetex Paintings are usually DFS