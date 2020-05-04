`PiecewiseCurve` / `AnimationCurve`
======

So, the `PiecewiseCurve` or `AnimationCurve` (Unity name) is a cornerstone of animation systems.  But it can be useful
for any problem where you want to model a non-trivial function `f(t)` 

- Animation
  - Skeletal
  - Sprite
  - UI tweens
  - Material Constants
  - Fades
- XP or Growth Curves for an RPG
- Loot Drop Rates
- Decay rates for power-ups
- etc...

At the most basic, we'll be talking about a `FloatCurve`, and given time, talking about how
to generalize to a generic curve.

This builds off Squirrel's lecture on curves from 0 to 1, as the implementation works exclusively on that range.  More 
on that in interpolation modes. 


## Usage

To demonstrate the usage, we'll define a fairly rigid animation in code, and then use it
for having a button appear from off screen, "bounce" once, and then come to rest. 

This will use the most basic of blending and only work for a single float (height) for now.  Concentrate
most on the usage.  If the curve changes, `Update` doesn't need to, leaving the animation to be a data
problem, not a code problem.

(meaning eventually, you probably want to be able to define these curves through data...)

```cpp
class UI
{
   vec3 m_position; 
   FloatCurve m_heightCurve; 

   void Init() 
   {
      // assume our item is 32 units high, so we'll start off the screen
      m_heightCurve.AddKey( 0.0f, m_screenHeight + 32.0f );
      
      // take about 2 seconds to arrive at the center of the screen, moving in a linear fashion
      m_heightCurve.AddKey( 2.0f, m_screenHeight * .5f, INTERPOLATE_LINEAR ); 

      
      // bounce up quickly (linearly) for 0.2f seconds, moving 16 units up from center
      m_heightCurve.AddKey( 2.2f, m_screenHeight * .5f + 16.0f, INTERPOLATE_LINEAR ); 

      // come back to rest at the center of the screen
      m_heightCurve.AddKey( 2.4f, m_screenHeight * 0.5f, INTERPOLATE_LINEAR ); 

      // Tell the curve to clamp when out of range
      m_heightCurve.SetLoopMode( LOOP_MODE_CLAMP ); 
   }

   void Update()
   {
      // get height based on time
      float time = TimeGetSeconds(); 
      float height = m_heightCurve.Evaluate( time ); 

      // set the position
      m_position = vec2( m_screenWidth * 0.5f, height ); 
   }

}
```

------
## Keyframes and Tweens

**DRAW** 

A **Key Frame** is a set value at a given time.  These are usually at points of inflection.

A **Tween**, short for "inbetween frames", is a value computed from its surrounding keyframes, and is usually generated automatically.  

These terms come from classical animation, where when storyboarding you first illustrate the major frames, and then have artists fill in the missing frames once they have the general motion roughed out. 

Sprite animation is strictly keyframe to keyframe (you have a frames of animation at given times, with no blending in-between).  
With 3D animation, you usually interpolate between key frames, generating the **tween frames** yourself.  Though, some games, like Guilty Gear Xrd, somethimes choose to keep with the just showing keyframes as a way to mimic 2D hand-drawn art.  

### Implementation
In our code, a keyframe is going to be a known value at a known time.  So an initial implementation would look like...

```cpp
struct KeyFrame 
{
   float time; 
   float value;

   // float leftTangent;  // this has to do with cubic
   // float rightTangent; 
};

class FloatCurve
{
   public:
      // this mode affects to the LEFT of the keyframe
      void AddKeyFrame( float time, float val, eInterpolationMode = INTERPOLATE_LINEAR )
      {
         // make sure keyframes stay in order of time
      }

      void SetLoopMode( eLoopMode mode ); 

      float Evaluate( float time ) const
      {
         int prevKeyIndex = FindKeyframeIndexForTime( time ); 
         int nextKeyIndex = prevKeyIndex + 1; 

         // do valid boudns checking
         // TODO

         KeyFrame key0 = m_keyframes[prevKeyIndex]; 
         KeyFrame key1 = m_keyframes[nextKeyIndex]; 

         switch (key1.interpolation) {
            case INTERPOLATE_CONSTANT: 
               return key1.value; 

            case INTERPOLATE_LINEAR:
               return RangeMap( time, key0.time, key1.time, key0.value, key1.value ); 

            case INTERPOLATE_CUBIC: 
               // do cubic
               return val; 

         }

      }  

   private: 
      int FindKeyframeIndexForTime( float time ) 
      {
         for (int i = ; i < m_keyframes.size(); ++i) {
            if (i >= m_keyframes.time) {
               return i; 
            }
         }

         return m_keyframes.size(); 
      }

   public: 
      std::vector<KeyFrame> m_keyframes; 
}
```

```cpp
// alternative
struct KeyFrame
{
   float time; 
   float leftTangent; 
   float rightTangent; 
}

class AnimationCurve
{
   std::vector<KeyFrame> m_keyframes; 
};

template <typename T>
class TypedCurve : private AnimationCurve
{
   public:
      T Evaluate( float time ) 
      {
         float animTime = CalculateTimeBasedOnLoopMode( time, m_loopMode, GetStartTime(), GetEndTime() ); 

         // find two keyframes
         // interplate between as if you were going between 0 and 1
         float blend = InterplateBasedOnModeBetweenZeroAndOne(); 
         return Lerp( m_keyValues[prevIndex], m_keyValues[nextIndex], blend ); 
      }

   std::vector<T> m_keyValues; 
};
```

### Cubic  
```cpp
// This interpolate between 0 & 1 and 0 & 1, based on two given velocities
// First derivitive will be v0 at t = 0, and v1 at t = 1
inline float Cubic( float const v0, float const v1, float const t )
{
   float const t2 = t * t;
   float const t3 = t2 * t;
   return (-2.0f * t3 + 3.0f * t2) 
      + (t3 - 2.0f * t2 + t) * v0
      + (t3 - t2) * v1;
}
```


------
## Interpolation Modes

- Constant
- Linear
- Cubic
- Whatever other curve that goes between 0 and 1

### Special Case for Cubic/Bezier

### Generating Smooth Curves


------
## Looping Modes

- Clamp
- Wrap
- PingPong

------
## General Data

------
## Blending 
If doing 3D animation, we may want to blend multiple unrelated animations together.  

- Direct Blending
- Duration Matching
- Skeletal Masks