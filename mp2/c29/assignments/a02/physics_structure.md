## Objects

- `Physics2D`: Our scene object.  Our physical representation of the world;
  - In charge of creating and destroying physics objects
    - ECS - Entity Component System
    - Data Oriented Design
    - Cache Friendliness

- `Rigidbody2D`: Representation of an object in our physics scene.  
  - Can be affected by the physics simulation
  - Have no inherit shape
  - Owns one or more `Collider2D` objects (will only require it owning one in this class)
  - Is the link between the game simulation and the physics simulation

- `Collider2D`:  Represents a shape in our physical simulation.  
  - If attached to a Rigidbody, represents the physical boundaries and properties of that body
  - If not attached to a Rigidbody, it can be...
    - A **Static Object** - a shape does other bodies interact with, but it itself can not be moved. 
    - A **Trigger Object** - a shape that does not interact with anything, but informs us when people enter/leave the boundaries

------

## Factory Pattern
Though we are going to use `new` and `delete` internally for now to create most physics objects, 
we are going to force creation and destruction to happen through the `Physics2D` object.  That is, the `Physics2D` object is a
**Factory** for `Rigidbody2D` and all `Collider2D` objects.  

The reason for this is to allow for a much easier conversion to cache friendly data structures. 



### Preventing Accidental Delete
Make the deconstructor private and use `friend` classes.


--- 

## Transform Hierarchy 

All colliders specify there location in local space - which is **local to the Rigidbody's space**.  Be sure your simulation is updating
only rigidbody locations, and that colliders are calculating their world location using their local space transformed by the owner's world space. 

---

## Header Examples

```cpp
class Physics2D
{
   public:
      void BeginFrame();
      void Update();      // nothing in A01, but eventually it is the update, collision detection, and collision response part
      void EndFrame();    // cleanup destroyed objects

      // factory style ccreate/destroy
      Rigidbody2D* CreateRigidbody(); 
      void DestroyRigidbody( Rigidbody2D* rb ); 

      DiscCollider2D* CreateDiscCollider( vec2 localPosition, float radius ); 
      void DestroyCollider( Collider2D* collider ); 

    public:
      // add members you may need to store these
      // storage for all rigidbodies
      // storage for all colliders
      // ...
};
```

```cpp
class Rigidbody2D
{
   friend class Physics2D;

   public:
      void Destroy();                             // mark self for destruction, and mark collider as destruction

      void TakeCollider( Collider2D* collider );  // takes ownership of a collider (destroying my current one if present)

      void SetPosition( vec2 position );          // update my position, and my colliders world position

   public:
      Physics2D* m_system;     // which scene created/owns this object
      Collider2D* m_collider; 

      vec2 m_worldPosition;     // where in the world is this rigidbody

   private:
      ~Rigidbody2D();           // assert the collider is already null 
};
```

```cpp
enum eCollider2DType
{
   COLLIDER2D_DISC,
};

// Interface for all Collider objects used with our Physics system
class Collider2D
{
   friend class Physics2D;

   public: // Interface 
      // cache off the world shape representation of this object
      // taking into account the owning rigidbody (if no owner, local is world)
      virtual void UpdateWorldShape()                             = 0; 

      // queries 
      virtual vec2 GetClosestPoint( vec2 pos ) const              = 0; 
      virtual bool Contains( vec2 pos ) const                     = 0; 
      virtual bool Intersects( Collider2D const* other ) const    = 0; 


      // debug helpers
      virtual void DebugRender( RenderContext* ctx, rgba const& borderColor, rgba const& fillColor ) = 0; 

   protected: 
      virtual ~Collider2D(); // private - make sure this is virtual so correct deconstructor gets called

   public: // any helpers you want to add
      // ...

   public:  
      eColliderType m_type;                  // keep track of the type - will help with collision later
      Physics2D* m_system;                   // system who created or destr
      Rigidbody2D* m_rigidbody = nullptr;    // owning rigidbody, used for calculating world shape
};
```

```cpp
class DiscCollider2D : public Collider2D
{
   public: // implement the interface of Collider2D
      // A02 TODO
      // ...

   public:
      vec2 m_localPosition; // my local offset from my parent
      vec2 m_worldPosition; // calculated from local position and owning rigidbody if present
      float m_radius; 
}
```