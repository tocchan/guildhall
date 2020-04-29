Collision Events
======

When collisions happen, we want to let the game be able to respond to them.  *This can be tricky*, as the response to an event can be destructive or change the objects in question, and your system has to be able to handle this.  Also, two of the events require
some frame to frame knowledge (frame in this case being a fixed step).  This may require you to keep a history of all collisions around from frame to frame in some form. 

The events we're going to care about are...
- `OnOverlapStart` - triggered when two objects are touching on a frame that were not touching on before.
- `OnOverlapStay` - triggered on a frame in which the objects are touching and they were touching the frame before.
- `OnOverLapLeave` - triggered on frames in which objects are not touching, but they were touching the frame before.

Try to maintain that if you ever call `OnOverlapStart`, it will be guaranteed that an `OnOverlapLeave` will be called.  So if an object gets destroyed, be sure it properly **leaves** all its currently known contacts.

Another need of this system is that multiple people may want to subscribe to this event, so a simple callback for each will not do.

------
## Implementation

We currently have two tools at our disposal for implementing this.

- `Delegates` - The pub-sub (publisher-subscriber) object we created this semster.
- `Global Event System` - This is the system you developed in SD1.  

Both of these can solve our problem, and which one you use is up to you.


### If Using Delegates
Each rigidbody and/or collider will have three delegates, one for each event listed above.

Users can then just subscribe directly on the object to get the event, and the event has a very fixed signature (likely a `Collision2D` object). 

When firing an event, you will want to fire it once from each side... ie...
- `A->OnOverlapStart(collision);`
- `B->OnOverlapStart(inverse_collision);` 

Another useful delegate in case you want to do something on ANY collision is just adding a global some delegates directly to the `Physics2D` object itself (which can be useful for debugging).


### If Using Global Event System
In this case, you'll likely have some predefined strings for the events in question, and trigger them at the proper points in your system. 

The arguments passed will be passed through a `NamedProperties` object, so would recommend defining some keys people can use to get access to the data they want.  

In this case, you can fire the event only once since the user's event will have to determine how to handle it anyway from context.  

Another option is you make unique events by combining the object (rigidbody) name with the event name, such as "RB0.OnOverlapStart" so users can have per-object events.  For this to work you must be able to give your objects a unique name (either auto-generated or user defined).  I would also recommend firing the event twice like before.

This approach does have the added overhead of using a `NamedProperties`, but again this should not really register since collisions per frame will likely be low.  And again, you can also improve `NamedProperties` later and get knock-on effects in other systems. 

Your GA Danny Durio I believe did this approach in his final DFS, and may be a good resource for more detailed implementation info. 


### Keeping Track of Collisions
When detecting collisions, you will now need to track them frame to frame.  Since A-v-B and B-v-A should be considered the same collision, I recommend giving your colliders a unique ID (for example, their index into the rigidbody array, or just a number that is incremented each time a rigidbody is created (confirming it is not already in use). 

Either way, once they have a unique index, you can create a unique key for the collision using the two ids, for example...

```cpp
IntVec2 colliderID = IntVec2( Min(obj0_id, obj1_id), Max(obj0_id, obj1_id) ); 
```

Idea here is we always put the minimum id first so it is unique for the two cases.  We can then insert this into a list or map that we can keep around and check against next frame.

Again, the hope is that collisions happen fairly rarely, so checking this list should end up being quick (even if you use a `std::vector`).  Again, if this ends up being slow, it is a good area for future work.

When storing collision, keep track of the frame it happened on (frame here being the fixed step), you can do this by just adding another `uint m_frameIndex` to your physics system.  

When you update a collision in the list, also update the frame it happened on to the current frame.  That way you can just loop through it at the end of your collision detection step and remove all events that did not happen this frame. 