Renderable
======

```cpp
class Renderable 
{	
	public: 
		virtual ~Renderable() {}
		virtual void PreRender( Entity const *ent ) = 0; 
		virtual void Render( RenderContext *ctx ) = 0; 
};

class IsoSpriteRenderable : public Renderable
{
	public:
		// how does this figure out WHICH sprite to render?
		virtual void PreRender( Entity const *ent ) override
		{
			m_position = ent->get_position(); 
			m_sprite = ent->m_animator->GetCurrentFrame(); 
		}

		virtual void Render( RenderContext *ctx ) override 
		{
			// does not change from A2
			// ...
		}

	public:
		// render sprite
		vec3 m_position; 
		IsoSprite *m_sprite; 
};

class MeshRenderable : public Renderable
{
	public:
		virtual void PreRender( Entity const *ent ) 
		{
			// update material parameters

			m_transform = mat44::Translation( ent->get_position() ); 
		}

		virtual void Render( RenderContext *ctx ) 
		{
			ctx->BindModelMatrix( m_transform ); 
			ctx->BindMaterial( m_material ); 
			ctx->DrawMesh( m_mesh ); 
		}

	public:
		// how does this figure out transform; 
		mat44 m_transform; 
		GPUMesh *m_mesh; 
		Material *m_material; 
};

class Entity
{
	public:
		Renderable *m_renderable; 
};

void Entity::Render( RenderContext *ctx ) 
{
	m_rendreable->PreRender( this ); 
	m_renderable->Render( ctx ); 
	// render a sprite

	// render a model?

	// rendering an iso sprite
	// ...
}
```

## EXTRA - DON'T DO THIS (yet)

```cpp

class ParticleEmitterRenderable : public MeshRenderable
{
	public:
		virtual void PreRender() override { /* fill mesh with current frames particles*/ }
		// virtual void Render( RenderContext *ctx ) override; // basically just mesh version
};

enum eRenderQueue : int
{
	OPAQUE = 0,
	ALPHA = 1000,
	OVERLAY = 2000, 
};

class Shader 
{
	public:
		int renderQueue; // renderPriority; 
}; 


class ForwardRenderer 
{
	// version V0
	void Render( RenderContext *ctx ) 
	{
		ctx->BeginCamera( cam ); 

		// sort all renderables by layer (queue)
		// if opaque, render opaque, don't care order

		// if layer is alpha, all things in same layer sort by depth by camera, then render
		RenderAllOpaque();
		RenderAlphaBackToFront();

		ctx->EndCamera(); 

		ApplyFullScreenEffects(); 
	}

	IsoSpriteRenderable* CreateIsoSpriteRenderable(); 
	MeshRenderable* CreateMeshRenderable(); 
	void DestroyRenderable( Renderable *r ); 

	// version V0
	std::vector<Renderable*> m_renderables; 
}
```
