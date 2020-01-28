```cpp
class Camera
{
	public:
		// ...

		// can use this to determine aspect ratio
		void SetOutputSize( vec2 size ); 

		// where is the camera.  With orthographic 
		// and no ration, this is effectively where you are looking
		// - ie, the screen will be centered here.
		// - note: ignore z for now
		void SetPosition( vec3 position ); 

		// sets orthographic so that your screen is "height" high.  
		// can determine min and max from height, aspect ratio, and position
		void SetProjectionOrthographic( float height, float nearZ = -1.0f, float farZ = 1.0f );

		// If you use the above functions, you have all the information you need for this
		// and you're a little more future proof for SD2
		vec2 ClientToWorldPosition( vec2 clientPos ); 


		// helpers
		float GetAspectRatio() const; 

	private: 
		// used internally for now - these will eventually go away
		// but for now you can compute these from the values set above
		// to use in ClientToWorldPosition
		vec2 GetOrthoMin() const; 
		vec2 GetOrthoMax() const; 

	public:
		vec2 m_outputSize; 
		vec2 m_position; 	

		// other changes from SD2 once you merge - ie, clear mode
};
```


## Client to World

```cpp
vec2 Camera::ClientToWorldPosition( vec2 clientPos )
{
	vec2 worldPos; 
	vec2 outputDimensions = /*window dimensions*/;

	// for now, you have an orthoMin and orthoMax.
	// so...
	// 1. Convert clientPos to pixel coordinates to normalized coordinates (0, 1) 
	//    by range mapping them from your client size to (0,1) in both dimensions.
	// 2. Convert to your ortho position by rangemapping the normalized coordinate 
	//    from (0,1) to (min, max).

	// note: you could skip and just RangeMap from (clientSpace to orthoSpace), but the additional
	// step is good practice for something coming up in SD2

	// note 2: client space is y-down, but your world space is y-up, be sure to 
	// take this into account

	// These TODOs are notes on things that will change in this function
	// as MP2 & SD2 advance;
	// TODO - take into account render target
	//        clientPos being the pixel location on the texture

	// TODO - use projection matrix to compute this

	// TODO - Support ndc-depth paramater for 3D-coordinates, needed for ray casts.

	// TODO - take into account viewport

	return worldPos; 
}
```
