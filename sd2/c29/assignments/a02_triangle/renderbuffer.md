- GPU memory
  - Usage 

  - Memory Types
- Creating the RenderBuffer
- Updating
  - Mapping

VertexBufffer
- buffer_attribute_t
- stride
- InputLayout


### Creating 
To create data is a little more involved then a `malloc` or new call.

GPUs have different types of memory that are good at different things.  And all GPUs
can be built differently, with different manufactures having different options.  D3D11
supplies hints about the memory usage to let the GPU pick the best memory pool(s) to use
for the given access patterns (read; how we use them).  

For our purposes, we'll break this into two categories...

1. `eRenderBufferUsageBit` - What can this memory be used for.  For example, vertices, indices, or globals
2. `eRenderMemoryHint` - How do we interact with it?  One, rarely, often?  
   - `GPU` - Read/Write from GPU only.  Rarely if never updated from CPU.
   - `DYNAMIC` - Memory written to often from the CPU.  **Can not be read from CPU**
   - `STAGING` - Can be read/written to from CPU, and copied from GPU to GPU, but not used in a pipeline. 
     - Used mostly for copying data from GPU to CPU or vice-versa.  If you do screenshots, you'll use this; 

There is also a memory hint in D3D11 called `STATIC`, for things that are read only GPU side.  I'm not going to cover 
it as I have not yet found a equivalent in a more modern API, and it is effectively a more restictive GPU hint. 

Using these two enums, we can allocate some memory; 

```cpp
// BIT_FLAG is a custom macro, sets the bit 
enum eRenderBufferUsageBit : uint
{
   VERTEX_BUFFER_BIT       = BIT_FLAG(0),    // A02: can be used to store vertices
   INDEX_BUFFER_BIT        = BIT_FLAG(1),    // we will discuss this later
   UNIFORM_BUFFER_BIT      = BIT_FLAG(2),    // A03: used to store constants
};
typedef uint eRenderBufferUsage;  

enum eRenderMemoryHint 
{
   RENDER_MEMORY_GPU,      // memory updated and used only from GPU side (if created from CPU, must be initialization data)
   RENDER_MEMORY_DYNAMIC,  // something updated often from CPU side, used by GPU side 
   RENDER_MEMORY_STAGING,  // memory that can be read/written from CPU, and copied to a GPU buffer
};
```

For this, and textures later, we'll need the following conversion funtions; 


```cpp
//------------------------------------------------------------------------
D3D11_USAGE DXUsageFromMemoryUsage( eRenderMemoryHint const hint )
{
   switch (hint) {
      case RENDER_MEMORY_GPU:      return D3D11_USAGE_DEFAULT;
      case RENDER_MEMORY_DYNAMIC:  return D3D11_USAGE_DYNAMIC;
      case RENDER_MEMORY_STAGING:  return D3D11_USAGE_STAGING;
      default: ASSERT_RETURN_VALUE(false, D3D11_USAGE_DYNAMIC); 
   }
}

//------------------------------------------------------------------------
uint DXBindFromBind( eRenderBufferUsage const bind_flags ) 
{
   uint ret = 0U;

   // buffer sage; 
   if (bind_flags & VERTEX_BUFFER_BIT) {
      ret |= D3D11_BIND_VERTEX_BUFFER;
   }
   
   if (bind_flags & INDEX_BUFFER_BIT) {
      ret |= D3D11_BIND_INDEX_BUFFER;
   }
   
   if (bind_flags & UNIFORM_BUFFER_BIT) {
      ret |= D3D11_BIND_CONSTANT_BUFFER;
   }

   // TODO: there are other data types that can be
   // bound to that you may want to add support to in the future
   // but for a majority of people, these will be the only
   // three used

   return ret;
}
```

