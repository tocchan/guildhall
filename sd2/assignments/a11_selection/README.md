A11: Game Loop, Raycasts, and Handles
======

## Overview
Get the basic RTS scaffolding in;  We should be able to create, select, and command units. 

This will get the logical and physical representation in the game, as well as implement the math for for selecting units; 

Features added in this assignment; 
- `Camera::ScreenPointToWorldRay`
- `Ray3` class
- Raycasts vs simple shapes
- `GameHandle` system for tracking entities
- Command Queue for the Game
- Ability to render capsules (useful for debug)


## Rubric

### [100%] Required
[ ] 10%: Be able to create a unit using 'Spacebar'
    [ ] Unit is created using a command
[ ] 20%: Unit is represented using a capsule or wire capsule
[ ] 20%: Be able to select a unit using left mouse click
    [ ] Commands and controller should only hold onto entities using handles
[ ] 10%: Selected unit should be shown as a different color
[ ] 20%: Left clicking on the map deselects the unit
[ ] 20%: Right clicking will command selected entity to move to click location.

### Penalties
- [01%] Per Warning (up to 10%)
- [01%] Per Leaked D3D11 Resource (up to 10%)
- [15%] Broken Dev Console

## Resources

