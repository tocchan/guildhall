//------------------------------------------------------------------------
//------------------------------------------------------------------------
class TypedPropertyBase
{
   public:
      virtual ~TypedPropertyBase() {}

      virtual std::string GetAsString() const = 0; 
      virtual void const* GetUniqueID() const = 0; 

      template <typename T>
      bool Is() const;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
template <typename VALUE_TYPE>
class TypedProperty : public TypedPropertyBase
{
   public: 
      virtual std::string GetAsString() const final { return ToString(m_value).c_str(); }
      virtual void const* GetUniqueID() const final { return StaticUniqueID(); }

   public:
      // std::string m_key;
      VALUE_TYPE m_value; 

   public: 
      static void const*const StaticUniqueID() 
      { 
         static int s_local = 0; 
         return &s_local; 
      }  
};


//------------------------------------------------------------------------
//------------------------------------------------------------------------
class NamedProperties
{
   public:
      //------------------------------------------------------------------------
      ~NamedProperties()
      {
         for (auto iter : m_keyValuePairs) {
            delete iter.second;
         }

         m_keyValuePairs.clear(); 
      }

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
         TypedPropertyBase* base = FindInMap(keyName); 
         if (nullptr != base) {

            // this works WITHOUT RTTI enabled
            // but will not work if prop is inherited from T
            if (base->Is<T>()) {
               // make sure this is safe!  how....?
               TypedProperty<T>* prop = (TypedProperty<T>*)base; 
               return prop->m_value; 
            } else {
               std::string strValue = base->GetAsString(); 
               return StringConvert( strValue.c_str(), defValue ); 
            }
         } else { // failed to find
            return defValue; 
         }
      }

      //------------------------------------------------------------------------
      // specialized for char const
      void SetValue( std::string const& keyname, char const* val )
      {
         SetValue<std::string>( keyname, val ); 
      }

      //------------------------------------------------------------------------
      std::string GetValue( std::string const& keyName, char const* val ) const
      {
         return GetValue<std::string>( keyName, val ); 
      }

   private: 
      //------------------------------------------------------------------------
      TypedPropertyBase* FindInMap( std::string const& key ) const
      {
         auto iter = m_keyValuePairs.find(key); 
         if (iter != m_keyValuePairs.end()) {
            return iter->second; 
         } else {
            return nullptr; 
         }
      }


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

// make generic ToString/FromString for pointer types to get
// around any template workers; 


