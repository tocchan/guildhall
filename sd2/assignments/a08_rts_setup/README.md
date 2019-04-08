A08: RTS Initial
======

## Overview
Starting the RTS project - going to get the game modes that we're 
going to support for this. 

### Game Structure
- An init phase (load all initial resources)
- A main menu to select the mode we want to play
- A load phase for loading resources needed for the mode
- An Editor Mode
- A Play Mode

### Game Classes
- `GameInput` or similar class
- `RTSCamera` - A Camera with a specific control scheme
- `Map` - A single level in the game. 

### Basic UI
- `UIWidget`
- `UIButton`, `UISlider` and `UIButtonGroup` UI objects;
- Buttons should be selectable with Mouse; 

## Rubric

### [100%] Required
- [ ] 30%: All game states are in and demonstratable
- [ ] 20%: A map is generated, and can be rendered (and is broken into correct number of vertices)
- [ ] 10%: Map can be resized, and regenerated.
- [ ] 20%: Basic `RTSCamera` controls work
    - [ ] Zoom
    - [ ] Pan
    - [ ] Pivot with Constraints
- [ ] 20%: UI Elements
  - [ ] UIWidget
  - [ ] UIButton
  - [ ] UIRadioGroup
  - [ ] UISlider
  - [ ] Main Menu using a UIButtonGroup
  - [ ] Editor Scene has a functional UI

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Goal(s)
