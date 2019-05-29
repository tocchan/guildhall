Assignment 2 Notes
=======

## Recommended Order of Tasks
- [ ] Sprite Rendering Fixes
    - [ ] Switch to a cutout shader
    - [ ] Add some z-bias to prevent clipping with ground (will potentially result in different artifacts when we get meshes in)
- [ ] Audio System work
    - [ ] Define audio mixers/clips/cues in data
        - [ ] Create the 'master' mixer and use it as the default if playing a sound without a defined mixer
              This will allow use to control volume overall within the game; 
        - [ ] Mixers can feed into other mixers (bgm and sfx for example can feed into master to give more fine grained control and leave master control)
    - [ ] Load at init phase.
    - [ ] Add background music (streamed) to main menu and game
    - [ ] Be able to play a sound at a position
    - [ ] Update listener position to be the camera's position each frame
    - [ ] Adjust 3D audio falloff for positional sounds until you're happy with it
- [ ] Animation Events
    - [ ] Define events in animation data
    - [ ] When animator advances, trigger events for the time frame [special attention for loop/wrap modes]
- [ ] Teams
    - [ ] Be able to define number of teams for a map when loading it
    - [ ] Define team colors for each team
    - [ ] Be able to define hostility between teams on a -1 to 1 scale (-1 is enemies, 1 is allies, 0 is neutral).  
        - [ ] Make the choice if this is bidirectional or not - ie, are relationships mutual or can A like B while B hates A?  Structure accordingly
    - [ ] Allow player controller to change teams (debug feature)
        - [ ] Spanwed units should join the team of current controller who is spawning them.
        - [ ] Only be able to select units that are part of your team; 
- [ ] Suicide Task - *Not part of the assignment, but a good test case for the above, and getting health/dying in*
    - [ ] Task that just does max health in damage first frame, and ends; 
    - [ ] Prevent/cleanup tasks from running when unit dies
    - [ ] Make sure animation events that play sounds work here; 
    - [ ] Cleanup corpse after a few seconds
- [ ] ActOnCommand - When right clicking on a unit when you have a selection - the unit itself will determine the task to accomplish
    - [ ] Follow Task - Will follow target unit until either unit dies, or another task replaces current task queue 
    - [ ] Attack Task - Similar to follow target, except when it gets into attack range it will attack instead of move
        - [ ] Define attack range in data
        - [ ] Move toward target until within attack range
        - [ ] Play a one-off attack animation
        - [ ] Determine how you will handle one-off animations
        - [ ] Determine how you will propogate events from animation to task; 
        - [ ] Repeat



## Animation Events
Be sure when selecting events to fire for a frame, you don't accidently fire events twice, or not fire an event at all.  Test cases to work with;

- Have an event that fires first frame of an animation (time 0)
- Have an event that fires last frame (time 1)
- If playing this, and set to loop move, make sure both events fire when it wraps around
- Special care for playing in reverse (or ping pong) if you want to support those - but our game doesn't need them - pick your battles!

When an event fires, it is firing for *that* unit.  You could use the global event system, and give the animator enough information to work with, or life the events up from the animator to the entity, so it can decide what to do (for example, playing audio should play at the entities position, not globally.  An attack action needs to forward the event to the attack task, since 'attack' may do different things depending on the current task or animation being played (spell casters, archers, melee for example could all be coming from the same unit)


## Tasks and Death
What to do when an entity dies and he stil has tasks to complete?  

## Controlling Animation 
Who should be in charge of the animation to play?

- Task:  We can give it to the task, but then we have a lot of duplicate code for all the tasks (hacky aand quick)
- Entity:  Instead we can let the entity act as the animation graph, but will have to be able to handle one-off animations (old-school Unity)
- Animator: Give the animator more control on how to choose animations - but will need to expose properties to help control this from the entity (Unity/Unreal)


## Monolithic Entity/Unit
Right now, everything in our game is a `entity`.  Buildings, units, trees, rock, everything will currently is desigend to be a "unit", with only data, stats, and configurations differentiating them. 

I'm not 100% bought in on this architecture yet as there seems to be a strong functional difference between a decorative stone and an archer, but have decided to push this until we ca more cleary see what problems splitting the class out would cause vs the gains.  This currently is one of the bigger risks with the design of the code - with my thinking going as follows; 

- We will have likley hundreds of controllable thinking units (workers, buildings), but probably thousands of props (trees, rocks).  Thinking on a lot of "dumb" objects seems wasteful.  Though this can be bucketized by having properties on the entity add it to differnet work-groups,
- There is still a lot of shared similarities between units and props, such as health, rendering, animation, etc.  Most UML diagrams I made thinking about this ended up in a lot of weird diamond inheritance problems.
  - A component based approach would be fairly slick - but would require making a component system, which does not add enough to justify the cost; 

