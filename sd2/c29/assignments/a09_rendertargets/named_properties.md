`NamedProperties`
======

## Goals

- Should have an identical interface to `NamedStrings` so we can sub it in for it.
- Can store any type of data in it (pointers and basic data types)
- Should be able to be data driven or code driven
  - Meaning if I set a `float` by string, getting it as a "float" should work, even if it is stored as a string.

------
## Examples

### From XML
This is more or less similar to what you have now.  You can 
construct a NamedStrings 
```xml
<actors>
   <actor id="thief"
          name="Thief"
          hp="4~8"
          attack="2~3"
          faction="Thieve's Guild" />
   <!-- ... -->
</actors>
```

But before we couuld have to do something like...

```cpp
void UseArgs( NamedProperties& args )
{
   std::string hp_range = args.get("hp"); 

   // pasre it, I use a function called `FromString` that I assume 
   // we wrote, that takes the default value as a second parameter
   FloatRange range = GetValue(hp_range, FloatRange(1.0f, 10.0f));  

   float actual_hp = hp_range.roll_inclusive(); 
   my_actor->set_hp( actual_hp, actual_hp );  // set current and max

}
```

But what we're going for is something like this...

```cpp
void UseArgs( NamedProperties& args )
{
   // the encompasses the cast automatically, and if it fails, will give us a default
   // value.
   FloatRange range = args.get_as( "hp", FloatRange(1.0f, 10.0f) ); 

   float actual_hp = range.roll_inclusive();
   my_actor->set_hp( actual_hp, actual_hp ); 
}

```

So when using this from data - we remove some boilerplate code to cast our data.  Even easier with things that 
end up being simple data like floats or ints. 


------
### CPP Use Case
Say we have a button that can start the game using the event `LoadLevel`.  We'll hook this 
up to a button driven by code and the dev console, driven by user text.

We will show two, one for using it with our existing dev console, we it is purely string based...
```cpp
void DevConsole::OnSubmit( char const* raw_line ) 
{
   std::string line = raw_line;
   ConsoleHistoryAdd( line ); 

   std::string command = ChopBeforeFirst( line, ' ' );  // command will be all text before first space, line will be all text after
   Trim( command ); 

   if (!EventExists(command)) {
      ConsoleErrorf( "Unknown command: %s", command.c_str() ); 
   } else {
      NamedProperties args( line ); // key0=value0 key1=value1 key2="string value" 
      FireEvent( command, args ); 
   }
}
```

So a command like
`"LoadLevel map=level_intro save_slot=3 gamepad_index=7"`

...or...

```cpp
void Button::OnClick()
{
   NamedProperties args; 
   args.set( "source", this ); 
   args.set( "map", "level_intro" ); 
   args.set( "save_slot", 2 );
   args.set( "gamepad_index", 4 ); 

   FireEvent( "LoadLevel", args ); 
}
```

Now, we can have an event...

```cpp
void OnLoadLevel( NamedProperties& args )
{
   int save_slot = args.GetValue<int>( "save_slot", 0 ); 
   int gamepad_index = args.GetValue<int>( "gamepad_index", 0 ); 
   std::string level = args.get( "map" ); 
   
   Button* btn = args.get_as<Button*>( "source", nullptr );
   if (btn != nullptr) {
      btn->SetLabelText( "LOADING..." ); 
   }

   g_theGame->LoadLevel( level, save_slot, gamepad_index ); 
}
```

Couple things to point out; 
- Whether it was set from string or value, i was able to get it as that value.
- I was able to get the source button (a pointer type), which would not be possible with just strings.  If it succeeds,
  I can then set text to to something else.  If it came from the `DevConsole` I could ignore it.


------
### CPP Header
What we had before...

```cpp
class NamedStrings
{
  ...
}
```

... and what we'll end up with...

```cpp
typedef NamedProperties EventArgs;
``` 


```cpp
class NamedProperties
{
      public:
         // do this in class - to make it match SD1 interface for NamedStrings
         // ...
};
```


## Implementation

### `TypedProperty`

Common Base
- Map -vs- Vector
  - Does it matter?

Determining Type
  - Options to Talk Through
    - RTTI (Run-Time Type Information) <- `dynamic_cast` requires this!
    - Virtual Method
    - Global Type ID (templates and statics!)
      - Want to use this one as lots of good knowledge is involved
    - ... maybe more, not sure... don't know myself...

### RTTI
```cpp
TypedProperty<T>* prop = dynamic_cast<TypedProperty<T>*>( base ); 
if (prop != nullptr) {
```

### Virtual
```cpp
if (prop->GetUniqueID() == TypedProperty<T>::UNIQUE_ID) {
  // legal - does not support inheritance, but whatever...
}
```

Does not work for inherited objects, for example;

```cpp
class Actor 
{
  ...
};

class Player : public Actor
{

};

void Game::Foo()
{
  NamedProperties args;
  // Player* m_player; 
  args.SetValue<Player*>( "target", m_player );

  // would be nullptr in this case, as
  // "Actor" is not a "Player" exactly
  // where a dynamic cast could catch this; 
  Actor* target = args.GetValue<Actor*>( "target", nullptr ); 
}




Specializing for Pointers

Duck Typing

Proper Cleanup
