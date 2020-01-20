A06 - Collision Events & Trigger Volumes
======

## Overview
The entire goal of this assignment is to give the physics system a way to communicate back to the game systems; We will accomplish this in two ways;

1. Allow colliders to trigger events on collision.
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
   floorCollider->onCollisionEvent = "FloorHit"; // these event strings default to empty - and only fire events if non-empty

   // Create two score regions
   // First will be normal - just tell you when you enter or leave; 
   m_score = m_physics.CreateTrigger(); 
   m_score->SetPosition( vec2( 20.0f, -40.0f ) ); 
   m_score->SetColliderAsBox( obb2( vec2::ZERO, vec2(8.0f), 0.0f ) ); // (local) center, size, rotation 
   m_score->onEnterEvent = "Score"; // these event strings default to empty - and only fire events if non-empty
   m_score->onLeaveEvent = "ScoreLeave";  

   // Second will destroy the other object immediately upon leaving
   // (which will also count as *leaving* - so a leave event would also happen)
   // (we want to maintain enters always match leaves - based on how triggers are commonly used to count)
   m_scoreAndDestroy = m_physics.CreateTrigger(); 
   m_scoreAndDestroy->SetPosition( vec2( -20.0f, -40.0f ) ); 
   m_scoreAndDestroy->SetColliderAsCapsule( capsule2( vec2(-10.0f, 0.0f), vec2(10.0f, 0.0f), 4.0f ) ); // (local) start, end, radius
   m_scoreAndDestroy->onEnterEvent = "ScoreAndDestroy"; // these event strings default to empty - and only fire events if non-empty
   m_scoreAndDestroy->onLeaveEvent = "ScoreLeave";  

   // make our game respond to these events; 
   EventSubscribe( "FloorHit", this, &Game::FloorHit );
   EventSubscribe( "Score", this, &Game::ScoreEnter );
   EventSubscribe( "ScoreLeave", this, &Game::ScoreExit );
   EventSubscribe( "ScoreAndDestroy", this, &Game::ScoreEnterAndDestroy );
}


//------------------------------------------------------------------------
void Game::FloorHit( NamedProperties &props )
{
   // named properties expected - we're not using them, 
   // just here as reference on what you should put in; 
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
void Game::ScoreExit( NamedProperties &props )
{
   Trigger2D *trigger = props.Get<Trigger2D*>( "trigger", nullptr ); // trigger that was hit
   Collider2D *collider = props.Get<Collider2D*>( "collider", nullptr ); // collider entering or leaving
   DebugRenderMessage( 4.0f, rgba::GREEN, "Score Region Left" ); 
}
```

### Demo
- [./build/mp2.a06.zip](./build/mp2.a06.zip)


## Grading 
- [ ] 20%: Physics objects that are destroyed are not cleaned up until the end of a physics step.
- [ ] 30%: `NamedProperties` now uses typed properties so we can store non-string data.
- [ ] 15%: Able to register an event when a collision happens
    - [ ] 05%: The object with the event should always be the "me" - meaning during a collision, you may fire two events, each from the perspective of the object fires the event;
- [ ] 30%: Ability to create `Trigger2D` objects with a collider defining its shape.
    - [ ] Collider fires an `OnEnterEvent` when an object first comes in contact.
    - [ ] Collider fires an `OnExitEvent` when an object is no longer touching (leaves or is destroyed)


### Supporting Layers
**BONUS** - Not really for points, just something extra if you're interested;  This adds layers into the system, so you can have more control on what interacts with what;  We will have support for 32 layers; 

- [ ] By default, all layers interact with all other layers; 
- [ ] `PhysicsSystem` can enable or disable collision between two specific layers
    - [ ] Implement `BitArray` class (or use `std::bitset`, but recommend creating your own for the exercise)
    - [ ] When disabled, objects will not collide or spawn events relative to each other
- [ ] `Trigger2D` objects can specify a `m_layerMask`, that is a bitfield defining which layers can trigger.
    - *Note: Unity just does this using the collision matrix, though I find letting the triggers just allowing themselves to create a layerMask more intuitive - for example, if I want a trigger for all enemy projectiles and ships, it saves me creating a layer for just that special rule.*

## Resources
...


## Source (Headers)
...

## Checklist

I recommmend starting by making sure all collider and rigidbodies can be marked for destruction and then destroyed at the end of a physics step;  This may require you making it so all creation and destruction goes through the `PhysicsSystem2D`.  This may require some refactoring, but will be necessary now that Game code can choose to destroy objects based on events as we're running through our system; 

### Switching to a Factory Pattern
- [ ] Make all `Collider2D` and `Rigidbody2D` constructors private - preventing game code from newing them off; 
- [ ] Add factory-pattern style `CreateRigidbody` and `CreateCollider*` methods on the physics system for creating physics
- [ ] Allow Colliders and Rigidbodies to mark themselves as destroyed
    - [ ] Upon first being destroyed, add themselves to a destroyed list on the owning system.
    - [ ] Do not process collisions with destroyed objects (early out)
- [ ] Purge destroyed objects at the end of a physics step, as well as when the system shuts down; 

### Adding Collision Events
- [ ] Make sure your `NamedProperties` support typed properties instead of just strings
- [ ] Add a `onCollisionEvent` string member to your `Collider2D` object
- [ ] After a collision has been resolved, if an event exists for either of them, first it; 
    - [ ] Be sure to fire the event always from the point of view of the person of the event,
        - [ ] Add a `collision2D::invert` method that swaps `me` with `them`, and inverts the `manifold2`
- [ ] Test!

### Adding Triggers
- [ ] Add a `Trigger2D` class - follow factory pattern rules for creation like above; 
- [ ] Allow triggers to register a collider to define the shape; 
- [ ] Keep track of all **dynamic objects** that are touching the trigger that frame
- [ ] For all new dynamic objects - fire the `onEnterEvent`
- [ ] For all dynamic objects no longer touching (or destroyed) - fire the `onExitEvent`, and remove them from the tracked list
- [ ] Before purging destroyed objects, be sure to walk through again and remove all colliders from the tracked list; 


## Notes

```cpp
PhysicsStep 
   update fixed frame count
   Update Rigidbodies
   Update collisions
   Update Triggers
      Update touching list against dyanmic rigid bodies
      Fire all events
         FireEnterAndLeaveEvents

   Purge
      Foreach Trigger
         FireLeaveEvents

      Delete purged objects
```
