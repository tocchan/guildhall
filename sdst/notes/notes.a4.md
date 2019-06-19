Assignment 4 Notes
=======

Sorry for dropping the ball on the A03 notes :(

## Recommended Order of Tasks

### Occupancy Maps & Tile Snapping
- [ ] Define an occupancy map for you map;
    - [ ] This can be as easy as a tool on your current `tile_info_t`
    - [ ] Add helper methods like...
        - [ ] `void Map::Occupy( ivec2 tile_pos )`
        - [ ] `void Map::Occupy( ivec2 bl_corner, ivec2 tr_corner );`
        - [ ] `vec2 Map::SnapToTilemap( vec2 pos, ivec2 size );`
        - [ ] `bool Map::IsOccupied( ivec2 pos )`
        - [ ] `bool Map::IsOccupied( ivec2 bl_corner, ivec2 tr_corner );`
        - ... etc... 
- [ ] A debug render mode to visualize the tilemap can be useful, and can set you up for work we'll be doing with pathfinding; 
- [ ] Get current buildings snapping to grid on build
- [ ] Make your tree a building, and confirm that if it snaps to a tile when placed
- [ ] Do not allow buildings (static objects?) from being pushed by other entities!

### Collision 
Buildings and units should use different collision (disc for units, aabb2 for buildings).

- [ ] Have a `Collider` type
    - [ ] Have a `ColliderDisc` and `ColliderBox`
    - [ ] This is very similar to MP2 and MP2 objects could be reused here.  Just know we don't need Rigidbodies; 
- [ ] Unit definitions can specify the collision type they want to use;
      - Example: `<collision type='disc' radius='0.4' />`
- [ ] Update your corrective step to use the proper shape-vs-shape push out (`manifold`) when correcting; 
    - [ ] As an additional challenge - use the proper 3D shape for raycasting that corresponds to your 2D physics shape, ie, disc uses capsule, and box uses aabb3; 


### Resources
- [ ] Add `supply`, `supply_limit`, and `wood` values to your `Team` object
- [ ] DISPLAY THESE RESOURCES ON SCREEN for your player's team!  ie, "Supply (1/8)  Wood(100)"
- [ ] Give your huts/townhall a `supply_limit` value
      - Example: `<unit id="human/townhall" supply="0" supply_limit="8" />`
      - This means the townhall uses up 0 supply, and increases the supply limit by 8;
- [ ] Give your units a supply cost
      - Example `<unit id="human/peon" supply="1" />`
      - Peons now require you to have at leaset 1 spare supply to build
- [ ] Upon spawning a unit and assigning it to a team...
      - [ ] Add to the `supply_limit` of that team based on the unit
      - [ ] Add to the `supply` of that team based on the unit
- [ ] Upon a unit being removed from a team (or dying)
      - [ ] Remove both `supply` and `supply_limit` of that unit from that team
- [ ] Add a `wood_cost` attribute to your `EntityDefinition` object, that defaults to 0; 
    - [ ] The `BuildTask` should immediately fail if you can not afford the building.
    - [ ] The `BuildTask` should subtract the current cost from the teams wood resources if he can spawn the building.


### Bulidings
- [ ] Define a `build_time` attribute in data for things that are trained/built
    - Example: `<unit id="human/townhall" build_time="120" />`
- [ ] When picking a target location for the `Build`, you should tile-snap the preview mesh
- [ ] The build task should behave as follows if it has no initial target but a definition to build set; 
    - [ ] First check you can afford the building
    - [ ] Check the build space is free of other static objects (we'll worry about dynamic objects in a later assignment)
    - [ ] Build the building, and deduct the cost from the teams funds
    - [ ] Set the building to a `CONSTRUCT` state, and set progress to 0.0%
        - [ ] Optionally you can also set the health to a very low initial health value (01% for example), but non-zero so it doesn't immediately die. 
    - [ ] The builder then moves onto the its normal 'help build' state of this task; 
- [ ] If a target was present, or the building was built, the peon now does the following behaviour;
    - [ ] If the building is not in a `CONSTRUCT` state, immediately finish the task; 
    - [ ] Move toward the building
    - [ ] If "near enough", start playing the attack animation
    - [ ] For **every frame** you are near enough, tell the building to increment its build progress.
        - [ ] How you keep track of progress is up to you - just know that if you state 120 as teh build time, I expect it to take 120s to build with a single peon. 
        - [ ] Once the building reaches 100% progress, set it to the `DEFAULT` state;
- [ ] Units in a `CONSTRUCT` state should not be allowed to be issued tasks (for now)


### Training
This is the first moment that having a `Component` system is really useful, as training breaks the mold of normal task behaviour, as it is potentially running multiple tasks in parallel.  

That said, I'm going to walk through a simpler version that just uses a single `TrainTask` to do this.  If you are interested in the component version let me know; 

- [ ] Create a `TrainTask` to keep track of what the unit can train, as well as progress into it when instanced; 
- [ ] When a unit selects a `TrainTask` it should immediately enqueue 
    - [ ] Potentially add meta-options to tasks, like `AUTO_SELF_TARGET` to help with UI, as it is the first task the doesn't require a target selection.
- [ ] Training will be broken down in a few states; 
    - [ ] `INITIAL` state
        - [ ] Try to `ReserveCost` for the unit.  This should take the resources required by the unit.  If this fails, stay in the state.
        - [ ] If able to reseve, mvoe to the `TRAINING` state
    - [ ] `TRAINING` state
        - [ ] Add frame time to a current build time; 
        - [ ] Update the task progress to be our current time building divided by the time needed (from the definition)
        - [ ] Upon completion, spawn the unit in a free tile around the building
            - [ ] Recommend jittering the start point so units clump instead of push out in a line
        - [ ] Set to a `COMPLETE` state, and finish the task; 
        - [ ] If cancelled during the `TRAINING` state, refund the reserved cost of the unit; 

### Gathering
- [ ] Add a `carry_limit` attribute to the unit definition; 
- [ ] Add a `carry_amount` member to the actual unit
- [ ] Be able to mark a unit in data as a `resource` (can be gathered) and a `depot` (can be returned to)
    - [ ] Could be a string tag
    - [ ] Could just be a bit flag
    - [ ] Could give it a `Resource` component
- [ ] Everything else is fairly straight forward based on the assignment tasks; 
    - Be sure to only allow gathering on units that are marked as a `resource`
    - Be sure to only allow returning to units marked as a `depot`
- [ ] Recommend the following helpers; 
    - [ ] `Map::GetEntitiesInDisc( vec2 pos, float radius, filter_function_t filter )` to use when looking for a new target; 
    - [ ] `Map::FindEntityClosestTo( vec2 pos, filter_function_t filter )`
        - where `bool (*filter_function_t)( Entity *ent )` is a function callback that returns true if the unit passes the filter; 




