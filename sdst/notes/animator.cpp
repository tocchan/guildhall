void Entity::Update( float dt ) 
{

	// 
	m_currentAnimation->updateAnimation(dt); 
}


enum eAnimationType 
{
	ANIMATION_IDLE, 
	ANIMATION_WALK, 
	ANIMATION_ATTACK,
	ANIMATION_DIE, 

	ANIMATION_COUNT, 
};

class Entity {


	AnimationDefinition *m_animationSet[ANIMATION_TYPES]; 
	// AnimationDefinition *m_currentAnimation; 
	eAnimationType m_currentState; 
	eAnimationType m_prevState; 

}; 

class Animator
{
	public:
		void Update( float dt )
		{
			// dilate dt if you want...

			// Add dt
			m_totalElapsedTime += dt; 

			// Apply loop mode; 
			m_totalElapsedTime = ApplyLoopMode( m_totalElapsedTime, 
				m_currentAnimation->m_totalTime, 
				m_loopMode ); 
		}

		SpriteDefinition* GetCurrentFrame() const 
		{
			m_currentAnimation.GetFrame( m_totalElapsedTime ); 
		}

		void Play( AnimationDefinition *defn, eLoopMode mode ); 
		void Play( std::string const &name, eLoopMode mode );

	private:
		std::map<std::string,AnimationDefinition*> m_animSet; 
		AnimationDefinition *m_currentAnimation; 
		float m_totalElapsedTime;

}