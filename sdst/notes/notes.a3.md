Assignment 3 Notes
=======

## Recommended Order of Tasks

- [ ] Figure out how want to define your data
- [ ] Recommend making tasks be able to self-register; 
- [ ] Maintain A2 Functionality but with generic right click; 
    - [ ] Add a `UnitTask::GetActionUtilityForTarget` or similar method for rating this take for a specific entity-target pair. 
    - [ ] Hook right click in the `PlayerController` to compute best task per unit for each target
    - [ ] Issue that task to that unit
- [ ] When units selected - use the first unit to show a task list on the screen.
    - [ ] Allow user to select this task however you want - just a single key is good
        - *Note: Another option is to define the hotkey in data for this task and data-drive it.  But sure you print what the hotkey is to screen*
    - [ ] Once a task is selected, allow user to select a target.
    - [ ] Issue that task on each selected unit *that can run that task*.  Don't do anything to units that can't run that task; 
        - [ ] Enqueue if shift is held - you can generalize this with a `Entity::SetTask` vs `Entity::EnqueueTask`
- [ ] Get trees in the game
    - [ ] Split your renerdering out to seperate `Renderable` types
        - [ ] 
        
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


- [ ] Add the idea of a "Team" to the map or game
    - [ ] Recommend a class or struct managing all information about a team
       - [ ] Color
       - [ ] Resources
       - [ ] Supply
- [ ] Be able to render a tree
    - [ ] Define model files for tree
    - [ ] Define a tree entity
        - [ ] Have a way to identify this tree as a resource (a flag, tag, etc..)
    - [ ] Have entities be able to render a model instead of a sprite
        - [ ] Create a `IRenderable` or similar class
            - [ ] Create a child class `SpriteRenderable` and move sprite rendering logic over to it
            - [ ] Have `Entity` update the `SpriteRendererable` with what it needs to render this frame. 
            - [ ] Call the render method on the renderable in from `Entity`
            - [ ] Create a `MeshRenderable` child class of `IRenderable`
                - [ ] Trees (or units with models) use this
                - [ ] Update and render similar to sprite, but with the data needed for the model. 
        - *Note: You can have an entity have a list of renderables, or just make a renderable 
           for all permutations of what we'll be drawing.  For RTS, both approaches would result in 
           similar amounts or work* 

        - [ ] If using a clip shader - you can just render in whatever order
        - [ ] If using an alpha shader and sorting - render trees BEFORE rendering units.
    - [ ] Be able to change what you render based on health percent (full, healthy, weak, and dead)
    - [ ] *extra* Add a bit of wobble on hit, and fall over on death. 
- [ ] Tasks - Continued
    - [ ] Go through and define in data how you want to think of tasks
    - [ ] Have the code be able to load these in
        - [ ] Recommend **Prototype Pattern** here, as you will clone tasks when issuing them
        - *Note: Combining move/follow into a single task made sense to me here.*
    - [ ] Right click chooses best command based on target
        - [ ] Tasks have a way to inform game of want to be used (give a weight for a given target/position)
            - [ ] A weight of 0 or negative should be considered an invalid task for the target.
        - [ ] Controller chooses task with best weight
        - [ ] Configure tasks so that right clicking will follow an ally, attack an enemy, and move toward trees
        - [ ] Add a gather task that is similar to "attack" for now, that only the peon can do - have it be able to target trees.
    - [ ] When selecting units - list available tasks in a temporary UI for the first unit selected
        - [ ] Be able to select which task you want to perform, and left click to "do" it
            - [ ] Task may have flags such as "needs target" to go into this second step.  
        - [ ] Be able to move or attack this way (should now be able to attack allies, but not trees, at allies have a low task priority, but trees are invalid targets for an attack)
        - [ ] If a valid target is picked - apply that task for all units selected that can perform it.
- [ ] Buildings
    - [ ] Define a building for the towncenter
    - [ ] Define a build task 
    - [ ] Allow 

## Teams
```cpp
class Team 
{
   uint m_index;
   rgba m_color;  
};
```

## Rough Notes
- Buildings
  - Building locked to grid
  - occupancy maps
- Resources
  - buying
  - refunding
- Unit
  - Task
  - Right-Click Task Choosing
  - Task UI
- Unit UI

## Things to Talk About
- Self Idenfifying classes : We did this before, what are the options?
- List of renderables -vs- specialized renderables; 
  - Renderer architecture - higher level framework 
- Definition/Factory pattern for renderables; 
- Prototype pattern for Actions
- Self Defining/Registering Tasks
  - Macros and Global Registrations
    - `#define TASK_TYPE( name, class )` example
- Colliders
- Tile Map/Occupancy
- Weighting Tasks (utility function)
  - `UnitTask::GetTargetUtility()`

- Retroactive
  - Redesign - I think I would have one kind of command, and that would be unit->task combos that are deferred enqueues for networking sake.  If not networking just deal directly with issuing tasks to objects; 