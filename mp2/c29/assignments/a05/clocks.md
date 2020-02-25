Clocks && Timers
======

## `Clock`
A clock is jsut a way to track time.

Clock relation to each other.

- Master Clock
- UI Clock
- DevConsole Clock
- Game Clock
- Network Clock
- Physics Clock
- etc...

A clock can be paused, dilated, sped up, etc.  Clocks also update automatically at the beginning of the frame, 
so a system doesn't have to have delta time passed to it - it will have delta time as part of its clock. 

### Updating

```cpp
void Clock::Update( double dt ) 
{
   dt = TransformBasedOnOptions( dt ); // scale and paused may affect how much time passes for this clock

   // update my closk concept of time
   frameTime = dt; 
   totalTime += dt; 

   // after transform
   foreach (childClock in children) {
      childClock->Update(dt); 
   }
}
```

### Destroying

If a clock is removed from the tree - we want to maintain a valid structure, so like removing a
node from any other tree sturcture be sure to...

- Give all my children to my parent (if no parent (root clock) just assign you chidlren to become root clocks)
  - This would result in those clocks not updating, but that is fine.  Usually this is happening at shutdown anyway.
- If I have a parent, Remove myself from my parent (I should have no children at this point). 

------

## `Timer`
A timer, also commonly referred to an `Interval` or `Stopwatch`, is a way to track eitehr how long something has been running, or how long until something is done.  

- Activity Timer
- Level Timer
- Power Timer
- Cursor Blink Interval
- Flipbook Interval
- ...

### Implementing A Timer
- Class Thought Excercise -


### Pausing a Timer
- todo -