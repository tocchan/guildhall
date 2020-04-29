Triggers
======

## Implementation
...


## Events
Not much to say on this.  It is implemented identical to collision events, though with a few notable exceptions...

- Triggers don't generate manifolds/contacts.  They're purely a "touching" or "not touching" check.  So the callback takes different data.  I will usually just pass the two colliders in question to the callback.

- I prefer to do this on the collider instead of the rigidbody since usually a trigger volume won't be attached to a rigidbody (not always the case, but one of the primary use of trigger volumes is just to check when a player enters or leaves a zone).