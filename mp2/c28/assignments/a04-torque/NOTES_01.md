```cpp

//------------------------------------------------------------------------------
void HandleCollision( Collision2D *col ) 
{
	manifold2 m = col->get_manifold();

	// correct
	Rigidbody2D *rb0 = col->m_first_object; 
	Rigidbody2D *rb1 = col->m_second_object; 

	float w0 = m1 / (m0 + m1); 
	float w1 = 1.0f - w0; // m0 / (m0 + m1); 

	rb0->move_by( m.normal * m.penetration * w0 ); 
	rb1->move_by( -m.normal * m.penetration * w1 );

	vec2 contact_point = m.contact + m.normal * (m.penetration * w0); 

	// generate impulse along normal
	float impulse_along_normal = CalculateImpulse(...); 

	rb0->ApplyImpulseAt( impulse_along_normal * m.normal, contact_point );
	rb1->ApplyImpulseAt( -impulse_along_normal * m.normal, contact_point );
}

//------------------------------------------------------------------------------
void Rigidbody2D::ApplyImpulses( vec2 linear_impulse, float angular_impulse )
{
	m_velocity += linear_impulse / m_mass;
	m_angular_velocity += angular_impulse / m_moment_of_inertia;  
}

//------------------------------------------------------------------------------
void Rigidbody2D::ApplyImpulseAt( vec2 linear_impulse, vec2 point_of_contact )
{
	vec2 linear = linear_impulse; 

	vec2 displacement = point_of_contact - GetCenterOfMass();  // world location of rigidbody (probably)

	float angular = Cross( linear_impulse, displacement ); 

	ApplyImpulses( linear, angular ); 
}

```