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


### Week 3 - Tasks and Buildings
#### Task List
- [ ] Be able to define what units can do in data;  [Example](../examples/units.a3.xml)
- [ ] Right-click now picks best task for the job.
      - [ ] Maintain old behaviour, but now more unified using a utility method
      - [ ] When a group is selected, each unit may pick a different task 
            (ex: peons gather, warriors move, when right clicking a tree)
- [ ] Units display available commands that can be issued to them through debug UI
    - [ ] Be able to select a task, and use mouse to select the target of the task
    - [ ] Task is applied to all units selected that can run that task.  Others ignore it; 
- [ ] Allow units to be rendered using a model in data instead of an animated iso-sprite
    - Tree Example: [Model File](../examples/foliage_models.xml), [Unit File](../examples/foliage_units.xml)
    - [Example](../examples/humans.a3.xml)
    - *Recommend breaking rendering out to a [Renderable](../notes/Renderable.md) component*
- [ ] Buildings can be defined in data
    - [ ] Define a tree
    - [ ] Define townhall
- [ ] Hook a key so you can spawn trees on a map at cursor location
- [ ] Create a `GatherTask`
    - [ ] Add it to peons
    - [ ] It behaves like `AttackTask`, but only works on trees
    - [ ] Make it so your `AttackTask` can't target trees (visible result is now warriors no longer can attack trees)
- [ ] Peon can build buildings
    - [ ] Add a `BuildTask` so peons can build a `townhall` 
    - [ ] When selecting a target, show a placeholder mesh to show the building being built
    - [ ] `BuildTask` just immediatley creates an entity as part of your team once peon reaches target point.

#### Notes
- ???


### Week 4 - Win Conditions and Profiling
#### Task List
- [ ] Occupancy Maps
    - [ ] Buildings define their occupancy region in data
        - *Note: could also infer it from the model size, but should allow an override through data*
- [ ] Collision Types for units
- [ ] Resources
    - [ ] Supply - used as a limiter for how many units you can build
    - [ ] Wood - used as a currency to build new units/buildings
    - [ ] Tree can be cut down
- [ ] Buildings can have dependencies (can only build hut if townhall is built)
- [ ] Townhall can build `peon`s
    - [ ] Up to max supply
    - [ ] Supply is given by huts
- [ ] Peons can chop trees for wood
    - [ ] `tree` unit defined in data
    - [ ] Tree moves through different mesh states as it lowers on health
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