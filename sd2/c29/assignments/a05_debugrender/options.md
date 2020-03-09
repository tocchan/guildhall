
- Has to be a singleton style (only one system)
- Can call from anywhere
- Don't need to pass an object aroudn
  - So **no** `g_theDebugRenderer`

```cpp
class DebugRender
{
   static void DrawPoint(...); 
}
```