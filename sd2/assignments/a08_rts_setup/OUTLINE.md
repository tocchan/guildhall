A08 Suggested Order
======

## Major Points
This assignment a few systems involved - so this list is a high-level checklist of tasks that should be completed; 

- [ ] Implement 5 required game states
- [ ] Have a `Map` that can generate the tiles and mesh needed
- [ ] Implement a `GameInput` class to translate raw input into game commands
- [ ] Implement an `RTSCamera` that is well behaved; 
- [ ] Implement basic game `UI` objects
    - [ ] Extend `NamedProperties` to work with arbitrary data types; 
- [ ] Implement map editor 
    - [ ] Implement cursor shader

Out initial Goal is just to get us into the editor and play modes, so we can start implementing the bulk of the assignment; 


## Init Screen & Main Menu
First, lets get the initial two screens in, and introduce a static load screen (unresponsive) by forcing at least one frame to render before starting our more heavy duty loading; 

- [ ] Implement two Game States, `Init` and `MainMenu`
- [ ] When the `Game` starts up - create a ui camera - set this to the resolution you want to use for your UI (I use 720p as my reference resolution). 
- [ ] Set yourself immediately to the `Init` state - load enough resources to render a splash screen to tell the user it is loading.  Render and Present a single frame of this scene.
- [ ] On the subsequent frame, load all other common resources (for now, textures & shaders)
    - [ ] I would recommend just keeping arrays of resource names here for now.  
- [ ] Once done loading, move onto the `MainMenu` state. 

## State & Class Stub Out
Next, we'll stub out the rest of the states and get the base functionality.  Commonly when I'm implementin ga new system I'll create the classes I think I'll need as empty stubs, and add methods as I implement the functionality I want.  For example, for a `Load` state, it will start by starting a `Map::Load`.  Upon finishing, moving on to the next state; 

- [ ] Implement the two paths...
    - [ ] Use console commands to start on these paths
    - [ ] `Load` to `Play`
    - [ ] `Load` to `Editor`
- [ ] `Load` is similar to `Init`, in that it'll render a single frame before doing any real work;
- [ ] Stub out the `Map` class...
    - [ ] `Map::Load` for `Load` to `Play` path
    - [ ] `Map::Create` or `Map::Load` for `Load` to `Editor`
- [ ] Upon finishing loading - present an option to continue to the player
    - [ ] Notify user the game is loaded, and wait for user interaction before continuing. 
    - [ ] "Press Any Key" or "Press Spacebar" are both acceptable forms of continuing
- [ ] Just render test to the screen for `Editor` and `Play` mode to show that you got there; 

From here there is in my opinion two areas I can move onto.  Either work on UI in the `MainMenu` state, or start fleshing out the `Map` and `RTSCamera`.  

I'm going to start with the `Map` and `RTSCamera` path as I think it is more interesting. 


## The Map
The `Map` class will be the master container for most of our game.  It will contain the terrain, entities, props, projectiles, etc... and play a major role in rendering, UI, and AI.

A map will be made up of a 2D-array of tiles.  These tiles will be the gameplay representation of the map.  They will hold information needed for pathing, AI, and other game specific purposes.

The map will also contain a 2D-array of vertices.  These will be the visual representation of the map, and will be used for rendering and UI.  We will also be using the representation 

- [ ] Start by creating a stub struct `MapTile`.  For this assignment, this structure will remain empty. 
- [ ] Flesh out `Map::Create` (and have `Map::Load` just call into `Map::Create` for now)
- [ ] Generate an array of tiles large enough to support the map size
    - [ ] `Map::GetTile( int x, int y )` will get a tile, starting at (0, 0) being the bottom left corner of the map 
