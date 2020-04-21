
## Intended Use
A `Delegate` is similar to a function or callback.  Users can register to be told about when something happens.  The main 
difference being is that delegates are a **PubSub** or **Publisher-Subscriber** model, meaning that multiple people can 
register to a single event to be notified.

This gives us some design wiggle room.  Some personal choices I make when creating a `Delegate` style class 
(called an `Event` in my own engine). 

- I don't allow a `Delegate` to return a value.  My design allows for purely a one way relationship.  The publisher 
  can tell me about things, but I can't tell them anything back.  
  - Don't have to worry about what to do if multiple people return different results
  - Makes the class signature easier
  - That said, there's nothing stopping me from passing an out variable forward that people can change.
- I allow priority, so I can control the order in which subscribers get their notification
- I allow a subscriber to unsubscribe anytime, even in the middle of getting a notification. 

At the end of the day, I want code that looks like this;

### The Subscriber Side
```cpp

// `Contact2D` tells me about the two objects touching
// `Collision2D` is usually a contact with additional information ab

class Physics2D
{
   // ....
   public:
      Delegate<float> on_fixed_update;             // called once for every step of the physics system
};

class Rigidbody2D
{
   public:
      // collision events
      Delegate<Collision2D const&> on_overlap_start; // called on frames a contact happens, but it wasn't their the frame before
      Delegate<Collision2D const&> on_overlap_stay;  // called on frames a contact happens and it happened the frame before
      Delegate<Collision2D const&> on_overlap_stop;  // called on frames where no contact happened but we had one the frame before
      Delegate<Collision2D const&> on_collision;   // called after collision has been resolved this fixed step
};

//------------------------------------------------------------------------
// Game.cpp
//------------------------------------------------------------------------
Bullet* Game::CreateBullet( vec2 pos, vec2 vel ) 
{
   // constructor would set the appropriate layers
   // create the rigidbody and collider
   // etc...
   Bullet* bullet = new Bullet(); 

   // change my rigidbody
   bullet->set_position( pos ); 
   bullet->set_velocity( vel ); 

   // be told when something happens
   bullet->m_rigidbody->on_overlap_start.subscribe( OnBulletHit ); 

   return bullet; 
}

// What gets called
void OnBulletHit( Collision2D const& col ) 
{
   // Tip:  Setup the collision for the callback so "me" is always the
   // person who subscribed.  It's a little more work for the system, but makes it behave
   // in a more predicatable way for the user
   Rigidbody2D* me = col.me->get_rigidbody2d(); 
   Rigidbody2D* them = col.them->get_rigidbody2d(); 

   // giving user data an id - just some random uint.  If it matches, it will return a pointer, if it doesn't, I return nullptr
   // if you system gets more features to figuring out types - may want to update this... 
   Bullet* bul = (Bullet*) me->get_user_data( DATA_TYPE_BULLET ); 
   GameObject* obj = (GameObject*) them->get_user_data( DATA_TYPE_GAMEOBJECT ); 

   if ((bul != nullptr) && (obj != nullptr)) {
      bul->destroy();                          // will disable the rigidbody - causing the collision to never resolve
                                               // if I wanted this to happen AFTER the collision resolved I could subscribe to `on_collision`
      obj->damage( 1.0f, -col.get_normal() );  // damage, pass normal in case I want to spawn an effect or apply an impact
   }
}
```

### The Publisher Side
```cpp
void Physics2D::fixed_update( flaot dt )
{
   // if no one subscribes, no problem
   // if people do, then they will get notified at this point
   // (no need to check nullptr with our delegate as a design choice)
   on_fixed_update( dt ); 

   detect_collision(); 

   // before resolving collisions... do pre-step
   foreach (Collision2D* col : m_frame_collisions) {
      Collision2D inv_col = col.get_inverse();  // like calling callback from pov of caller - personal preference
      if (col was new) {
         col->me->m_rigidody->on_overlap_start(*col); 
         col->them->m_rigidbody->on_overlap_start(inv_col); 
      } else if (col is not old) {
         // call `on_overlap_stay`
         // ...
      }
   }

   // foreach collision was that old or invalidated (either object was disabled/destroyed)
   // call `on_overlap_stop` and remove from this frames collisions
   // ...

   // resolve collisions
   resolve_collisions(); // be sure not to process collisions that were possibly removed due to the event

   // call the collision callbacks
   foreach (Collision2D* col : m_frame_collisions) {
      // on_collision(*col); // global collision callback?  I don't particularly find it useful... but nothing stopping you from adding it

      // TODO: Collider2D events - I find it both useful to
      // have collision events on both the rigidbody and collider

      // personally I like calling collisions events from point of view 
      // of the caller, so I'll inverse the collision for the other guy
      Collision2D inv_col = col.get_inverse(); 
      col->me->m_rigidbody->on_collision(*col); 
      col->them->m_rigidbody->on_collision(inv_col); 
   }
}
```


