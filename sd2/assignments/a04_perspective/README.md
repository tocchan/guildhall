A04: Perspective
======

*Note:  This assignment will have some visual anomalies, which we'll fix next week with Depth Buffers*

## Rubric
- [ ] **30%** Rendered 3D Spinning Cube
- [ ] **30%** Rendered 3D UV Sphere
- [ ] **10%** Depth Buffer
- [ ] **20%** Shader.XML
- [ ] **10%** Extras

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


## Extras

- [ ] Built-In Shaders (DefaultUnlit, Default, Invalid)
      DefaultUnlit & Default will be the same shader for now; 
    - [ ] Updated Shader Error Reporting
- [ ] IcoSphere Generation (No UV)
    - [ ] IcoSphere UVs (Can use spherical projection - there's no clean seam so  will require a wrapping sampler to work)
- [ ] Plane Generation (with subdivision count)
    - [ ] Surface Generation for equations of the form "vec3 f(u, v)";
    - [ ] NURB Generation (can use previous)
- [ ] Shader Database Hot-Reload 
- [ ] ShaderStage Resource Database
    - [ ] Shader Defines (database key is now based off defines and name)


