enum eCameraClearBit 
{
	CAMERA_CLEAR_COLOR_BIT		= (1 << 0), 		// A01
	// CAMERA_CLEAR_DEPTH_BIT		= (1 << 1), 
	// CAMERA_CLEAR_STENCIL_BIT	= (1 << 2),
};
typedef uint eCameraClearFlags;

class Camera
{
	public:
		// be able to tell our system WHERE we're rendering to
		// if set to nullptr, will use default backbuffer
		void SetColorTarget( Texture* texture );		// A02

		void SetClearMode( eCameraClearFlags flags, rgba color, float depth = 0.0f, uint8_t stencil = 0 ); // A01


	public:
		Texture* m_colorTarget = nullptr; 
};