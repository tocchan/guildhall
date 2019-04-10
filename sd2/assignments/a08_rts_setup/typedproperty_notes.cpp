class BaseProperty
{
	public:
		virtual std::string ToString() const		= 0; 
};

template <typename T>
class TypedProperty : public BaseProperty
{
	public:
		TypedProperty( T const &val )
			: m_value(val)
		{}

		virtual std::string ToString() const override 
		{
			return ToString( m_value ); 
		}

	public:
		T m_value; 
};

std::string ToString( void const*const ptr ) 
{ 
	return ""; 
}


class NamedProperties
{
	public:
		// specialized typed ones; 
		template <typename T>
		T Get( std::string const &key, T const &def ) 
		{
			auto ew = m_properties.find( key ); 
			if (ew == m_properties.end()) {
				return def; 
			}

			BaseProperty *prop = ew->second; 

			TypedProperty<T> *typed_prop = dynamic_cast<TypedProperty<T>*>( prop ); 
			if (typed_prop == nullptr) {
				std::string str = prop->ToString(); 
				return FromString( str, def ); 

			} else {
				return typed_prop->m_value; 
			}

			/*
			std::string val = Get( key, "" ); 
			T value = FromString<T>( val, def );
			return value; 
			*/
		}

		template <typename T>
		void Set( std::string const &key, T const &val )
		{
			TypedProperty<T> *prop = new TypedProperty<T>(val); 

			if (key already exists) {
				delete old key; 
			}

			m_properties[key] = prop; 
		}

		// Specialization 
		void Set( std::string const &key, char const *str )
		{
			Set( key, std::string(str) ); 
		}

		std::string Get( std::string const &key, char const *def ) 
		{
			return Get<std::string>( key, def ); 
		}

		// 
		// 
		template <typename T>
		void Set( std::string const &key, T *ptr )
		{
			Set<T*>( key, ptr ); 
		}

		template <typename T>
		T* Get( std::string const &key, T *def )
		{
			auto ew = m_properties.find( key ); 
			if (ew == m_properties.end()) {
				return def; 
			}

			BaseProperty *prop = ew->second; 
			TypedProperty<T*> *typed_prop = dynamic_cast<TypedProperty<T*>*>( prop ); 
			if (typed_prop == nullptr) {
				return def;
			} else {
				return typed_prop->m_value; 
			}
		}

	public:
		std::map<std::string,BaseProperty*> m_properties; 
};

// Exmaple
void Foo()
{
	EventArgs evt;

	Map mapByValue;

	evt.Set( "map", mapByValue );   // evt.Set<Map>
	evt.Set( "map", &mapByValue );	// evt.Set<Map*> -> T == Map*
}


char const*

template <typename T>
T FromString( char const *str, T const &def )
{
	return T(str); 
}

template <>
float FromString( char const *str, float const &def )
{
	// atof
	// if that fails, return def, otherwise, return result; 
}