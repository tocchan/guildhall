Physics System Layout
======

## Goal
We're going to make a simple physics system, mimicing the layout of existing systems, such as Box2D or Havok.

This involves having the the following concepts; 

- **PhysicsSystem**:  A single simulation, containing our rigid bodies and colliders.  
- **Rigidbody2D**:  A single point representing the center of mass of a solid object.  This is the object that moves and has forces applied to it. 
- **Collider2D**:  A shape attached to a rigid body, used for detecting and correcting collisions. 
- **Joint2D**:  An object connecting two rigid bodies or to a fixed point in space.  

To start, we'll create our initial system to get something that feels like the following from game side; 

```cpp
//----------------------------------------------------
// SomeGameObject
//----------------------------------------------------
// Some object in our game that is affected by physics
//----------------------------------------------------
class SomeGameObject
{
	public:
		SomeGameObject()
		{
			// setup the physics for htis; 
			PhysicsSystem *physics = Game::GetPhysicsSystem(); 

			// First, give it a rigidbody to represent itself in the physics system
			m_rigidbody = physics->create_rigidbody( /*mass*/ 1.0f );
			m_rigidbody->set_simulation_mode( PHYSICS_SIM_DYNAMIC ); 

			// give it a shape
			m_collider = m_rigidbody->set_collider( new AABBCollider2D(vec2::ZERO, vec2(1.0f, 1.0f)) );  

			// give it a for the physics system to affect our object
			m_rigidbody->set_object( this, &m_transform ); 
		}

	public:
		transform2 m_transform; 
		Rigidbody2D *m_rigidbody;
		Collider2D *m_collider; 
};

//----------------------------------------------------
// Game
//----------------------------------------------------
//----------------------------------------------------
void Game::Startup()
{
	m_physics = new PhysicsSystem(); 
	m_physics->set_gravity( vec2( 0.0f, -9.8f) ); // gravity being more of a global acceleration

	// add some objects to our scene
	add_object( new SomeGameObject() ); 
	add_object( new SomeGameObject() ); 
}

//----------------------------------------------------
void Game::Update()
{
	// let physics system play out
	float delta_time = TimeGetFrameDelta(); 
	m_physics->update(dt); 

	// destroy objects not currently visible; 
	aabb2 bounds = m_camera->get_bounds_2d(); 
	foreach (SomeGameObject *obj in m_objects) {
		if (obj is outside bounds) {
			destroy_object( obj ); 
		}
	}
}

//----------------------------------------------------
void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext(); 

	m_camera->get_color_target( nullptr ); 
	m_camera->set_orthographic_projection( 40.0f ); 

	ctx->begin_camera( m_camera );
	ctx->clear_color_targets( rgba::BLACK );  

	// display debug information
	m_physics->debug_render( ctx ); 

	// overwrite the selected object with a white draw; 
	if (m_selected_object != nullptr) {
		m_selected_object->m_rigidbody->debug_render( ctx, rgba::WHITE ); 
	}

	ctx->end_camera( m_camera ); 
}
```


## Physics Objects
The above rough out will give us the following classes;

```cpp
class PhysicsSystem; 
class Rigidbody2D; 
class Collider2D; 
class AABBCollider2D; // inherits from Collider2D
class DiscCollider2D; // inherits from Collider2D
```

First, let's start with `PhysicsSystem` since it is to physics what `RenderContext` is to rendering. 

*Note: These will not be complete implementations.  You may find you need additional members for book-keeping.*

### PhysicsSystem

```cpp
class PhysicsSystem 
{
	friend class Rigidbody2D; 

	public:
		// recommend doing the factory pattern
		// gives us options for optimizing memory later
		// as well as automatically being able to add it to the system
		Rigidbody2D* create_rigidbody(); 
		void destroy_rigidbody( Rigidbody2D *rb ); 

		void BeginFrame()
		{
			// reset per frame stuff; 
			// copy all transforms over; 
		}

		void PreRender()
		{
			// figure out movement, apply to actual game object;

			// whatever else you may want to do end frame; 
		}

		void update( float dt ) 
		{
			BeginFrame(); 
			// debug: clear all frame information
			// such as if they are currently touching another object; 

			// we'll eventually switch to fixed-step updates, so we'll call down immediately to a run_step to make
			// that port easier; 
			run_step( dt );

			PreRender();  
		}

		void debug_render( RenderContext *ctx ); 

	private: 
		// would also prefer a factory pattern here
		// but we'll let the rigidbody handle making them 
		// until we get more into templates;  We just want
		// these so the rigidbody can call them; 
		void add_collider( Collider2D *col ); 
		void remove_collider( Collider2D *col ); 
	
		// A single step of the physics simulation; 		
		void run_step( float dt ) 
		{
			// move all objects
			foreach (rigidbody) {
				rigidbody->move( dt ); 
			}

			// correct all objects
			// TODO - for now just tell me if they're touching; 
			foreach (collider) {
				foreach (other collider) {
					if (collider->is_touching(other_collider)) {
						collider->set_has_collision(true); 
						other_collider->set_has_collision(true); 
					}
				}
			}
		}

	public:
		// way to store all rigid bodies
		// way to store all colliders

		// system info, like gravity; 
}; 
```

