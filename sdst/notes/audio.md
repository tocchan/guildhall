FMOD Core API Notes
======

## FMOD 3D Audio Link
- FMOD Whitepaper on 3D Audio: https://fmod.com/resources/documentation-api?version=2.0&page=white-papers-3d-sounds.html

## Overview
To get 3D Audio working you need to do three things
1. Load audio as a 3D resource
2. Set a listener in the scene
3. Configuring the 3D-Channel (`FMOD::Channel`)

## Loading
When loading audio using `createSound` - if you want it to be 3D, you just have to set the correct options.  In the case of
3D audio, just use the flag `FMOD_3D`.

Other load options of note;
- `FMOD_CREATESTREAM`:  Load the sound as a stream, keeping just enough of the audio in memory to play it.  Improves loading time and has a lower memory limit, but will cause more hits to the physical media.
- `FMOD_LOOP_NORMAL`: Resource is setup for looping and will loop cleaner.  Without this, some audio may *pop* when looping back around.


```cpp
// example
```


## Setting Listener
...


```cpp
// example
```


## Setting 3D Audio Position
...

```cpp
// example
```


## Audio Mixer
A mixer is a `FMOD::ChannelGroup` that other channel/channel groups can pipe to.  This allows us to control volume volume and 
effects for a collection of sounds.  We'll be using them as a way to control volume of different clips in our scene; primarily 
SFX (sound effects) and BGM (background music), which both pipe to master.  

This allows us to control SFX and MUSIC seperately, as well as have an overall volume control 'master'.

To create a `FMOD::ChannelGroup`, you can call `createChannelGroup` on your FMOD object; 

```cpp
// example
```
