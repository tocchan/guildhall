```cpp

// rectangle2 - rect2
// aabb2

// What do we have?
class SpriteSheet {

}

class SpriteDefinition {
	// rect2 m_pixelCoords; // (0,0, 128, 128)
	Texture2D *m_texture; 
	rect2 m_uvs;    // (0, 0) -> (0.125, 1)
	vec2 m_dimensions; 
	vec2 m_pivot; 
};


void DrawBillboardSprite( vec3 position, SpriteDefinition *sprite, Camera *camera ) 
{
	// tl - tr
	// |     | 
	// bl - br
	vec3 corners[4]; 
	vec2 uvs[4]; 

	float width = sprite->GetWidth();
	float height = sprite->GetHeight();
	vec2 pivot = sprite->GetPivot(); 

	// technically right
	vec3 right = camera->GetRight();
	vec3 up = camera->GetUp();

	corners[0] = position + height * up; 
	corners[1] = position + height * up + width * right; 
	corners[2] = position; 
	corners[3] = position + width * right;

	vec2 localOffset = -(pivot * vec2(width, height)); 
	vec3 worldOffset = localOffset.x * right + localOffset.y * up;  
	// vec3 worldOffset = (vec4( localOffset, 0, 0 ) * camera->GetCameraMatrix()).xyz();

	// offset so pivot point is at position
	for (uint i = 0; i < 4; ++i) {
		corners[i] += worldOffset; 
	}

	sprite->GetUVs( uvs ); 

	ctx->BindTexture( sprite->GetTexture() ); 
	ctx->DrawQuad( corners, uvs ); 
}

class IsoSpriteDefinition
{
	std::vector<SpriteDefinition*> m_sprites; 
	std::vector<vec3> m_directions;

	SpriteDefinition* GetSpriteForLocalDirection( vec3 dir ) 
	{
		uint bestIndex = 0; 
		float bestValue = Dot( dir, m_directions[0] ); 

		for (uint i = 1; i < m_directions.size(); ++i) {
			float value = Dot( dir, m_directions[i] ); 
			i f(value > bestValue) {
				bestIndex = i; 
				bestValue = value; 
			}
		}

		return m_sprites[bestIndex]; 
	}
};

void DrawBillboardedIsoSprite( vec3 position, vec3 entityForward, IsoSprite *isosprite, Camera *cam )
{
	vec3 entityForwardRelativeToCamera = TransformDirection( entityForward, cam->GetViewMatrix() ); 
	SpriteDefinition *sprite = isosprite->GetSpriteForLocalDirection(entityForwardRelativeToCamera); 
	DrawBillboardSprite( position, sprite, cam ); 
}

```