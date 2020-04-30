Layers 
======

Layers are a way to categorize objects.  

- Each object/collider can only be part of a single layer.
- Layers can be set to either interact or not interact with another layer (including itself)
- Collisions and Triggers are only detected between layers that are allowed to interact. 

## Implementation
Layers can be done as either a string or a number.  Even if using string based layers
we usually map those strings to a number (0-31) so that we can fast and simple bit math 
to do our layer checks. 

When an object is created, it should be set to the default layer (layer 0). 

All layers will interact with each other by default, so usual setup will be for the 
user to manually disable layer interactions they don't want. 

```cpp
void Rigidbody2D::SetLayer( uint layerIndex ); 
uint Rigidbody2D::GetLayer() const; 

void Physics2D::EnableLayerInteraction( uint layerIdx0, uint layerIdx1 ); 
bool Physics2D::DoLayersInteract( uint layerIdx0, uint layerIdx1 ); 
```

### Collision Matrix
We specifically choose values 0-31 so we can use a bitfield representation for our `DoLayersInteract` check.

We will store an array of 32 uints representing which layers each layerIdx can collider with, each set to a default of `0xffff'ffff`.

When checking if a layer interacts with another, we fetch his uint from the array, and then check if the bit of the second layer is set.  

```cpp
bool Physics2D::DoLayersInteract( uint layerIdx0, uint layerIdx1 ) const
{
   return (mLayerInteractions[layerIdx0] & (1 << layerIdx1) != 0); 
}
```

Note, when you set that layers interact, be sure to set it both ways.  The redundant stores make checking a little faster.  Big fan of this tradeoff since checking is a much more common action then enabling/disabling layers. 