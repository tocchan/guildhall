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
- [ ] Be able to `spawn unit` which will spawn a unit at cursor location
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
- [ ] 10%: Visually show which units are selected (world space)
- [ ] 05%: Display list of selected units (screen space) - includes type and current health
    - [ ] 05%: Color based on health percentage (> 80% green, > 30% yellow, red)
- [ ] 05%: Be able to spawn units as part of a team
    - [ ] 05%: Ability to switch team during gameplay (command or hotkey)
    - [ ] 05%: Can only select and issue orders to units who are part of your team
- [ ] 10%: With a unit selected, when right-clicking...
    - [ ] ...move when clicking just terrain (current behaviour)
    - [ ] ...attack when clicking a hostile
    - [ ] ...follow when clicking an ally team
- [ ] 20%: Allow units to attack other units
    - [ ] 05%: Display health bar above units that are damaged
- [ ] 05%: Allow units to die
    - [ ] 05%: Dead units are not selectable
    - [ ] 05%: Dead units disappear after a fixed (non-zero) time (3 seconds is a good start)
- [ ] 20%: Hook up 3D Positional Audio for Combat Sounds [hit, miss, death]
	- *note for future: explicitly state that this should be event driven*

#### Notes
- [Example Video](https://www.youtube.com/watch?v=fyUrOtFWbTo)
- [Recommended Work List](notes/notes.a2.md)


### Week 3 - Tasks and Buildings
#### Task List
- [ ] 10%: Be able to define what units can do in data;  [Example](./examples/humans.a3.xml)
- [ ] 10%: Right-click now picks best task for the job.
      - Maintain old behaviour, but now more unified using a utility method
      - When a group is selected, each unit may pick a different task 
            (ex: peons gather, warriors move, when right clicking a tree)
- [ ] 02%: Units display available commands that can be issued to them through debug UI
    - [ ] 05%: Be able to select a task, and use mouse to select the target of the task
    - [ ] 03%: Task is applied to all units selected that can run that task.  Others ignore it; 
- [ ] 10%: Allow units to be rendered using a model in data instead of an animated iso-sprite
    - Tree Example: [Model File](./examples/foliage_models.xml), [Unit File](./examples/foliage_units.xml)
    - [Example](./examples/humans.a3.xml)
    - *Recommend breaking rendering out to a [Renderable](./notes/renderable.md) component*
- [ ] 05%: Buildings can be defined in data
    - [ ] 05%: Define a tree
          - Be able to mark it as a resource to make others tasks easier;
    - [ ] 05%: Define townhall
- [ ] 05%: Hook a key so you can spawn trees on a map at cursor location
- [ ] 20%: Create a `GatherTask`
    - Added it to peons
    - It behaves like `AttackTask`, but only works on *resources*, basically trees for now; 
    - Make it so your `AttackTask` can't target trees
    - Make it so `GatherTask` can only work on resource objects; 
- [ ] 20%: Peon can build buildings
    - [ ] Add a `BuildTask` so peons can build a `townhall` 
    - [ ] When selecting a target, show a placeholder mesh to show the building being built
    - [ ] `BuildTask` just immediate creates the building

#### Notes
- [Unit Example File](./examples/humans.a3.xml)
- [Model Example File](./examples/foliage_models.xml)
- [Renderable Notes](./notes/renderable.md)
- todo: video


### Week 4 - Win Conditions and Profiling
#### Task List
- [ ] Occupancy Maps
    - [ ] 05%: Buildings define their occupancy region in data
        - *Note: could also infer it from the model size, but should allow an override through data*
    - [ ] 05%: Static objects when placed/built should snap to their correct tile location.
- [ ] 05%: Make a tree a 1x1 "building" that is placed at a tile and occupies the map;
- [ ] -: Collision Types for units
    - [ ] Disc Collider for units
    - [ ] Box (AABB2) collider for buildings
    - *Note: Similar to the Pachinko colliders, but doing much less.  There are no rigidbodies, physics systems, etc...*
- [ ] Resources
    - [ ] 03%: Supply - used as a limiter for how many units you can build
          - Automatically add this when a unit is built and assigned to a team
          - Automatically remove this when that unit dies
    - [ ] 02%: Wood - used as a currency to build new units/buildings
    - [ ] 05%: Display on the screen your teams current supply, supply limit, and wood resource values; 
- [ ] Buildings are now property constructed;
    - [ ] 05%: Display a build preview that is properly tile-snapped; 
    - [ ] 05%: Can only build in tiles that are not occupied (using Occupancy map)
    - [ ] 02%: Can only build if you have the resources
    - [ ] 03%: Resources are consumed upon building; 
    - [ ] 04%: Peons then must be near the building to build it up, playing an attack animation while nearby; 
      - [ ] 02%: Allow multiple peons to work on the same building; 
- [ ] Townhall can build `peon`s
    - [ ] 04%: Up to max supply, and only if resources are available; 
    - [ ] 04%: Peons train at the townhall - visually show a progress bar for this
    - [ ] 02%: Upon finishing training, spawn a peon in an open tile near the building; 
- [ ] Peons can chop trees for wood (Proper `Gather` behaviour)
    - [ ] 02%: Peons move toward resource points (trees), and attack
    - [ ] 02%: Upon attacking, gatherer should add resources to his supply and remove them from the tree
    - [ ] 02%: When the gatherer is full, he should return to the town hall to drop off
    - [ ] 02%: Upon reaching town hall, add his resources to the team, clearing his owned resources in the process
    - [ ] 02%: Return to his target tree afterwards.  
    - [ ] 02%: If the target tree is dead, find the closest tree to his current location as the new target
    - [ ] 02%: If no new targets are present, the `Gather` task ends
    - [ ] 02%: Trees die when they run out resources; 
        - [ ] 03%: Tree moves through different mesh states as it lowers on health
- [ ] 20%: Do your appropriate debugging. 

#### Notes
- [Video Example](https://www.youtube.com/watch?v=ilvaslSdLgs)

### Week 5 - Minimap & Threaded Loading
#### Task List
- [ ] **ENGINE WORK**: Updated Time Functions
- [ ] **ENGINE WORK**: `PROFILE_LOG_SCOPE` and supporting objects.  *Recommend `Core/Profiling.hpp`*
- [ ] Time the load phase of your game, **in both DEBUG and RELEASE configurations**.  Write it down - I'll be asking for this number.
    - [ ] DEBUG TIME:   ________________
    - [ ] RELEASE Time: ________________
- [ ] **ENGINE WORK**: Add an `AsyncQueue` templated class.  *Recommend 'Core/Async/AsyncQueue.hpp'*
    - [ ] Support thread-safe `void AsyncQueue::Enqueue( T const &obj )` that enqueues the object safely
    - [ ] Support thread-safe `bool AsyncQueue::Dequeue( T *out )` that will dequeue an safely, and return bool is it succeeds
    - *Recommend using a `std::queue` and a `std::mutex` to implement this class*

- [ ] Initial load screen now shows a smooth animation to show that the game isn't frozen
      - [ ] This should never stutter for more than half a second, and rarely
- [ ] Game will load textures asynchronous during the initial startup phase
- [ ] Game will load models asynchronous during initial startup phase
- [ ] Once all resources are loaded, stop the loading threads
- [ ] Time the load phase after the work is done; 
    - [ ] Debug Time:   _________________
    - [ ] Release Time: _________________ 

- [ ] Goblin Hut structure defined
- [ ] Goblin Defined
- [ ] Feature: Map Editor
    - [ ] Can define number of teams for this map
        - [ ] Can define team color for any team
        - [ ] Can define starting resources for a given team
    - [ ] Can place any defined unit/building.  Any *functional* UI is fine here; 
          - Quickest option is a console command of `set_editor_cursor unit=<unitname> team=<team>`*
          - Another option is to have cycling menus you can key through
          - If you're using ImGUI for other projects - it would work well here too; 
    - [ ] Can save out the map (xml is fine) to a *specified name*.  Recommend a console command `save_map name=<name>`
    - [ ] Can load into these maps from the main menu.  Recommend a console command `load_map name=<name>`
    - [ ] If a `level0.map` exists, load that by default; 
- [ ] Game must compile & run in **Release** and **Debug** build configurations;

**Warning:  Once you start working with threads - a lot of your engine systems are not thread-safe.  In particular your `ConsolePrintf`, and potentially some logging functions.  You have the tools to fix this now if you so desire, but it not required for the assignment.**

#### Notes
- [Deadlock Empire](https://deadlockempire.github.io/) - Very good site for helping you gain intuition on race conditions; 
- [Amdahl's Law](https://en.wikipedia.org/wiki/Amdahl%27s_law) - Theoritical Speed-Limit to how much parrallization can help a specific problem. 
- [Instruction Pipelining](https://en.wikipedia.org/wiki/Instruction_pipelinin) - More just light reading atm.  Useful to know about for micro-optimization; 
- [Mike Acton: Data Oriented programming](https://www.youtube.com/watch?v=rX0ItVEVjHc) - Good talk about data oriented programming.


### Week 6 - Pathing & Optimization
#### Task List
- [ ] Game shows a minimap
    - [ ] Shows buildings that belogn to a team
    - [ ] Shows units that belong to a team
    - [ ] Can click on minimap to move camera there
    - [ ] Can right click minimap to issue commands as if you were clicking the map terrain
- [ ] Game completes in win condition when all goblin units are dead (buildings/units)
- [ ] Game completes in loss condition when all local units are dead (buildings/units)
- [ ] Pathing!
- [ ] `AICommander` that controls the Goblin Team
    - [ ] Will build goblins, and sends them to attack the town center when he has a big enough group- [ ] Support custom binary formats for meshes to speed up loading time
- [ ] Buildings can have dependencies (can only build hut if townhall is built)
- [ ] Units not navigate around obstacles instead of moving in a straight line toward a target
- [ ] Improve loading times farther
- [ ] General Visual and Audio Improves and Polish
- [ ] Users Choice

#### Notes
- ???


### Week 7 - Work Week

### Week 8 - Work Week & Final Presentation