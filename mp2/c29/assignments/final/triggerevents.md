Triggers
======

## Implementation
A trigger in a lot of ways is simpler than what we've done so far.  

First, it is just a collider that can be marked to be a trigger (a simple `bool` will suffice).  If something is a collider, the only thing it cares about is if it is **touching** another collider, but it doesn't need the manifold or contact information.  

If a trigger collider is touching an object, it will trigger the events for that collider. 

Colliders are also ignored when detecting collisions.

------
## Events
The events supported should be...

- `OnTriggerEnter`
- `OnTriggerStay`
- `OnTriggerLeave`

Not much to say on this.  It is implemented identical to collision events, though with a few notable exceptions...

- Triggers don't generate manifolds/contacts.  They're purely a "touching" or "not touching" check.  So the callback takes different data.  I will usually just pass the two colliders in question to the callback.

- I prefer to do this on the collider instead of the rigidbody since usually a trigger volume won't be attached to a rigidbody (not always the case, but one of the primary use of trigger volumes is just to check when a player enters or leaves a zone).