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
- A basic UI structure with some basic UI building blocks
- Extending our `NamedProperties` to support arbitrary types; 


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
  - [ ] `UIWidget` - Master object
  - [ ] `UILabel` - A simple text label
  - [ ] `UIButton` - A clickable button with text and/or an image.  
  - [ ] `UIRadioGroup` - A group of multiple buttons in which only one button can be selected.
  - [ ] Main Menu using a `UIRadioGroup`
  - [ ] Editor Scene has selectable texture buttons in a `UIRadioGroup`

**Note:  Feel is important on this one, especially on the `RTSCamera` and UI.  I will be deducting points for easy to break features, such as the camera scrolling not being smooth, zoom distance not being clamped, or not being contained to the map.**

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console


## Goal(s)

### Demo
- [./builds/a08.zip](./builds/a08.zip)

### UI
We'll have a code-based UI system for now, that supports positioning and sizing.  

The goal will be to have an interface somewhat similar to this; 

```cpp
   // Create Editor UI
   m_ui = new UIWidget(); 
   m_terrain_group = new UIRadioGroup(); 

   float button_size = 64.0f; 
   float button_pad = 8.0f; 
   vec4 cursor = vec4( 0.0f, 1.0f, 32.0f, -32.0f ); 
   for (uint i = 0; i < MAX_TEXTURES_PER_PATCH; ++i) {

      // Add a button
      UIButton *btn = m_ui->CreateChild<UIButton>(); 

      // Position and size it; 
      vec4 pos = cursor + vec4( 0.0f, 0.0f, (button_size + button_pad) * i, 0.0f ); 
      btn->SetPosition( pos ); 
      btn->SetPivotAndSize( vec2( 0.0f, 1.0f ), vec2(button_size) ); 
      btn->SetTexture( map->m_color_textures[i] ); 

      // Add it to the radio group and set a select on; 
      btn->SetRadioGroup( m_terrain_group ); 
      btn->AddSelectEvent( Stringf( "set_map_texture index=%u", i ) );
   }

   // set first selectoin, and move down for drawing the first slider; 
   m_terrain_group->SetSelectionIndex( 0U ); 
   cursor.w -= 64.0f;

   // Add a little space - and create a slider label; 
   cursor.w -= 16.0f;
   UILabel *label = m_ui->create_child<UILabel>( cursor ); 
   label->SetPivot( vec2( 0.0f, 1.0f ) ); 
   label->SetTextSize( 16.0f ); 
   label->SetText( "PAINT STRENGTH" ); 


   // ... add rest of the UI
```

We will hook this into our normal event system so that any command we can trigger through dev console will also be triggerable through UI. 

This will also motivate us to finally extend our `NamedProperties` container to support arbitrary data types instead of just strings.  

- [./src/NamedProperties.hpp](./src/NamedProperties.hpp)