There is probably some edge cases I'm missing up top - but you should get the gist.  

This system has one more "promise" that I think it should make - not having to do with `Delegate`s exactly, but more the expected behaviour 
of the system.  Try to maintain that if an object ever called `on_overlap_start` that it will be guaranteed to call `on_overlap_stop`.  This 
helps make the system more predictable for the user at some slight headaches to the writer.  

---

## Implementation
I'm going to use quite a few more modern C++ tools for this to keep code to a minimum.  This is going to involve...
- Operator Overloading
- Templates
- Variadic Templates
- Lambdas
- Method Pointers


So, from intended use, I know I want...

- ...to call it like a function
- ...to take arbitrary argument types
- ...to subscribe and unsubscribe

So this is a good use case for templates - or more specifically, a new'ish concept called variadic templates.

An initial (not final) implementation would look like this; 

```cpp
// the ...ARGS may be 0 or more arguments of type `typename` - allowing us to use this
// with any number of argument types
template <typename ...ARGS>
class Delegate
{
   public:
      // typedef the callback type to save on typing later
      using function_t = std::function<void(ARGS...)>;  // expan the ARGS out to be the signature for the function type

   public:
      void subscribe( function_t const& func_to_call ); 
      void unsubscribe( function_t const& func_to_call ); 
      void unsubscribe_all(); 

      void invoke( ARGS& ...args ) const; // will call all subscribers
      void operator() (ARGS& ...args) const  { return invoke(args...); } // allows us to use this object as a functor

   public:
      std::vector<function_t> m_subscribers; 
}
```

### Immediate Problems

If this only allowed users to subscribe, we have it pretty easy.  We just add to  `function_t`, adding to it
on subscribe.  Trigger would then just loop over this invoking each one - easy!

But...
- ...how do we unsubscribe?  
- ...what happens if we unsubscribe WHILE invoking?

First, how do we **unsubscribe**.  At initial glance this probably wouldn't seem too bad, until you run into the problem that 
`std::function` is not comparable.  There are good reason for this (std::function works on all callable types, including functors, 
not all callable types are comparable).  So by adding this functionality we will be limiting to a specific sub-group
of callable objects (C functions & methods).  Add more as you need them!  

This is going to change our signature slightly so we can access this;  First, I need something to identify a subscription by,
and to be a little forward thinking, I want this to also work for methods.  


```cpp
// the ...ARGS may be 0 or more arguments of type `typename` - allowing us to use this
// with any number of argument types
template <typename ...ARGS>
class Delegate
{
   public:
      using c_callback_t = void (*)(ARGS...);            // signature of a standalone C function that works with this Delegate
      using function_t = std::function<void(ARGS...)>;   // callable object to store it
      struct sub_t {
         void const* obj_id;           // object I'm calling on
         void const* func_id;          // address of function I'm calling
         function_t callback;    // callable object o call above function/method

         inline bool operator==( sub_t const& other ) const { return (obj_id == other.obj_id) && (func_id == other.func_id); }
      }; 
      // typedef the callback type to save on typing later

   public:
      void subscribe( c_callback_t const& cb );
      void unsubscribe( c_callback_t const& cb );

      void unsubscribe_all(); 

      void invoke( ARGS& ...args ) const; // will call all subscribers
      void operator() (ARGS& ...args) const  { return invoke(args...); } // allows us to use this object as a functor

   private:
      // moving this private as no one will use these directly
      void subscribe( sub_t const& func_to_call ); 
      void unsubscribe( sub_t const& func_to_call ); 

   public:
      std::vector<sub_t> m_subscribers; 
}
```

So, first, I need to solve the comparable problem.  For `C` functions, they are easily comparable as they're just a memory address,
and methods are going to be similar, with the added identifier of the object they're called on.  So I am
going to add a `sub_t` subscription type to have these idenfitiers; 

I make the `sub_t` comparable by adding a comparison operator to it.  This should be familiar from SD1. 

I move the main subscribe/unsubscribe methods to private, as they are no longer meant to be used, but can now 
be fully implemented. These should be fairly straight foward to implement and are left as excercises to the reader.

Invoke is also fairly simple (though will get more complicated - see _Unsubscribing during an invoke_ below).

That leaves us to look at `void subscribe( c_callback_t const& cb )`. 

