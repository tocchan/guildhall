MP2-A02: Collision Response
======

## Overview
To have a basic 2D Physics engine with elastic collision response with adjustable restitution and mass. 

## Goals
- Object only destroy when fully off screne (use AABB intersect check - destroy if false).
- [One Week Demo](mp2.a02.week1.zip): By 2019-02-12 get to a point where objects push out of each other; 
- [Final Demo](mp2.a02.week2.zip): By 2019-02-19 should have collision response working. 

## Class Notes
- [2019-02-05 Notes on Manifolds & Correction](notes.manifold.md)
- [2019-02-12 Notes on Collision Response](notes.momentum.md)

## Controls
By default, arrow keys should control a cursor.  While in this cursor mode, when the user presses F1 through F4 it should spawn an object at the cursor of random size (extents somewhere between 0.25 and 2.00). 

- **F1** - Spawn a static box at cursor.
- **F2** - Spawn a static disc at cursor.
- **F3** - Spawn a dynamic box at cursor.
- **F4** - Spawn a dynamic disc at cursor.
- **< & >** - Adjust restitution of selected object, or what to create a new object as (clamped between 0 and 1)
- **N & M** - Adjust mass of selected object, or what to create a new object at (clamped between 0.1f and 10.0f)

The user should also be able to possess one of the objects.  Pressing TAB should select the first known object if no object is selected, otherwise cycle to the next object.  This should put the player in a "control" mode.  The possessed object should be coloured yellow and be treated as static for the duration. 

**NEW** While possessed, mass/restitution controls affect the selected object.

- **TAB** - Select the next object for possession.  If no object selected, select object closest to the cursor; 
- **SPACE** - Deselect object and reutrn to cursor mode. 
- **DEL** - Destroy selected object
- **ARROW KEYS** - Move the object. 

## Additional Notes
- Our physics system is fairly simple - you may notice *bugs* such as heavy objects able to push smaller objects through a floor.  This requires a bit more work to fix and would be a good vector for additional exploration.  This class though will focus mostly on objects the same mass so we'll ignore the issue. 
- Test thororoughly once you have it working.  These are a few things to check; 
  - Spawn a dynamic objects ontop of each other with 0.5 restitution, make sure they fall and don't stick together.
  - 0.0 restituion balls should slide off other balls, but maintain horiztonal movement and move off the edge the screen.