- [ ] Generate verties to visualize this map.  Each tile is compromised of four quads to give us a little more visual fidelity per tile.  See below image; 
- [ ] Each tile should be on UV unit large (and 1 unit in world distance on the XY-plane)
- [ ] Game modes should now setup a 3D perspective game camera (`RTSCamera`), and call `Map::Render` which should call into `Map::RenderTerrain`
    - [ ] Be sure you can see a solid terrain.


## GameInput
New in this assignment (and unsure if this is similar to SD1), we're going to add a `GameInput` class that will translate raw input (mouse and keyboard) into game specific concepts (pan, zoom, attack, select, etc...)

This is to unify all the different types of ways we may issue a command, and let the game only worry about the commands themselves.  For example, we will hook up both mouse scrolling and keyboard scrolling in this assignment; 

This will also store game options such as scroll speed (for mouse and keyboard). 

- [ ] Add a `GameInput` class. 
- [ ] Stub out `vec2 GameInput::GetFramePan`, `float GameInput::GetFrameZoom`, and `float GameInput::GetRotation()`... return 0's for now; 
= [ ] Add the following  members; 
    - `float m_keyboardPanSpeed`  - control how quickly we pan based on keyboard input
    - `float m_mousePanSpeed` - control how fast we pan based on moving the mouse to the edge of the screen.
    - `float m_mousePanEdgeDistance` - control how close to the edge in UI units to get before starting to scroll.
    - `float mRotationConstraint` - How much we allow the user to rotate the camera. 
- [ ] Implement `GameInput::Update`...
    - [ ] Tie `WASD` controls to set current frame pan (use keyboard pan speed)
    - [ ] Tie mouse position to keyboard mouse (use edge distance and mouse pan speed)
    - [ ] Tie mouse wheel to zoom speed;
    - [ ] If the `Control` key is held down, tie mouse movement to control the current rotation.
        - [ ] This is limited by rotation constraints;
    - [ ] If the control key is not held, trend back to no rotation over multiple frames;  
    - *Note: fee free to add other controls if you prefer.  This is just the baseline I expect.*

You can use `DebugRender` commands to verify this is working, or wait until you implement the `RTSCamera` to verify your work; 


## The RTSCamera
The `RTSCamera` inherits from `Camera`, so that we can interact with it in a way that makes more sense for an RTS.

- [ ] Implement the following...
    - [ ] `RTSCamera::SetFocalPoint( vec3 pos );` - Set target (point directly along cameras forward)
    - [ ] `RTSCamera::SetDistance( float zoom );` - Set distance from focal point.
	    - [ ] Add options to limit distance (boht minimum and maximum distance)
    - [ ] `RTSCamera::SetRestingRotation( float degrees );` - set default rotation around focal point when at "rest", ie, no user rotational offset;  *I use -45 degrees*
    - [ ] `RTSCamera::SetRotationOffset( float degrees );` - Set an additional rotation around focal point

Every frame during gameplay, we will use `GameInput` to update the `RTSCamera`.

- [ ] Keep track of a current 2D `m_targetPoint` on your game or game mode; 
- [ ] Use `GameInput` frame pan to move this target point;
    - [ ] **Be sure to pan relative to camera's current forward/right**
- [ ] Implement a `vec3 Map::GetPosition( vec2 xy )` to get a focal point from the xy point;  For now, this should be nothing since our map will rest on `z = 0.0f`, bit uf you want to add height later, this will make you more future proof; 
- [ ] Set this as your camera's new focal point; 
- [ ] Get current zoom change and apply it to your `RTSCamera` current distance from focal point;   


## UI System 
Our UI system is based off the almighty `aabb2` and the backbone `UIWidget` (or `UIElement` if you prefer). 

`UIWidget` is the relationship and positioning structure in which all other visual widgets will inherit (some abstract UI concepts (like `UIRadioGroup` do not need to inherit from `UIWidget`). 

So we will start by implementin a `UIWidget`, making sure we can position and size tings correctly, and then move onto making a `UILabel` and `UIButton`, then finish up with the concept of a *Radio Group*. 

 ** TODO **