```cpp
// template implementation has to be done in the header as the code doesn't
// actually exist until someone invokes it, and they fully need to know how to 
// create their implementation so it can't be hidden.  Some people
// prefer creating *.inl* or *.inc* files that the header can include
// so they can at least move it to a different file.  For now, I'd suggest just
// implementing at the bottom of the file.
//
// Notice the full class name when declaring...
template <typename ...ARGS>
void Delegate<ARGS...>::subscribe( c_callback_t const& cb )
{
   sub_t sub; 

   // give it identifying information
   sub.obj_id = nullptr;            // no object
   sub.func_id = (void const*) &cb; // function we're calling has a unique address
                                    // (fun trivia - cb == &cb == &&cb == *cb == **cb == ***cb.  Function pointers get weird rules)
   sub.callback = cb;               // actually be able to call it!

   subscribe( sub );                // call to our common subscribe
}

template <typename ...ARGS>
void Delegate<ARGS...>::unsubscribe( c_callback_t const& cb ) 
{
   // create a sub with just the identifying information...
   sub_t sub;
   sub.obj_id = nullptr;
   sub.func_id = (void const*) &cb; 
   // no need to copy the actually callback - we won't be using it

   // remove it from the list
   unsubscribe( sub ); 
}
```

---

### Supporting Methods

Now to give us some real utility - the ability to subscribe a method.

Use would look this this; 

```cpp
void Game::on_button_clicked( Button* src ) 
{
   // Do something cool when they click the button
   ...
}

void Game::setup_ui()
{
   Button* btn = m_canvas->create_button(); 
   btn->on_click.subscribe_method( this, &Game::on_button_clicked );
}
```

First, notice that a method needs to pieces of data - both the object it will call on, and the method pointer itself.  In the above
example, the object is a `Game`, and the method is `Game::on_button_clicked`. 

So to use method callbacks, this looks like...

```cpp
// this is all a "type" btw, named method_ptr, that is
// pointing to a method on game, that returns void, and takes a Button*.
void (Game::*method_ptr)( Button* btn ) = &Game::on_button_clicked; 

// get the pointer to the object you want to call it on...
Game* game_ptr = g_theGame;
Button* some_btn = ...; // assume we have a button pointer for this example; 

// will call g_theGame->on_button_clicked
(game_ptr->*method)( some_btn ); 
```

In short, method callbacks are kinda clunky.  Luckily we'll be hiding it behind our delegate for the most part - we just need to 
know how it works for the implementation.  

```cpp
template <typename ARGS...>
class Delegate
{
   // ...

   public:
      // TEMPLATES WITHIN TEMPLATES!  MADNESS!
      template <typename OBJ_TYPE>
      void subscribe_method( 
         OBJ_TYPE* obj,          // object we're calling it on
         void (OBJ_TYPE::*mcb)(ARGS...) 
      ); 

      // ...
};
```

Okay, so, how is this implemented - now it is time for where old-school C stubborness meets modern'ish C++. 

```cpp
template <typename ...ARGS>   // template for the class
template <typename OBJ_TYPE>  // template for the method
void Delegate<ARGS...>::subscribe_method( OBJ_TYPE* obj, void (OBJ_TYPE::*mcb)(ARGS...) )
{
   sUb_t sub; 
   sub.obj_id = obj;             // easy, pointer is pointer
   sub.func_id = *(void**) &mcb; // compiler is going to try to disuade you hard from using the memory address of the method
                                 // reason being it is not safe to use due to how methods pointers can be implemented
                                 // but as identifying information - it'll be fiiiiiine.  

   // now, how to get this into a function.  Couple options...
   // 1. std::bind  (boo, boring!)
   // 2. LAMBDA!!!! (omg so easy)

   // So lambda - I'm going to do a capture by value for my two
   // pointer types, and allow the arguments to be passed through.
   sub.callback = [=](ARGS ...args) { (obj->*mcb)( args... ); }; 

   subscribe( sub ); 
}
```

Now, similar for unsubscribe - though **remember that unsubscribe does not need the callback, just the identifying information**.
So get the ids, and unsubscribe the thing.  


---

## Unsubscribing During a Callback
Now, if someone unsubscribes during an invoke, you may currupt the list.  Plus they may not necessarily unsubscribe themselves
(say a game object subscribes to an event that kills itself, like a bullet on collision.  If that bullet had a particle system that 
generated on collision too, the object may unsubcribe him as well on death.  Both of which are happening during invoke).  

The approach I use for this is similar to marking objects for destruction from SD1.  I don't remove 
subscriptions from the list immediately, but instead will just mark them for removal.  Then, during the next invoke (or possibly
the same one), I'll either purge an item or call its callbcak depending if it has been marked for removal. 

## Thread Safety
This class as written is meant for single-threaded use.  Just be aware of this that when threading is added to your engine
any cross-thread events will want to use a different class, such as an `AsyncDelegate`.  

The overhead to make it thread safe is non-trivial, so I like having both options in my engine depending on intended use.  

## Priority
Another feature I use in my own engine is the ability to give a subscription a priority or order identifier.  
The lower the value, the sooner it will run in the list (hence I use the term `order` in own engine instead of priority,
as priority would make me think the opposite).  

Not much needs to be changed.  Just when subscribing, provide a priority, and be sure to insert into the list
the new subscription at the correct place in the list depending on priority.  If priorities are equal, I assume the 
later the object is added the later it will be invoked. 