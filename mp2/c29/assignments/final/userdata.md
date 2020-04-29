User Data
======

The idea here is to be able to attach some arbitrary data to our system pieces, `Rigidbody2D` and/or `Collider2D` (system is usable with one or the other, but both can give users more options). 

Some initial options here;
- `C` style - An arbitrary member pointer for user data.
- `NamedProperties` - Give our rigidbody a `NamedProperties` member so we can set any data we want on it.
- Entity Component System (ECS) Approach (maybe for a DFS)

------
## C-Style
In this approach our object will just have a member like `void* m_userData = nullptr`  that an object can set or retrieve.  The system itself doesn't use it, but during a callback you can use it to get back to the original idea.  

It looks something like this...

```cpp
GameObject* Game::CreateGameObject() 
{
   GameObject* go = new GameObject();
   go->m_rigidbody = m_physics->CreateRigidbody();

   // link the rigidbody to the game object
   go->m_rigidbody->SetUserData( USER_TYPE_GAMEOBJECT, go ); // just a pointer, so casts to a void* natively.

   // finish setting up your object
   // ...

   return go;
}

void Game::OnOverlapStart( Collision2D* col ) 
{
   Rigidbody2D* rb0 = col->GetMyRigidbody();
   Rigidbody2D* rb1 = col->GetTheirRigidbody(); 

   // get the user data off, make sure the cast is safe, and then 
   // apply damage
   GameObject* go = (GameObject*) rbo->GetUserData( USER_TYPE_GAMEOBJECT );
   if (go != nullptr) {
      GameObject* go = (GameObject*) rbo->GetUserData(); // cast should be safe since types matched.
      go->Damage( 10.0f ); 
   }
}
```

In this example, I also set an additional piece of information which is just a `uint` to help make sure the check is safe.  This would be optional, but I find it useful in a system that each rigidbody may be attached for a different type of object...

```cpp
class Rigidbody2D
{
   public:
      void SetUserData( uint type, void* data ); 
      void* GetUserData( uint type ) const { return (type == m_userDataType) ? m_userData : nullptr; }

   public:
      void* m_userData = nullptr;
      uint m_userDataType = 0;   // can be whatever the game needs this to be to help idenfity the pointer since void* has no type.
}
```

I like this approach for the simplicity of implementation and very small overhead.  It is also a very common method of creating a 
user-hook into a system, so most programmers will be familiar with the paradigm. 

Some complaints about this approach are usually to do with safety checks - this doesn't have them.  So you should know that the `void*` you're using is what you think it is.  The `type` helps with that, but again you're relying on people to use the system as intended, and it requires the user to setup an enum for types they want to use with it.  Not a lot of work, but can be a maintenance point.

Another drawback is you can only have one piece of data associated with the object.  In my personal experience, this has never been an issue, but your experiences may vary. 


------
## NamedProperties

This one solves some complaints of the top approach, by just having...

```cpp
class Rigidbody2D
{
   public:  
      NamedProperties m_userProperties; 
}
```

Adding the appropriate getter & setting methods so users don't access the member directly.

This lets you set any data you want on a rigidbody, and it gets you type safety by the nature of `NamedProperties`.  The con is that this object is a lot heavier, so setting or getting data out of it can take a little longer.  

Due to the nature of physics and the callbacks - this should not present to much of a problem (you likely will have less than a dozen a frame at most), but it is something to keep in mind if you start noticing slowdown in your system.  This can be alleviated by spending time optimizing `NamedProperties` though to be more access-efficient, which has the effect of improving other systems that would use it.


------
## Entity Component System
One of the benefits of an ECS is that you get this abstraction naturally due to the nature of components and entities.  In this case, your colliders and rigidbodies just become components attached to some engine style entity.  If they need to get a game-side component it is as easy as asking their parent.

This is the approach you usually see in the major engine these days (Unity and Unreal).

Problem with this approach is, well, you don't have an ECS system yet, so this is not really viable at the moment.  Though if you decide to implement one for a DFS or Thesis topic - you may want to come revisit your physics system and refactor it to take advantage of your new system. 