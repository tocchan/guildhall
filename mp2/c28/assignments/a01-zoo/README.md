MP2-A01: Collision Zoo
======

## Overview
Be able to create static & dynamic AABB & Disc objects.  Dynamic objects should fall off the screen under the affect of gravity. 

Objects will be colored based on their state; 
- **YELLOW** - Static Object
- **MAGENTA** - Static Object in contact another object
- **BLUE** - Dynamic Object
- **RED** - Dynamic Object in contact with another object.
- **WHITE** - Selected

## Class Notes
- [2019-01-29 Notes](NOTES.md)

## Controls
By default, arrow keys should control a cursor.  While in this cursor mode, when the user presses F1 through F4 it should spawn an object at the cursor of random size (extents somewhere between 0.25 and 2.00). 

- **F1** - Spawn a static box at cursor.
- **F2** - Spawn a static disc at cursor.
- **F3** - Spawn a dynamic box at cursor.
- **F4** - Spawn a dynamic disc at cursor.

The user should also be able to possess one of the objects.  Pressing TAB should select the first known object if no object is selected, otherwise cycle to the next object.  This should put the player in a "control" mode.  The possessed object should be coloured yellow and be treated as static for the duration. 

- **TAB** - Select the next object for possession.  If no object selected, select object closest to the cursor; 
- **SPACE** - Deselect object and reutrn to cursor mode. 
- **DEL** - Destroy selected object
- **ARROW KEYS** - Move the object. 

## Additional Notes
- Objects that leave the screen should be destroyed.  
- The bottom of the screen should contain a solid static box to act as the floor.  Up to you if you want to allow this to be possessed or not; 
- The cursor should be constrained to the screen.  

