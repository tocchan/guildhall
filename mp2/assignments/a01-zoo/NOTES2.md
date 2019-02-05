```cpp


class Entity
{
	public:
		Entity() 
		{
			m_rigidbody = g_thePhysics.CreateRigidbody( this, &m_transform ); 
			m_rigidbody->SetCollider( new AABBCollider2D(vec2::ZERO, vec2(20,50)) ); 
			m_rigidbody->BindToTransform( &m_transform ); 
		}

		~Entity()
		{
			delete m_rigidbody;
			m_rigidbody = nullptr; 
		}

	public:
		Transform m_transform; 
		Rigidbody2D *m_rigidbody; 
};

class Rigidbody2D
{
	public:
		~Rigidbody2D()
		{
			DestroyOldCollider(); 
			if (m_system) {
				m_system->UnregisterRigidbody(this); 
			}
		}

		void SetCollider( Collider2D *col ) 
		{
			DestroyOldCollider(); 
			m_collider = col; 
			m_system->RegisterCollider(col); 
		}

		void DestroyOldCollider()
		{
			if (m_collider != nullptr) {
				m_system->UnregisterCollider(m_collider); 
				delete m_collider; 
				m_collider = nullptr;
			} 
		}

		void CopyTransformsFromGame() // BeginPhysicsStep
		{
			if (m_object_transform) {
				m_transform = *m_object_transform; 
			}
		}

		void CopyTransformToGame() 
		{
			if (m_object_transform) {
				*m_object_transform = m_transform; 
			}
		}

	public: 
		Transform2D m_transform; 
		Transform2D *m_object_transform; 
		eSimulationmode m_sim_mode; 
};

class PhysicsBucket 
{
	public:
		std::vector<Rigidbody2D*> m_rigidbodies; 
		std::vector<Collider2D*> m_colliders; 
}

class PhysicsSystem 
{
	public:
		void Update( float dt )
		{
			CopyTransformsFromGame();

			m_accumulatedTime += dt; 
			while (m_accumulatedTime >= m_fixedDelta) {
				RunSingleStep( m_fixedDelta ); 
				m_accumulatedTime -= m_fixedDelta; 
			}

			CopyTransformsToGame(); 
		}

		void RunSingleStep( float dt ) 
		{
			// A01
			// move all rigidbodies
			// check for collisions;

			// A02
			// move all rigidbodies
			// resolve against static
			// resolve against dynamic
			// resolve against static   // Make sure we're not in static geometry; 
		}

		PhysicsBucket m_buckets[PHYSICS_SIM_MODE_COUNT]; 
}; 


class Game::Update
{
	// get input

	// update sim

	// update physics

	// render
}

```