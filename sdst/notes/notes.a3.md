Assignment 3 Notes
=======

## Recommended Order of Tasks

- [ ] Add the idea of a "Team" to the map or game
    - [ ] Recommend a class or struct managing all information about a team
       - [ ] Color
       - [ ] Resources
       - [ ] Supply
- [ ] Be able to render a tree
    - [ ] Define model files for tree
    - [ ] Have entities be able to render a tree instead of a sprite
        - [ ] Create a `IRenderable` or similar class
            - [ ] Create a child class `SpriteRenderable` and move sprite rendering logic over to it
            - [ ] Have `Entity` update the `SpriteRendererable` with what it needs to render this frame. 
            - [ ] Call the render method on the renderable in from `Entity`
            - [ ] Create a `MeshRenderable` child class of `IRenderable`
                - [ ] Trees (or units with models) use this
                - [ ] Update and render similar to sprite, but with the data needed for the model. 
        - *Note: You can have an entity have a list of renderables, or just make a renderable for all permutations of what we'll be drawing.  For RTS, both approaches would result in similar amounts or work* 

        - [ ] If using a clip shader - you can just render in whatever order
        - [ ] If using an alpha shader and sorting - render trees BEFORE rendering units.
    - [ ] Be able to change what you render based on health percent (full, healthy, weak, and dead)
    - [ ] *extra* Add a bit of wobble on hit, and fall over on death. 
- [ ] Tasks - Continued
    - [ ] Go through and define in data how you want to think of tasks
    - [ ] Have the code be able to load these in
        - [ ] Recommend **Prototype Pattern** here, as you will clone tasks when issuing them
        - *Note: Combining move/follow into a single task made sense to me here.*



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
