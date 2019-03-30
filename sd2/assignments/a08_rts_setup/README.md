A08: RTS Initial
======

## Overview
Starting the RTS project - going to get the game modes that we're 
going to support for this. 

### Game Structure
- An init phsae (load all initial resources)
- A main menu to select the mode we want to play
- A load phase for loading resources needed for the mode
- An Editor Mode
- A Play Mode

### Game Classes
- `RTSCamera` - A Camera with a specific control scheme
- `Map` - A single level in the game. 

### Map Editor
- Vertex Painting Interface
- Saving and Loading a Map

## Rubric

### [100%] Required
- [ ] All game states are in and demonstratable
- [ ] Can load into a map from the main menu (ex: `play_map name`)
- [ ] Can load into map editor from the main menu (ex: `edit_map name`)
    - [ ] A map that doesn't exist will be created; 
- [ ] Can save from map-editor; 
- [ ] Basic `RTSCamera` controls work
    - [ ] Zoom
    - [ ] Pan
    - [ ] Pivot with Constraints
- [ ] Map is vertex painted

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Goal(s)