### Rigidbody2D
```cpp
enum eSimulationType
{
	PHYSICS_SIM_STATIC,	 // does not move
	PHYSICS_SIM_DYNAMIC, // does move 
}; 

class Rigidbody2D
{
	public:
		// apply a single step of movement to this object
		void move( float dt )
		{
			// calculate acceleration; 
			vec2 acc = m_system->get_gravity() * m_gravity_scale; 

			// next step - apply to velocity
			m_velocity += acc * dt; 
			m_transform.position += m_velocity * dt; 
		}

		// show it on screen;
		void debug_render( RenderContext *ctx, rgba const &color ); 

	public:
		PhysicsSystem *m_system; 			// system this rigidbody belongs to; 
		void *m_object; 					// user (game) pointer for external use
		transform2 *m_object_transform; 	// what does this rigidbody affect

		transform2 m_transform; // rigidbody transform (mimics the object at start of frame, and used to tell the change to object at end of frame)
		vec2 m_gravity_scale;	// how much are we affected by gravity
		vec2 m_velocity; 
		float m_mass;  			// how heavy am I

		Collider2D *m_collider; // my shape; (could eventually be made a set)

};
```

### Collider2D 
```cpp
enum eColliderType2D 
{
	COLLIDER_AABB2, 
	COLLIDER_DISC,

	// more to come;
	// COLLIDER_CAPSULE, 
	// COLLIDER_BOX,   
};

class Collider2D
{
	public:
		bool is_touching( Collider2D *other )
		{
			collision2d collision; 
			return GetCollisionInfo( &collision, this, other ); 
		}

	public:
		Rigidbody2D *m_rigidbody;   // rigidbody I belong to
		eColliderType2D m_type; 	 
};

class AABBCollider2D : public Collider2D  
{
	public:
		aabb2 get_local_shape(); // what is my shape relative to my rigidbody
		aabb2 get_world_shape()
		{
			aabb2 shape = get_local_shape(); 
			shape.translate_by( m_rigidbody->get_position() ); 
			return shape; 	
		} 

	public:
		aabb2 m_local_shape; 
};

class DiscCollider2D : public Collider2D { /*...*/ }


```

### Collision Matrix 


```cpp
struct collision2d
{
	// for now, nothing, 
	// but next week will contain information 
	// for correction; 
};

//------------------------------------------------------------------------
// Doing this by lookup - as it is a good intro to callbacks
// but could also do this by double dispatch:  a->collides_with( b )
bool GetCollisionInfo( collision2d *out, Collider2D * a, Collider2D *b )
{
   // 2D arrays are [Y][X] remember
   static collision2d_check_cb COLLISION_LOOKUP_TABLE[COLLIDER2D_COUNT][COLLIDER2D_COUNT] = {
      /*******| aabb2 | disc  | point | capsl | line  | obb2  */
      /*aabb2*/ CheckAABB2ByAABB2, CheckAABB2ByDisc, nullptr,           nullptr, nullptr, nullptr, 
      /*disc */ CheckDiscByAABB2,  CheckDiscByDisc,  nullptr,           nullptr, nullptr, nullptr,
      /*point*/ nullptr,           nullptr,          nullptr,           nullptr, nullptr, nullptr,
      /*capsl*/ nullptr,           nullptr,          nullptr,           nullptr, nullptr, nullptr, 
      /*line */ nullptr,           nullptr,          nullptr,           nullptr, nullptr, nullptr, 
      /*obb2 */ nullptr,           nullptr,          nullptr,           nullptr, nullptr, nullptr, 
   }; 

   uint a_type = a->get_type(); 
   uint b_type = b->get_type(); 
   ASSERT_RETURN_VALUE( (a_type < COLLIDER2D_COUNT) && (b_type < COLLIDER2D_COUNT), false ); 

   collision2d_check_cb cb = COLLISION_LOOKUP_TABLE[a_type][b_type]; 
   if (cb == nullptr) {
      return false; // no known collision; 
   } else {
      out->me = a; 
      out->them = b; 
      return cb( out, a, b ); 
   }
}
```

#### Double Dispatch Method
**DON'T DO THIS**

```cpp
class Base 
{
	virtual bool get_collision( Base *other ) = 0; 

	virtual bool collides_with( A* ); 
	virtual bool collides_with( B* ); 
};

class A : public Base
{
	virtual bool get_collision( Base *other )
	{
		other->collides_with(this); 	
	}
};

class B : public Base
{
	virtual bool get_collision( Base *other ) = 0; 
};


void Foo()
{
	A *a = new A(); 
	B *b = new B(); 

	a->get_collision( other_a ); 
	a->get_collision( b ); 
}
```