
// standalone C function?
// StringUtils.hpp (Squirrel... I think...)
// parse.h (Forseth... I'm pretty sure...)
float GetValueFromString( std::string const& str, float const& defValue ); 
RGBA GetValueFromString( std::string const& str, RGBA const& defValue ); 
FloatRange GetValueFromString( std::string const& str, FloatRange const& defValue );
// ...

std::string ToString( float value ); 
std::string ToString( RGBA value ); 
std::string ToString( FloatRange const& range ); // "a~b"


class TypedPropertyBase
{
   public:
      virtual ~TypedPropertyBase() {}

      virtual std::string GetAsString() const = 0; 
      virtual void const* GetUniqueID() const = 0; 

      template <typename T>
      bool Is() const 
      {
         return GetUniqueID() == TypedProperty<T>::UNIQUE_ID; 
      }

};

template <typename VALUE_TYPE>
class TypedProperty : public TypedPropertyBase
{
   public: 
      virtual std::string GetAsString() const final { return ToString(m_value); }
      virtual void const* GetUniqueID() const final { return UniqueID; }

   public:
      // std::string m_key;
      VALUE_TYPE m_value; 

   public: 
      static int s_Whatever; 
      static void const*const UNIQUE_ID = &s_Whatever;  
};

TypedProperty<int>::UNIQUE_ID != TypedProeprty<float>::UNIQUE_ID


class NamedProperties
{
   public:
      //------------------------------------------------------------------------
      ~NamedProperties()
      {
         // TODO
         TEXT TO BE AN ERROR SO YOU DONT FORGET TO DO THIS!
         // make sure you delete all the properties here
         // now... cause you newed them
      }

      //------------------------------------------------------------------------
      void PopulateFromXMLAttributes( XMLElement const& element ); 
      void PopulateFromKeyValuePairs( std::string const& str ); // key=value key1="some string" key2=value3

      //------------------------------------------------------------------------
      void SetValue( std::string const& keyName, std:;string const& valueName ); 

      //------------------------------------------------------------------------
      // base string version
      std::string GetValue( std::string const& keyName, std::string const& defValue ) const; 

      //------------------------------------------------------------------------
      // for everything else, there's templates!
      template <typename T>
      void SetValue( std::string const& keyName, T const& value )
      {
         TypedPropertyBase* base = FindInMap(keyName); 
         if (base == nullptr) {
            // doesn't exist, make a new one            
            TypedProperty<T>* prop = new TypedProperty<T>(); 
            prop->m_value = value;
            m_keyValuePairs[keyName] = prop; 

         } else {
            // it exists?
            if (base->Is<T>()) {
               TypedProperty<T>* prop = (TypedProperty<T>*)base; 
               prop->m_value = value; 
            } else {
               // not the same thing
               // delete and remake
               delete base; 

               // doesn't exist, make a new one            
               TypedProperty<T>* prop = new TypedProperty<T>(); 
               prop->m_value = value;
               m_keyValuePairs[keyName] = prop; 
            }
         }
      }

      //------------------------------------------------------------------------
      template <typename T>
      T GetValue( std::string const& keyName, T const& defValue ) const
      {
         TypedPropetyBase* base = nullptr; 
         auto iter = m_keyValuePairs.find( keyName ); 
         if (iter != m_keyValuePairs.end()) {
            base = *iter; 

            // this works WITHOUT RTTI enabled
            // but will not work if prop is inherited from T
            if (prop->GetUniqueID() == TypedProperty<T>::UNIQUE_ID) {
               // make sure this is safe!  how....?
               TypedProperty<T>* prop = (TypedProperty<T>*)base; 
               return prop->m_value; 
            } else {
               std::string strValue = prop->GetAsString(); 
               return GetValueFromString( strValue, defValue ); 
            }
         } else { // failed to find
            return defValue; 
         }
      }

   private: 
      // this is going to be different
      // std::map<std::string, std::string> m_keyValuePairs;
      // std::map<std::string, void*> m_keyValuePointers;

      // We need to type type in a map
      // But we can't store the temlate argument... or can we?
      std::map<std::string,TypedPropertyBase*> m_keyValuePairs; 
};




// we want to support
NamedProperties args;
args.SetValue( "value", 1.0f / (float)(2 ^ 23) ); 
float value = args.GetValue( "value", 0.0f ); 

args.SetValue( "model", m_transform.GetModelMatrix() ); 
mat44 model = args.GetValue( "model", mat44::IDENTITY );  // this should succeed
transform3 transform = args.GetValue( "model", transform3::IDENTITY );  // this should fail

// How do you write, SetValue and GetValue for this to work?
Actor* pointerToObject = m_player; 
m_rigidbody.m_userArgs.SetValue( "owner", pointerToObject ); 
Actor* owner = args.GetValue<Actor*>( "owner", nullptr );      // succeed
Material* mat = args.GetValue<material*>( "owner", nullptr );  // fail


