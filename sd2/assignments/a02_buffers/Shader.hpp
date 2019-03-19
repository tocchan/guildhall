//------------------------------------------------------------------------
// Shader
//------------------------------------------------------------------------

//------------------------------------------------------------------------
class Shader
{
   public:
      // !! DON'T FORGET -> update deconstructor to free newly added inputLayout

      // old stuff...
      //...

      // **NEW STUFF**
      // we'll hard code this for now, and replace it with a more
      // general purpose one later; 
      bool CreateInputLayoutForVertexPCU(); 

   public:
      ShaderStage m_vertexStage;
      ShaderStage m_fragmentStage; 
      
      ID3D11InputLayout *m_inputLayout       = nullptr; 


}; 

//------------------------------------------------------------------------
// Shader.cpp
//------------------------------------------------------------------------


//------------------------------------------------------------------------
bool Shader::CreateInputLayoutForBufferLayout( BufferLayout const *layout )
{
   // Early out - we've already created it; 
   // TODO: If vertex type changes, we need to rebind; 
   if (m_inputLayout != nullptr) {
      return true; 
   }

   // This describes the input data to the shader
   // The INPUT_ELEMENT_DESC describes each element of the structure; 
   // Since we have POSITION, COLOR, UV, we need three descriptions; 
   D3D11_INPUT_ELEMENT_DESC input_description[3]; 

   // initialize meomry to 0 (usually a sane default)
   memset( input_description, 0, sizeof(input_description) ); 

   // Map Position
   input_description[0].SemanticName = "POSITION";             // __semantic__ name we gave this input -> float3 pos : POSITION; 
   input_description[0].SemanticIndex = 0;                     // Semantics that share a name (or are large) are spread over multiple indices (matrix4x4s are four floats for instance)
   input_description[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // Type this data is (float3/vec3 - so 3 floats)
   input_description[0].InputSlot = 0U;                        // Input Pipe this comes from (ignored unless doing instanced rendering)
   input_description[0].AlignedByteOffset = offsetof( VertexPCU, position );   // memory offset this data starts (where is position relative to the vertex, 0 in this case)
   input_description[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;   // What is this data for (per vertex data is the only one we'll be dealing with in this class)
   input_description[0].InstanceDataStepRate = 0U;             // If this were instance data - how often do we step it (0 for vertex data)

   // YOU DO -> Map UV  
   input_description[1].SemanticName = "TEXCOORD";             // __semantic__ name we gave this input -> float3 pos : POSITION; 
   input_description[1].SemanticIndex = 0;                     // Semantics that share a name (or are large) are spread over multiple indices (matrix4x4s are four floats for instance)
   input_description[1].Format = DXGI_FORMAT_R32G32_FLOAT;     // Type this data is (float3/vec3 - so 3 floats)
   input_description[1].InputSlot = 0U;                        // Input Pipe this comes from (ignored unless doing instanced rendering)
   input_description[1].AlignedByteOffset = offsetof( VertexPCU, uv );   // memory offset this data starts (where is position relative to the vertex, 0 in this case)
   input_description[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;   // What is this data for (per vertex data is the only one we'll be dealing with in this class)
   input_description[1].InstanceDataStepRate = 0U;             // If this were instance data - how often do we step it (0 for vertex data)

   // YOU DO -> Map Color
   // ...

   ID3D10Blob *vs_bytecode = m_vertexStage.GetByteCode(); 

   // Final create the layout
   HRESULT hr = gD3DDevice->CreateInputLayout( input_description, 
      ARRAYSIZE(input_description),
      vs_bytecode->GetBufferPointer(), 
      vs_bytecode->GetBufferSize(), 
      &m_inputLayout );   

   return SUCCEEDED(hr); 
}