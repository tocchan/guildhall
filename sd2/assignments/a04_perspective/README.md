A04: Perspective
======

*Note:  This assignment will have some visual anomalies, which we'll fix next week with Depth Buffers*

## Rubric
- [ ] **30%** Rendered 3D Spinning Cube
- [ ] **30%** Rendered 3D UV Sphere
- [ ] **10%** Depth Buffer
- [ ] **20%** Shader.XML
- [ ] **10%** Extras

If the `DevConsole` is not functional, you will lose 15% of your grade;  Recommend giving your dev console its own camera it will use for rendering; 


## Topics
- Mouse Input
  - Relative Mouse Mode
  - 4 DoF Camera
- Matrices
  - Model Matrix
  - Perspective Matrices
  - Camera & View Matrices
    - Inverting a Matrix 
- Mesh (with Index Buffers (IBO))
  - `CPUMesh`
  - `VertexMaster`
  - `GPUMesh`
- Basic Mesh Generation Ver.1
  - Box (AABB3)
  - UV Sphere
- Depth Buffers

## Additional Links
- [Squirrel: Understanding Homogeneous Coordiantes (GDC2015)](https://www.youtube.com/watch?v=o1n02xKP138):  Good talk by our very own Squirrel about homogeneous coordintes and perspective matrices; 

- [Wikipedia: Cramer's Rule](https://en.wikipedia.org/wiki/Cramer%27s_rule): Math behind the matrix inversion function - if you're curious how it work; 


## Goal(s)

**Demo App**: [./demo/a04.zip](./demo/A04.zip)

Code we want to get working; 

### Setting the Scene (Perspective Camera)

First, we want to setup the camera for perspective mode.  We will need a way to test 
that this works, so we'll also draw a quad at the origin of the world.

```cpp
class Game
{
	public:
		Camera *m_camera; 

		vec3 m_camPosition		= vec3(0.0f, 0.0f, -10.0f); // we'll start 10 back; 
		vec3 m_camEuler			= vec3(0.0f); 
		float m_camFOVDegrees	= 90.0f; 
}; 

void Game::Startup()
{
	m_camera = new Camera(); 

	// set our field of view, and our near and far plane
	// (you can not include zero, so always have near plane at least a *little* positive)
	// (test this in Unity - if in perspective mode, it won't let you set the near plane
	// to zero)
	m_camera->SetColorTarget( nullptr ); // use backbuffer; 
	m_camera->SetProjectionPerspective( m_camFOVDegrees, 0.1f, 100.0f ); 
}

void Game::Render()
{
	RenderContext *ctx = App:GetContext(); 

	// Move the camera to where it is in the scene
	// (right now, no rotation (looking forward), set 10 back (so looking at 0,0,0)
	mat44 camTransform = mat44::FromEulerZXY( m_camEuler, m_camPosition ); 
	m_camera->SetCameraMatrix( camTransform );

	// Draw something
	ctx->BeginCamera( m_camera ); 
	ctx->ClearColorTargets( rgba::BLACK ); 

	ctx->BindShader( "shader/default_unlit.hlsl" ); 
	ctx->BindTextureViewWithSampler( "white" ); 
	ctx->DrawQuad( aabb2::FromPoints( vec2(-1.0f), vec2(1.0f) ) ); // just draws a quad at the origin, that is size two in length

	ctx->EndCamera(); 
}
```

### Mesh Generation & Rendering
Next, let's give something more interesting to look at - a cube and a sphere.

*Note:  These functions are not final until we add Normals/Tangents/Bitangents.  So if you do a lot of extras pertaining to more geometry, know that they will need to be maintained until the Normal Map assignment.*

```cpp
class Game
{
	public:
		// Define the shapes, and how are they positionedin the world; 
		GPUMesh *m_cube; 
		mat44 m_cubeTransform; // cube's model matrix

		GPUMesh *m_sphere;
		mat44 m_sphereTransform;   // sphere's model matrix
};

void Game::Startup()
{
	CPUMesh mesh;

	// create a cube (centered at zero, with sides 2 length)
	CPUMeshAddCube( &mesh, aabb3::ThatContains( vec3(-1.0f), vec3( 1.0f ) ) ); 
	m_cube = new GPUMesh( ctx ); 
	m_cube->CreateFromCPUMesh( mesh, GPU_MEMORY_USAGE_STATIC ); // we won't be updated this; 

	// create a sphere, cenetered at zero, with 
	mesh.Clear();
	CPUMeshAddUVSphere( &mesh, vec3::ZERO, 1.0f );  
	m_sphere = new GPUMesh( ctx ); 
	m_sphere->CreateFromCPUMesh( mesh, GPU_MEMORY_USAGE_STATIC );
};

void Game::Render()
{
	// ...

	ctx->BeginCamera( m_camera ); 

	//...

	// Set the cube to rotate around y (which is up currently),
	// and move the object to the left by 5 units (-x)
	m_cubeTransform = mat44::FromEulerZXY( vec3(0.0f, timeSeconds, 0.0f), vec3(-5.0f, 0.0f, 0.0f) ); 
	ctx->SetModelMatrix( m_cubeTransform );
	ctx->BindTextureViewWithSampler( "image/box.png" );  
	ctx->DrawMesh( m_cube ); 

	// Set the sphere to rotate around y in the opposite direction
	// and move the object to the right
	m_sphereTransform = mat44::FromEulerZXY( vec3(0.0f, -2.0f * timeSeconds, 0.0f), vec3(5.0f, 0.0f, 0.0f) ); 
	ctx->SetModelMatrix( m_sphereTransform ); 
	ctx->BindTextureViewWithSampler( "image/earth.jpg" ); 
	ctx->DrawMesh( m_sphere ); 

	ctx->EndCamera(); 
}
```

### Depth Buffer
Upon having the above working - you will notice a visual anomaly - we can see facings we wouldn't expect to (this manifests as a cube almost looking inverted).  This is due to currently, the last thing we draw is what shows up on screen.  We're going to introduce a depth buffer; 


```cpp
void Game::Startup()
{
	// setup the camera same as before;

	m_camera->SetDepthStencilTarget( ctx->GetFrameDepthStencilTarget() ); 

	// currently we can only load from hlsl, but depth stencil
	// rules we're going to put on the shader object - so we'll precreate
	// for now, and set it up (using a Less Or Equal rule so our old stuff works)
	m_shader = ctx->GetOrCreateShader( "default_unlit.hlsl" ); 
	m_shader->SetDepth( COMPARE_LEQUAL, true ); // says only write color if depth is less-equal to whatever was already there
															  // and that we also want to update the depth when we do so; 
}

void Game::Render()
{
	// mostly everything same as before;
	// ...

	ctx->BeginCamera( m_camera ); 
	ctx->ClearColorTargets( rgba::BLACK ); 
	ctx->ClearDepthStencilTarget( 1.0f );  // we clear to 1, treating Z as going forward (hence, 1 is the farthest away)

	// for now, use the shader setup above - when we get xml loading, we can use the string name again;
	ctx->BindShader( m_shader ); 

	// ... draw scene as normal; 
	ctx->EndCamera(); 

	// ...
}
```

### Moving Around
Finally, let's allow the user to move around; 

```cpp

void Game::UpdateInput()
{
	float dt = GetFrameTime();

	// this is in pixels 
	vec2 mouse = GetMouseFrameChange(); 

	// we usually want to scale the pixels so we can think of it
	// as a rotational velocity;  Work with these numbers until 
	// it feels good to you; 
	vec2 scalingFactor = vec2( 1.0f, 1.0f ); 
	vec2 turnSpeed = mouse * scalingFactor; 

	// y mouse movement would corresond to rotation around right (x for us)
	// and x mouse movement corresponds with movement around up (y for us)
	m_camEuler += dt * vec3( turnSpeed.y, turnSpeed.x, 0.0f ); 

	// Let's fix our "pitch", or rotation around right to be limited to -85 to 85 degrees (no going upside down)
	m_camEuler.x = Clamp( m_camEuler.x, -85.0f, 85.0f );

	// Next, let's keep the turning as between 0 and 360 (some people prefer -180.0f to 180.0f)
	// either way, we just want to keep it a single revolution
	// Note: modf does not correctly mod negative numbers (it'll ignore the sign and mod them as if 
	// they were positive), so I write a special mod function to take this into account;  
	m_camEuler.y = Mod( m_camEuler.y, 360.0f ); 

	// Awesome, I now have my euler, let's construct a matrix for it;
	// this gives us our current camera's orientation.  we will 
	// use this to translate our local movement to a world movement 
	mat4 camMatrix = mat44::FromEulerZXY( m_camEuler ); 

	// compute a movement vector based on keyboard/gamepad input (your choice)
	vec3 localMovementDirection = GetLocalMovementDirection();
	localMovementDirection.ClampLength( 1.0f ); // TODO

	// figure out how fast we're going (again, based on input - have at least a walk/run option)
	float speed = GetMovementSpeed(); // shift runs, control may walk?

	// compute actual local movement
	localMovementDirection *= (speed * dt); 

	// move relative to the camera's orientation;
	// note: try not doing this to get a better understanding of why it is necessary;  
	vec3 worldMovementDirection = camMatrix.TransformDirection( localMovementDirection ); 
	m_camPosition += worldMovementDirection; 
}
```

## Shader.XML
- Be able to a file similar to this: [./shader/default_unlit.xml]('./shader/default_unlit.xml')

Be able to load in both the shader program, and initially setup the blend and depth states; 


## Extras

### Recommended
- [ ] \(X04.01: 4%) Built-In Shaders (`DefaultUnlit`, `Default`, `Invalid`)
    - `DefaultUnlit` & `Default` will be the same shader for now; 
    - [ ] \(X04.02: 2%) If a shader fails to load - it should duplicate the `Invalid` shader. 
    - [ ] \(X04.03: 2%) Invalid shader uses a screenspace checkboard pattern of yellow and magenta; 
    - [ ] \(X04.04: 3%) Support `raster` state options in the shader xml. 
- [ ] \(X04.10: 4%) Shader Database Hot-Reload 

### For Fun
- [ ] \(X04.15: 4%) Mesh Subdivide
- [ ] \(X04.20: 2%) Cube Sphere Generation
- [ ] \(X04.30: 4%) IcoSphere Generation (No UV)
    - [ ] \(X04.31: 2%) IcoSphere UVs (Can use spherical projection - there's no clean seam so  will require a wrapping sampler to work)
- [ ] \(X04.40: 2%) Plane Generation (with subdivision count)
    - [ ] \(X04.41: 4%) Surface Generation for equations of the form "vec3 f(u, v)";
    - [ ] \(X04.42: 4%) NURB Generation (can use previous)
- [ ] \(X04.50: 4%) `ShaderStage` Resource Database
    - [ ] \(X04.51: 4%) Shader Program Defines (`ShaderStage` needs to be keyed off a hash of the program and defines)
- [ ] \(X04.60: 4%) Watched Folders Resource Reloading


## Extra Information

### X04.01: Built-In Shaders
Source for these shaders should be part of Engine.  Can either have a raw string containing source, or embed the resource into the executable. 


### X04.02 and X04.03: Failure to Load
If a shader fails to load, have it use the default shader;   This should do the normal matrix transforms, and then output a solid magenta colour `float4( 1, 0, 1, 1)`.

Be sure to use a duplicate of the Invalid shader, not the Invalid shader itself, as when you reload, you don't want to accidently modify the Invalid shader; 

For X04.03, instead output yellow and magenta grid pattern, done completely in the shader.  If you do this in screen space it stands out a little more as it will not move with the camera, which looks really odd.


### X04.04: Raster States
We currently just create a raster state and use it, but there are certain effects and options that can be useful to expose; 

- **Cull Mode**:  Determines if we should render or not based on front-face setting.  Default to call back faces (triangles pointing away from us).  Good option to expose is no culling (for 2D, or just debug purposes)
- **Front Face Winding Order**:  What is the winding order to treat as front-face.  We will default to counter-clockwise.  Not a lot of reason to change this as most affects that would want to (back-face rendering) can be done using cull-mode.
- **Fill Mode**:  Defaults to `solid`, but can be changed to `wire`.  Determines what part of a triangle to render.  Either the entire of it, or just the edges of it;  useful if you want a wire-frame mode; 


### X04.10: Shader Database Hot-Reload
Tie this to either a dev console command or a function key.  

Should reload all shaders that are *not built-in* from from - in place.  So all pointers to shaders are still valid.  Useful for quickly debugging or playing with shaders;

A good addition to this task is folder watching (**X04.60**), which will reload based on the system detecting file changes; 


### X04.20: Cube Sphere Generation
`void AddCubeSphereToMesh( CPUMesh *out, vec3 center, float radius, uint iterations = 4 )`

A quick way to generate a more uniform sphere that is textured.  Generate a cube first, and subdivide the faces `iterations` times, then push out all verts along their normal to radius length.  The texture is repeated 6 times on the outside of the sphere (the 6 original faces). 

`iterations = 0` will result in just getting a cube back; 

This is easier if you have Plane generation already working. 


### X04.30 and X04.31: IcoSphere Generation

`void AddIcosphereToMesh( CPUMesh *out, vec3 center, float radius, uint iterations = 4 )`

An IcoSphere is a more uniform distribution of polygons along the outside of a sphere.  Useful more for debug purposes (rendering a wire sphere to show an area), as they are difficult to texture without doing a UV unwrap; 

Recommend generating one by starting with a icosahedron, and then subdivide to the desired resolution;  `iterations` is referring to this subdivide count.  Having `iterations = 0` will result in just getting the icosahedron back; 

See [https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/](https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/) for more information. 

To texture, ico-spheres do not have a clean seam, so you need to ues a wrapping sampler, and repeat some vertices where you notice a wrap may happen; 

See this [StackOverflow.com](https://stackoverflow.com/questions/41957890/how-to-properly-map-a-2d-image-texture-to-an-icosphere) for more information; 


### X04.40: Plane Generation
```cpp
void AddPlaneToMesh( CPUMesh *out, 
	vec3 origin, 
	vec3 right, float xmin, float xmax, 
	vec3 up, float ymin, float ymax, 
	uint xsteps = 1, uint ysteps = 0 ); 
```

This will add a plane in the world.  Use `right` and `up` how to move away from the origin.  So if you wanted to generate a quad centered in the screen that was 2-length along the edge, you would say;

```cpp
AddPlaneToMesh( &cpu_mesh, vec3::ZERO, 
	vec3::RIGHT, -1.0f, 1.0f, 
	vec3::RIGHT, -1.0f, 1.0f ); 
```

`steps` will subdivide the plane on that direction, and must be `>= 1`.  If `ysteps == 0`, set `ysteps = xsteps`.  Gives the method a nice short-hand; 

- `steps = 1` will generate a single quad
- `steps = 2` will generate 4 quads
- `iterations = 3` will generate 9 quads
- ...etc...


### X04.41: Graphing Calculator

```cpp
typedef std::function<vec3(float, float)> graph_cb; 
void AddSurfaceToMesh( CPUMesh *out, 
	vec3 origin, 
	float xmin, float xmax, uint xsteps, 
	float ymin, float ymax, uint ysteps, 
	graph_cb func ); 
```

This takes an arbitrary function/ethod of the form `vec3 foo( float x, float y )`

Given an `(x, y)` coordinate, it will generate a vec3 in space.  

`min`, `max`, and `steps` controls the inputs.  For example, if you put 0 to 10, with 10 steps, it would generate 11 points (0, 1, 2, ..., 10).  

`steps` must be `>= 1`. 

Use this to generate the surface.  Code is very similar to the plane.


## X04.42: NURBs

```cpp
void AddNURBToMesh( CPUMesh *out, 
	vec3 const *points ); 	// can assume 16 points

// optional
void AddNUBToMesh( CPUMesh *out, 
	uint width, uint height, 
	vec3 const *points );  // should be (width * height) points - so above would be a 4x4
```

Similar to the graphing calculator (in fact you can use it to do this task), generate a NURB spline; 

For more information, see [Wikipedia](https://en.wikipedia.org/wiki/Non-uniform_rational_B-spline). 


## X04.50: Shader Stage Database
This is an optimization now that you may have variations of shaders that are just state differences so you're not-recompiling multiple copies of the same shader stages. 

This is very API specific (D3D11, OpenGL, D3D12, and Vulkan store the programmable part differently), so this database is not something the game would see.  

With D3D11, we would want to store off the specific stages into a database so if multiple shaders use a stage (or even mix and match stages), we can limit the number of loaded shader stages.

The slowest part of creating a shader is compiling, and this can also have a side-effect of helping us limit state switches later (for example, switching between an alpha and transparent shader could be done without a stage switch potentially).


## X04.51: Shader Defines
Be able to define defines per stage/pass in a shader.xml.  See the example shader for example;  These can be set when compiling a shader to influence how the shader compiles (if the shader uses pre-compiler declarations like #ifdef).  Useful for creating permutations of the same shader.

With the shader database, you now need to key a "loaded shader stage" off not just the filename, but also its defines.  Probably safe to do a CRC32 hash of the strings combined with the name.


## X04.60: Watched Folders
If you have resources, you can setup your engine to watch for data files changing, and reload them; 

See: https://docs.microsoft.com/en-us/windows/desktop/fileio/obtaining-directory-change-notifications

Hook this up for Shaders so if you save a shader, it automatically recompiles and is reflected in game.  I also suggest having a game_config or dev conosle command to turn it off. 

