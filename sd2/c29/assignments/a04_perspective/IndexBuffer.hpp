class IndexBuffer : public RenderBuffer
{
	public:
		void Update( uint icount, uint const* indices ); 
		void Update( std::vector<uint> const& indices ); // helper, calls one above
}
