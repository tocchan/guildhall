/** Our Goal is the following; 

NamedProperties args;

// be able to add values directly as that type
args.Set( "value", 1.0f ); 
args.Set( "direction", vec3( 0, 0, 1 ) ); 

// still support our old way of adding by string
args.Set( "position", "3, 2, 1" ); 


// When fetcing, we can do things like this
float val = args.Get( "value", 0.0f );             // will get 1.0f, 
float val2 = args.Get( "NotSet", 0.0f );           // will get 0.0f (or default value) since it doesn't exist
vec3 dir = args.Get( "direction", vec3::FORWARD );  // will get (0, 0, 1)

// trickier - requires some support functions to be written for all types we want to support; 
// *note to self: this adds quite of bit of template meta-magic to the code.  May not be worth supporting*
vec3 pos = args.Get( "position", vec3(0, 0, 0) );   // will try to parse the string as a vec3, if it fails, would return the default
std::string valString = args.Get( "value", "" );    // will return "1.0" as a string since it exists; 

**/


//------------------------------------------------------------------------
// Base type - what would this have to implement to support the above interfaces?
//------------------------------------------------------------------------
class NamedProperty 
{
   public:
      virtual std::string AsString() const      = 0; 
      virtual void const* GetIdentifier() const = 0; 
}; 


//------------------------------------------------------------------------
// Our Collection
//------------------------------------------------------------------------
class NamedProperties
{
   public:
      NamedProperties();
      ~NamedProperties(); // be sure to now delete all the properties!  


      std::string GetPropertyString( std::string const &name, std::string const &def = "" ); 

      

   public:
      // Template interface we want;
      template <typename T>
      void Set( std::string const &id, T const &value )
      {
         // ... TODO in class
      }

      template <typename T>
      T Get( std::string const &id, T const &defaultValue ) 
      {
         // ... TODO in class
      }



   private: 
      // supporting methods; 
      NamedProperty* GetProperty( std::string const &name ) const; 
      void SetProperty( std::string const &name, NamedProperty const *prop ); 

   public:
      std::map<std::string, TypedProperty*> m_properties; 
}; 



//------------------------------------------------------------------------
// The storage
//------------------------------------------------------------------------

// There are a few ways we can get the "type" checking in
// One is to use RTTI (Run Time Type Information) which is a compile option.  This allows for dynamic_cast to work;
// Second (which I"ll show) will use what we know about static memory to create a unique identifier for types; 

template <typename T>
class TypedProperty : public NamedProperty
{
   public:
      virtual std::string AsString() const         { return ToString( m_value ); } // using duck typing - this will fail if we don't implement a "ToString" for a type stored here
      virtual void const* GetIdentifier() const    { return &s_Id; }

      static void const* GetTypeIdentifier()       { return &s_Id; }

   public:
      static T m_value; // actual value being stored here; 

   private: 
      static int s_Id; // doesn't matter what the value is - the *address* is unique; 

}; 

