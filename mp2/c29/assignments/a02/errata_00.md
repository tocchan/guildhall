MP2 Assignment Corrections
------

- `Collider2D::~Collider2D` should be `protected`, not `private`.  Also just `virtual`, not pure `virtual`, so give it a body.
- `Collider2D::Destroy` and `Rigidbody2D::Destroy` should not delete the object, but just call the approprate `Physics2D::Destroy*`
  - That destroy does not `delete`, but instead mark for destruction.  Delete should only happen in `Physics2D::EndFrame`
- `Collider2D::Render` should take a `RenderContext` so it can draw.

```cpp

void Physics2D::DestroyRigidbody( Rigidbody2D* rb )
{
	Collider2D* collider = rb->GetCollider();
	if (collider != nullptr) {
		rb->SetCollider(nullptr); 
		collider->Destroy();
	}
}

Rigidbody2D::~Rigidbody2D()
{
	GUARANTEE_OR_DIE( m_collider == nullptr ); 
}
```