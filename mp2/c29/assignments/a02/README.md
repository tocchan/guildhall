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

GameObject::~GameObject()
{
   // delete m_rigidbody; // this should be an error if setup right
   m_rigidbody->Destroy(); // destroys through the system;  
   m_rigidbody = nullptr; 
}
```

```cpp
void Game::Startup()
{
	m_physics = new Physics2D(); 
}

Game::Shutdown()
{
   delete m_physics; 
}

void Game::Update()
{
   // input handling
   // ...

   // GameObject* m_selectedObject

	// give the physics system a time slice
	m_physics->Update(); 
}


void Game::Render()
{
   // render all your colliders as determined by the rules of the assignment
   // should be fairly similar to your last MP1 assignment I'm told; 
   // TODO for assignment
}

void Game::EndFrame()
{
   m_physics->EndFrame(); // clean up destroyed objects
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

   float radius = RandomFloatInRange( 0.5f, 4.0f ); // pick a sane range for your scale
   DiscCollider* collider = m_physics->CreateDiscCollider( vec2::ZERO, radius );
	go->m_rigidbody->TakeCollider( collider ); 

	return go; 
}
```



------

## Checklist
- [ ] *10pts*: Insantiate a physic system into the game.
- [ ] *10pts*: Be able to create game objects with a collider based on button press,
    - [ ] `1` should create random radius disc centered at cursor
- [ ] *10pts*: Can move around using `WASD` or equivalent (if different, make note in readme)
- [ ] *10pts*: Can zoom in and out using mouse-wheel.
    - [ ] Make sure this is clamped at a max and min "zoom" so zoom can't go negative.
- [ ] *05pts*: Pressing 'O' (capitol letter 'Oh') should reset camera position and zoom to default (ie, move to origin)
- [ ] *05pts*: Be sure to render the objects in your scene, by default...
    - [ ] Render with a blue border
    - [ ] Render with 50% alpha white interior
- [ ] *10pts* If mouse is inside the object, render the border yellow.
- [ ] *15pts* Be able to drag them around
    - [ ] If the mouse is dragging the object, render the border dark green.
- [ ] *10pts* If the object is touching another object, render the interior 50% alpha red.
    - [ ] Disc-vs-Disc collision should be same as MP1
- [ ] *10pts* If dragging, and `delete` or `backspace` is pressed, destroy object.
- [ ] *05pts* Press `Escape` to quit.
- [ ] Add `mp2.a02.md` readme to root folder


## Resources

### Notes
- [Physics System Structure](./physics_structure.md)
- [Mouse Input](./mouse_input.md)
- [Mouse Dragging](./mouse_drag.md)


