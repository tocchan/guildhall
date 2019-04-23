A06 - Collision Events & Trigger Volumes
======

## Overview
The entire goal of this assignment is to give the physics system a way to communicate back to the game systems; We will accomplish this in two ways;

1. Allow colliders to trigger events on collision, which will be called on both colliders from their point of view.  
2. Allow creation of `Trigger2D` objects that let us know when objects have entered or left a specific region. 

Since the system is now allowing game code to run during the physics step, we have to be more careful about when we destroy objects; 


### Goal 
```cpp
//------------------------------------------------------------------------
void Game::Startup()
{
   // Rigidbody2D *m_floor; 
   // Trigger2D *m_score; 
   // Trigger2D *m_scoreAndDestroy; 

   // create a floor with a hit event;
   m_floor = m_physics.CreateRigidbody(); 
   Collider2D* floorCollider = m_floor->SetColliderAsBox( obb2( vec2::ZERO, vec2(40.0f, 5.0f), 0.0f ) ); 
   floorCollider->onCollision.SubscribeMethod( this, &Game::FloorHit ); // register a hit event; 

   // Create two score regions
   // First will be normal - just tell you when you enter or leave; 
   m_score = m_physics.CreateTrigger(); 
   m_score->SetPosition( vec2( 20.0f, -40.0f ) ); 
   m_score->SetColliderAsBox( obb2( vec2::ZERO, vec2(8.0f), 0.0f ) ); // (local) center, size, rotation 
   m_score->onEnter.SubscribeMethod( this, &Game::ScoreEnter ); 
   m_score->onLeave.SubscribeMethod( this, &Game::ScoreLeave ); 

   // Second will destroy the other object immediately upon leaving
   // (which will also count as *leaving* - so a leave event would also happen)
   // (we want to maintain enters always match leaves - based on how triggers are commonly used to count)
   m_score = m_physics.CreateTrigger(); 
   m_score->SetPosition( vec2( -20.0f, -40.0f ) ); 
   m_score->SetColliderAsCapsule( capsule2( vec2(-10.0f, 0.0f), vec2(10.0f, 0.0f), 4.0f ) ); // (local) start, end, radius
   m_score->onEnter.SubscribeMethod( this, &Game::ScoreEnteAndDestroy ); 
   m_score->onLeave.SubscribeMethod( this, &Game::ScoreLeave ); 
}


//------------------------------------------------------------------------
void Game::FloorHit( NamedProperties &props )
{
   // named properties expected; 
   Collider2D *them = props.Get<Collider2D*>( "them", nullptr ); 
   Collider2D *me = props.Get<Collider2D*>( "me", nullptr ); 
   collision2d col = props.Get<collision2d>( "collision", nullptr ); 


   DebugRenderMessage( 4.0f, rgba::BLUE, "Floor Hit!" ); 
}

//------------------------------------------------------------------------
void Game::ScoreEnter( NamedProperties &props )
{
   // Properties expected; 
   Trigger2D *trigger = props.Get<Trigger2D*>( "trigger", nullptr ); // trigger that was hit
   Collider2D *collider = props.Get<Collider2D*>( "collider", nullptr ); // collider entering or leaving

   DebugRenderMessage( 4.0f, rgba::GREEN, "Score Region Entered" ); 
}


//------------------------------------------------------------------------
void Game::ScoreEnterAndDestroy( NamedProperties &props )
{
   // Properties expected; 
   Trigger2D *trigger = props.Get<Trigger2D*>( "trigger", nullptr ); // trigger that was hit
   Collider2D *collider = props.Get<Collider2D*>( "collider", nullptr ); // collider entering or leaving

   DebugRenderMessage( 4.0f, rgba::GREEN, "Score & Destroy Region Entered" ); 
   collider->Destroy(); // may actually want to get the parent object and destroy that instead; 
}

//------------------------------------------------------------------------
void Game::ScoreLeave( NamedProperties &props )
{
   Trigger2D *trigger = props.Get<Trigger2D*>( "trigger", nullptr ); // trigger that was hit
   Collider2D *collider = props.Get<Collider2D*>( "collider", nullptr ); // collider entering or leaving

   DebugRenderMessage( 4.0f, rgba::GREEN, "Score Region Left" ); 
}
```


## Grading 

## Resources

## Demo

## Source (Headers)

## Checklist


