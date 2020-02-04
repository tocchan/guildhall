```cpp

void Physics2D::Update()
{
	// move the simulation forward by deltaTime
	float deltaTime = GetFrameDeltaTime();
	
	AdvanceSimulation( deltaTime ); 
}

void Physics2D::AdvanceSimulation( float deltaTime )
{
	ApplyEffectors( deltaTime ); 	// apply gravity to all dynamic objects
	MoveRigidbodies( deltaTime ); 	// apply an euler step to all rigidbodies, and reset per-frame data
	// DetectCollisions(); - A04	// determine all pairs of intersecting colliders
	// CollisionResponse(); - A04	// resolve all collisions, firing appropraite events
	CleanupDestroyedObjects();  	// destroy objects 
}

```

## Mass, Force, Acceleration, and Velocity

- Mass = How heavy/dense the object is.  
- Position = Where something is in the world
- Velocity = Change in Position over Time, `m / s`, 
- Acceleration = Change in Velocity over Time, `m / s^2`
- Force = Outside energy being applied to an object `kg*m / s^2`

- Force = Mass * Acceleration;    Meaning to push something that's heavier, I need to apply more force to it.  

- Velocity -vs- Speed:  Velocity implies a direction.  Speed is a scalar unit (length of velocity).  


### Forward Euler Step

An Euler step is doing a discrete integral for our time step.  Forward refer to moving through
in in from acceleration to position.  We'll be talking about different Euler steps later on in class,
but this is a good starting point. 

```c

Vec2 accel = GetFrameAcceleration(); // take into account outside forces and user changes

Vec2 deltaVel = accel * deltaTime; // our change in velocity this for this frame
m_velocity += deltaVel; 

Vec2 deltaPos = velocity * deltaTime; // out change in position this frame
m_position += deltaPos; 

```


## Gravity & Effectors
An `Effector` we will define as some sort of outside force that applies to some or all objects in our world.

Examples would be; 
- Gravity
- Black Hole
- Wind
- Magnets
- etc...


## Applying Force -vs- Acceleration

