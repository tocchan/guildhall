MP2.A02 `Physics2D::Init`
======

## Overview
Get started writing our physics engine.  We're going to get the basic classes stubbed out,
and the start of the proejct

**Turn-In Branch Name: `mp2/turnin/a02`**

**Due Date: Feb 06, 2020**

### Goal 
Some example code for how the system will be used in this assignment;

```cpp
// Assume we have some game object
class GameObject
{
	public:
		Rigidbody2D* m_rigidbody; 
};
```

```cpp
void Game::Startup()
{
	m_physics = new Physics2D(); 
}

void Game::Update()
{
	// give the physics system a time slice
	m_physics->Update(); 
}

void Game::Render()
{
	// render the physics scene using the passed in camera
	// to configure location/projection options.
	g_Physics->DebugRenderScene( m_gameCamera ); 
}

```

Creating a object should look similar to the following...

```cpp
GameObject* Game::CreateDisc()
{
	// get the mouse position in world position
	vec2 mousePos = g_theInput->GetClientMousePosition();
	mousePos = m_gameCamera->ClientToWorldPosition(mousePos); 

	GameObject* go = new GameObject();
	go->m_rigidbody = m_physics->CreateRigidbody();
	go->m_rigidbody->SetPosition( mousePos );  
	go->m_rigidbody->AddCollider( new DiscCollider(vec2(0.0f), radius) ); 

	return go; 
}
```



------

## Checklist
- [ ] Insantiate a physic system into the game.
- [ ] Be able to create discs/polygons
    - [ ] `1` should create random radius disc centered at cursor
    - [ ] `2` should create random polygon centered at cursor with between 3 and 8 points.
- [ ] Can move around using `WASD` or equivalent (if different, make note in readme)
- [ ] Can zoom in and out using mouse-wheel.
- [ ] Be able to select to create a polygon or sphere
- [ ] Render the polygon or sphere
    - [ ] Render with a blue border
    - [ ] Render with 50% alpha white interior
- [ ] Be able to drag them around
    - [ ] If mouse is inside the object, render the border yellow
    - [ ] IF the mouse is dragging the object, rener the border green
- [ ] If the object is touching another object, render the interior 50% alpha red.
    - [ ] Disc-vs-Disc
    - [ ] Disc-vs-Polygon
    - **Polygon-vs-Polygon is not required... yet.**
- [ ] If dragging, and `delete` is pressed, destroy object.
- [ ] Press `Escape` to quit.
- [ ] Add `mp2.a02.md` readme to root folder


## Resources

### Notes
- [Mouse Input](./mouse_input.md)
- Physics System
  - [Structure](./physics_structure.md)
  - [Collider Matrix](./physics_collider.md)
- Polygon
  - [Contains Point](./polygon_contains.md)
  - [Get Closest Point](./polygon_closest.md)

### Headers
These can be put in `Engine/Physics2D`

- [Physics2D.hpp](./Physics2D.hpp)
- [Rigidbody2D.hpp](./Rigidbody2D.hpp)
- [Collider2D.hpp](./Collider2D.hpp)
- [DiscCollider.hpp](./DiscCollider.hpp)
- [PolyCollider.hpp](./PolygonCollider.hpp)