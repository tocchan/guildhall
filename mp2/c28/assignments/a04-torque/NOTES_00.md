## Outline

- Zen Topic: Math in Games
- Contact Points (surface)
- Angular Movement
  - Angular Velocity
  - Rotation 
  - Torque
    - Moment of Inertia
      - Point Particle
      - Sphere (Cylindar)
      - Box
      - Inertia Tensor (3D)  
    - Parallel Axis Theroem
- Impulse
  - An impulse is a force acting over a very short period of time, or can be thought of as "an immediate change in velocity"
  - Restructuring impact in terms of impulse forces
  - `Rigidbody2D::ApplyImpulse`  
  - Impulses and Torque



## Opinion: Math in Games

## Contact Points

### Rounded OBB vs Rounded OBB
This is the generic case - if you can solve here, you can solve all cases needed for the assignment.  The more specialized cases though (especially disc vs ---) can be faster, but would recommend saving it until you have everything working; 

This is just a slight continuation of existing code.  When calculating feature points (nearest point to the seperating axis), you just need to keep track of which point you used; 

The tricky case is edge-vs-edge detection.  Using one of the points will give you an experience with torque that feels unatural (either too fast, or too slow).  So what we want to the entire contact edge.  You can return the line-segment, or the center of the segment (depends on how you want to use it).  Either way, for the physics simulation, we only need the center point; 


## Angular Movement
We want objects to be able to rotate around their center of mass (which we'll use the rigidbody's location as the "center").  The first few concepts - angular velocity and angular acceleration are straight-forward extensions of  

We will start by making sure our rigidbody supports a `rotation`, either a single angle, a complex number representation, or a 2D basis.  I recommend the float. 

### Angular Velocity

`Rotation += AngularVelocity * deltaTime;`

*Note: be sure to keep rotation between 0 and 360 degrees if using degrees.  Hence you're adding cyclic values.*
*Note: modf will give bad results for negative numbers as they will return `Sign(val) * (Abs(val) % modulo)`.  Recomend writing a custom `Mod` function that does the mathematically correct thing;* 


### Angular Acceleration

`AngularVelocity += AngularAcceleration * deltaTime`


### Torque

Is the equivalent of Force in angular speak;

`F = ma`

`Torque = MomentOfInertia * AngularAcceleration`

=>

`AngularAcceleration = Torque / MomentOfInertia`

```cpp

class Rigidbody2D
{
	public:
		void AddForce( vec2 force ) { m_frame_forces += force; }
		void AddTorque( float torque ) { m_frame_torque += torque; }

		void update( float deltaTime ) 
		{
			// one euler step for linear
			vec2 acceleration = m_frame_forces / m_mass; 
			m_velocity += acceleration * deltaTime; 
			m_position += m_velocity * deltaTime;

			// angular
			float angularAcceleration = m_frame_torque / m_moment; 
			m_angular_velocity += angularAcceleration * deltaTime; 
			m_rotation += m_angular_velocity * deltaTime; 
		}

		

	public:
		vec2 m_frame_forces; 
		float m_frame_torque; 

		float m_mass; 		// float m_inverse_mass; 
		float m_moment; // of inertia

		vec2 m_velocity; 
		float m_angular_velocity; 
};
```

## Impulses

So, once things rotate - the rest is surprisingly straight forward, in that we just have to update how we handle impacts slightly; 

First, our effective velocity at point of impact now is influenced by angular velocity, and the local distance of impact from the center of mass;  

`velocityDueToRotation = r * angularVelocity`

...where `r` is the distance from local origin.  The direction is tangental to the displacement, hence the `-rotate90()` used below;

Second, we'll going to resolve collision in terms of impulse, and then apply that impulse to both objects.  An **impulse** is an instantaneous change in velocity, or it can be thought of as applying a constant force for a second instantly.  

The impulse we calculate is called a **Normal Impulse**, as it is the impulse along the impact normal.  See links with assignment for how to calculate this; 

Once calculated, it is applied to both objects (postive to A, negative to B - due to every action having an equal but opposite reaction).  

Linear Impulse is applied directly to the object as before to calculate change in linear velocity; 

Angular Impulse is applied along the tangent to the displacement from center, scaled by the distance (giving us leverage).  This can calculated shorthand by using the 2D Cross Product, but will put the long way in code below; 

```cpp
class Rigidbody2D
{
	public:
		// ...
		vec2 GetImpactVelocity( vec2 worldPos ) const
		{
			vec2 displacement = GetPosition() - worldPos; 

			/*
			float distance = displacement.GetMagnitude(); 
			vec2 dir = displacement.GetNormalized)(; 
			vec2 tan = -dir.GetRotate90();

			vec2 velocityFromRotation = tan * distance * m_angular_velocity; 
			*/

			return GetVelocity() - displacmenet.GetRotate90() * m_angular_velocity; 
		}

		void ApplyImpulse( vec2 impulse, vec2 worldPos )
		{
			// apply linear impulse
			m_velocity += impulse / m_mass; 

			// apply angular impulse
			vec2 localImpact = GetPosition() - worldPos; 
			vec2 directionOfTorque = -localImpact.GetRotate90(); 
			float impulseTorque = Dot( impulse, directionOfTorque ); 

			// float impulseTorque = Determinant( directionOfTorque, impulse ); 
			m_angular_velocity += impulseOfTorque / m_moment; 
		}
		//...
}
```