```cpp
void RenderBuffer::Create( eRenderBufferUsage usage, eRenderMemoryHint memHint, size_t byteSize, size_t elemByteSize )
{
   Cleanup(); // before we create a new one - be sure to destroy the old one

   // static buffers MUST be supplied data.
   ASSERT_RETURN_VALUE( byteSize > 0U, false ); 

   // Describe our allocation
   D3D11_BUFFER_DESC bufferDesc;
   MemZero( &bufferDesc );

   // convert our types to D3D11 types to describe it; 
   bufferDesc.ByteWidth = (UINT) byteSize;
   bufferDesc.StructureByteStride = (UINT) elemByteSize;
   bufferDesc.Usage = DXUsageFromMemoryUsage(memHint);
   bufferDesc.BindFlags = DXBindFromBind(usage);

   // Give hints about how the CPU will work with it.  We will
   // infer these from our memory hint
   bufferDesc.CPUAccessFlags = 0U;
   if (memHint == RENDER_MEMORY_DYNAMIC) {
      bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   } else if (memHint == GPU_MEMORY_STAGING) {
      bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
   }

   bufferDesc.MiscFlags = 0U; // Some buffer usages have some extra options for how they may be used - see docs for more dtails; 
   // only structured buffers are bindable at shader resources - so set that flag.

   // Map initial data if relevant.
   D3D11_SUBRESOURCE_DATA data;
   D3D11_SUBRESOURCE_DATA *data_ptr = nullptr;
   if (ci.initial_data != nullptr) {
      MemZero( &data );
      data.pSysMem = ci.initial_data;
      data_ptr = &data;
   }

   // Create it
   ID3D11Device *dev = m_device->get_handle();
   HRESULT hr = dev->CreateBuffer( &bufferDesc,    // description of our allocation
      nullptr,                                     // initial data (required for `STATIC` or `IMMUTABLE` memory hint) 
      &m_handle );                                 // out variable for our buffer handle

   // error checking
   if (SUCCEEDED(hr)) {
      m_usage = usage;
      m_memoryUsage = memHint; 

      m_totalByteSize = (uint) size; 
      m_elemByteSize = (uint) ci.elem_size; 
      return true;
   } else {
      ASSERT(false); // failed?
      return false;
   }
```

### Updating (Mapping Memory)
This only works for `STAGING` or `DYNAMIC` buffers, as the memory
is **Host Writable** - meaning it can be written to from the CPU (your program). 

```cpp
D3D11_MAPPED_SUBRESOURCE resource;

// first, ask D3D11 to map, this locks the resource so the GPU won't try to use it our data until we unmap. 
HRESULT result = dxContext->Map( m_handle,      // gpu memory we want to map
   0,                                           // which subresource (used for textures with mip-maps or arrays)
   D3D11_MAP_WRITE_DISCARD,                     // what do we do with the memory that was there (discard means throw it away - ie, can't read)
   0U,                                          // What to do if the GPU is busy - by default we'll "block", ie, wait for it to be ready.
   &resource );                                 // out variable, filled with information about the mapping

if (SUCCEEDED(result)) {
   // this gets us a pointer to the GPU memory (resource.pData) we can then just copy into
   // note: memcpy from this would be garbage since we used DISCARD
	memcpy( resource.pData, data, byte_size );

	dx_context->Unmap(m_handle, 0); // we're done, unlock the memory
 	return true;
} else {
   LogWarningf("Failed to map resource." );
   return false;
}
```

### Updating (Full Copy)
In D3D11, you can fully update a piece of `GPU` or `DYNAMIC` memory by calling
`ID3D11DeviceContext::UpdateSubresource`.  Only do this if the sizes perfectly match.  

```cpp
dxContext->UpdateSubresource( m_handle, 0, nullptr, data, 0U, 0U );
```

### `RenderBuffer::Update`
So the psuedo code for your update will look something like the following

```cpp
void RenderBuffer::Update( void const* data, size_t totalByteSize, size_t elemByteSize )
{
   // Destroy old handle if not compatible with current data
   // ...

   // if no handle exists, create a new one given current options
   // ...

   // if dynamic, map and copy in new data
   // ...
   // else update the subresource
   // ...
} 
```

