Special Topics
======

In this class, we will be creating a basic RTS framework.  This is the first time attempting this in this class, so expect a few growing pains as we work though it.

The class will be sturucted as feature requests for the RTS based on what we came in with from SD2.  All tasks will be posted here as soon as they are known, as well as the week assigned.  Tasks are due **TWO WEEKS** after that assigned week.  Meaning you have one week to work on it with minimal discussion, a class to ask questions, and another week to finish it out while starting on the next weeks task.

That gives us 6 assignments worth of work.

## Weekly Tasks

### Week 1 - UI & Visuals
#### Task List
- [ ] Design can define a unit in data, including the following stats; 
    - [ ] Speed
    - [ ] Health
    - [ ] Animation Set
- [ ] Units are rendered correctly with given art assets
    - [ ] Should support at least *walk* and *idle* animations.
- [ ] Both *peon* and *warrior* units are defined
- [ ] Be able to `spawn unit` which will spawn a unit where the user is looking
- [ ] Units no long self-intersect (will push off each other)
- [ ] User can select and issue commands to multiple units at once
- [ ] User can queue up tasks on units by holding shift
    - [ ] If not holding shift, latest task replaces entire task queue

#### Notes
- [Example Video](https://www.youtube.com/watch?v=Ie7Q4G8kqpI)
- [Frustum Work](notes/frustum.md)
- [Billboarding](notes/billboarding.md)
- [Animator](notes/animator.cpp)


### Week 2 - Combat & Audio
#### Task List
- [ ] Visually show which units are selected (world space)
- [ ] Display list of selected units (screen space) - includes type and current health
    - [ ] Color based on health percentage (> 80% green, > 30% yellow, red)
- [ ] Be able to spawn units as part of a team
    - [ ] `set_team` command to switch commander team
    - [ ] Can only select and issue orders to units who are part of your team
- [ ] With a unit selected, when right-clicking...
    - [ ] ...move when clicking just terrain (current behaviour)
    - [ ] ...attack when clicking a hostile
    - [ ] ...follow when clicking an ally team
- [ ] Allow units to attack other units
    - [ ] Display health bar above units that are damaged
- [ ] Allow units to die
    - [ ] Dead units are not selectable
    - [ ] Dead units disspear after a fixed (non-zero) time (3 seconds is a good start)
- [ ] Hook up 3D Positional Audio for Combat Sounds [hit, miss, death]

#### Notes
- [Example Video](https://www.youtube.com/watch?v=fyUrOtFWbTo)
- [Recommended Work List](notes/notes.a2.md)


### Week 3 - Buildings & Gathering
#### Task List
- [ ] Selected Unit UI.  Graphical and shows portraits
    - [ ] If more than 16 units selected, show 15, and the last tile just a box that says +n...
- [ ] Units display available commands that can be issued to them through UI
    - [ ] Peons have a sub-menu for build structure that lists structures they canb build
    - [ ] Move, Attack, Patrol, and Hold Position command available to `peon` and `warrior`
- [ ] Resources
    - [ ] Supply - used as a limiter for how many units you can build
    - [ ] Wood - used as a currency to build new units/buildings
- [ ] Buildings can be defined in data
    - [ ] Define townhall
    - [ ] Define hut
- [ ] Peon can build buildings
- [ ] Buildings can have dependencies (can only build hut if townhall is built)
- [ ] Townhall can build `peon`s
    - [ ] Up to max supply
    - [ ] Supply is given by huts
- [ ] Peons can chop trees for wood
    - [ ] `tree` unit defined in data
    - [ ] Tree moves through different mesh states as it lowers on health

#### Notes
- ???


### Week 4 - Win Conditions and Profiling
#### Task List
- [ ] Goblin Hut structure defiend
- [ ] `AICommander` that controls the Goblin Team
    - [ ] Will build goblins, and sends them to attack the town center when he has a big enough group
- [ ] Able to construct a Warrior Hall after a townhall is built
    - [ ] Warrior Hall can build `warrior` units
- [ ] Game completes in win condition when all goblin units are dead (buildings/units)
- [ ] Game completes in loss condition when all local units are dead (buildings/units)
- [ ] Map Editor
    - [ ] Can place trees, goblin huts, and starting units on a map
    - [ ] Can save out the map
    - [ ] Can load and play these maps
- [ ] Gather profiling data about your game and identify current hotspots

#### Notes
- ???

### Week 5 - Minimap & Threaded Loading
#### Task List
- [ ] Game shows a minimap
    - [ ] Shows buildings that belogn to a team
    - [ ] Shows units that belong to a team
    - [ ] Can click on minimap to move camera there
    - [ ] Can right click minimap to issue commands as if you were clicking the map terrain
- [ ] Initial load screen now shows a smooth animation to show that the game isn't frozen
    - [ ] Loading should mostly be handled on a secondary thread
- [ ] Support custom binary formats for meshes to speed up loading time

#### Notes
- ???


### Week 6 - Pathing & Optimization
#### Task List
- [ ] Units not navigate around obstacles instead of moving in a straight line toward a target
- [ ] Improve loading times farther
- [ ] General Visual and Audio Improves and Polish
- [ ] Users Choice

#### Notes
- ???


### Week 7 - Work Week

### Week 8 - Work Week & Final Presentation