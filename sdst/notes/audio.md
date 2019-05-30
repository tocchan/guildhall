FMOD Core API Notes
======

## FMOD 3D Audio Link
- FMOD Whitepaper on 3D Audio: https://fmod.com/resources/documentation-api?version=2.0&page=white-papers-3d-sounds.html

## Overview
To get 3D Audio working you need to do three things
1. Load audio as a 3D resource
2. Set a listener in the scene
3. Configuring the 3D-Channel (`FMOD::Channel`)

All of this is going to require you have a `FMOD::System`, which I'll refer to as `g_fmod` below; 

## Loading
When loading audio using `createSound` - if you want it to be 3D, you just have to set the correct options.  In the case of
3D audio, just use the flag `FMOD_3D`.

Other load options of note;
- `FMOD_CREATESTREAM`:  Load the sound as a stream, keeping just enough of the audio in memory to play it.  Improves loading time and has a lower memory limit, but will cause more hits to the physical media.
- `FMOD_LOOP_NORMAL`: Resource is setup for looping and will loop cleaner.  Without this, some audio may *pop* when looping back around.


```cpp
uint options = 0U; 
if (is_3d) {
   options |= FMOD_3D; // if you want to use it as 3D - you want to load it this way,
                       // though technically you can change after load, but the documentaiton
                       // recommends against it; 
}

if (is_looping) {
   options |= FMOD_LOOP_NORMAL;  
}

if (is_streaming) {
   options |= FMOD_CREATESTREAM; 
}

FMOD::Sound *sound = nullptr; 
g_fmod->createSound( src_filename, options, nullptr, &sound ); 

// sound is now the resource - save or store somewhere - be sure to release when done; 
```


## Setting Listener
For any 3D audio to work, we need to define the **listener** in the scene.  This is the microphone in the room that hears the sound, and is used
to determine how to play the sounds around you, including panning and volume changes.  

Important methods here are;

- `g_fmod->set3DNumListeners` - *Default 1*.  This is the number of listeners present in the scene.  YOu usually will not need to change this,
  but it can be useful for a split screen game.  Documentation states that sounds will play from the listener which results in the highest volume. 
- `g_fmod->set3DListenerAttributes` - Sets the position and orientation of the listener.  By default it uses a Left-Handed coordinate system with x-right and y-up.  
- `g_fmod->set3DSettings` - Configuration for the 3D audio - most notably the scale of your world.  By defualt they assume a 1-meter per unit scale, but
  if you were in say, Unreal, which uses 1-centimeter per unit, you would want to add a distance factor of `100`.  You will not need to touch this method
  for the RTS though.  

Where you set your listener is important to the sound design of the game.  If you're making an FPS - you may want to set it at the camera (you are the player).  If you're making a 2D game, you may want to set the listener on the character instead (to give positional hints on where enemies are).  



## Setting 3D Audio Position
The last step for getting 3D audio to work is to also set the audio clip (`FMOD::Channel`) position in the world, as well as falloff controls, when playing the sound; 

To note - I would recommend starting a sound paused, setting the 3D information, and then resuming the sound.  This can resolve some audio bugs where the audio thread picks up the sound before the 3D settings take effect, resulting in an audible pop as you get a split second of the sound being played in 2D; 

```cpp
// playing a sound (code you already have) - started paused
FMOD::Channel *channel; 
g_fmod->playSound( sound, nullptr /*mixer*/, true /*paused*/, &channel ); 

// the below only work IF the sound is set to be a 3D sound; 

// now, given a channel, we can set the position of the audio
// vec3 pos; // used for positional audio - set to world position of sound
// vec3 vel; // used for doppler effects - feel free to default to 0 for now; 
channel->set3DAttributes( (FMOD_VECTOR*)&pos, (FMOD_VECTOR*)&vel ); 

// set falloff rules.  By default, they use an inverse falloff,
// which you can think of for each doubling of min distance, you half the 
// volume.  Attenuation stops at max distance (sound will not get quieter)
channel->setMode( FMOD_3D_INVERSEROLLOFF ); // default, you don't need to call this; 
                                            // see docs for more options (FMOD_3D_LINEARROLLOFF is particulary useful for games)
channel->set3DMinMaxDistance( 1.0f, 10000.0f ); // default values - how they are used depends on the mode

// resume the sound now that it is setup; 
g_fmod->setPaused(false); 
```


## Audio Mixer
A mixer is a `FMOD::ChannelGroup` that other channel/channel groups can pipe to.  This allows us to control volume volume and 
effects for a collection of sounds.  We'll be using them as a way to control volume of different clips in our scene; primarily 
SFX (sound effects) and BGM (background music), which both pipe to master.  

You can get the `master` group by using `g_fmod->getMasterChannelGroup()`, which the default group sounds play to. 


This allows us to control SFX and MUSIC seperately, as well as have an overall volume control 'master'.

To create a `FMOD::ChannelGroup`, you can call `createChannelGroup` on your FMOD object; 

```cpp
// Creates a channel group
FMOD::ChannelGroup *mixer; 
g_fmod->createChannelGroup( "name", &mixer ); 


// will set subMixer to play through mixer;  
// Example would be "music" being a subMixer of "master".  So if music is 50%, and master is 50%, music would actually play at
// 25% volume, but anything else would play at 50%. 
mixer->addGroup( subMixer ); 

// to play a sound using a mixer
// FMOD::Channel*
playSound( sound, mixer, paused, &channel ); 

// free the memory of the group (unlike channels, they should be manually released on shutdown)
// DO THIS DURING SHTUDWON
mixer->release(); 
```
