void App::Startup()
{
   SubscribeEvent( "help", ShowHelp ); 

}

void DevConsole::Startup()
{
   DefineEvent( "help", ShowHelp, "Displays all available commands", EVENT_IS_VISIBLE_TO_DEVCONSOLE_BIT );
}

void DevConsole::RunCommand( char const* command )
{
   EventType* type = gEventSystem->FindEvent( command ); 
   if ((type != nullptr) && type->CanRunFromConsole()) {
      FireEvent( command );
      // type->Fire(command);  
   }
}

void EventSystem::GetOrCreateEvent( name ); 

class EventType
{
   std::string name; 
   std::vector<Callback> callbacks;
   std::string description = "";  
   uint flags        = 0; 
}

// Run a command
void DevConsole::RunCommand( char const* command ) 
{
   if (EventSystemIsCommandDevConsoleVisible( command )) {
      FireEvent( command, FROM_DEVCONSOLE_BIT );
   }
}

void ShowHelp()
{
   foreach (defined event that is visible to dev consoel) {
      Print event info; 
   }
}



// 1. You need subscribe event

void SubscribeEvent( name, callback ); 


EventSystem::EventSystem()
{
   SubscribeAllKnownEvents(); 
}

void EventSystem::SubscribeAllKnownEvents()
{
   for (event_registrar in gRegistrarList) {
      SubscribeEvent( event_registrar.name, event_registrar.callback );
   }
}

struct EventRegistrar
{
   char const* name; 
   Callback callback; 

   EventRegistrar( char const* name, Callback cb )
      : name(name)
      , callback(cb)
   {

   }
};

// when do constructors call?
// - when they enter scope

// when do global constructors call?
// - when program starts -> before 'main'
// - after program is loaded into memory  
//   - code memory
//   - data memory
//   - after ALL data has been setup 

// top of EventSystem.cpp (source file)
static uint constexpr MAX_REGISTERED_EVENTS (128)

static EventRegistar* gRegistrarList[MAX_REGISTERED_EVENTS];
static uint gRegistrarCount = 0; 

EventRegistrar::EventRegistrar( char const* name, Callback cb )
   : name(name)
   , callback(cb)
{
   gRegistrarList[gRegistrarCount] = this; 
   ++gRegistrarCount; 
}


static void SomeMethod( NamedStrings& str ); 
static EventRegistrar gRegisterSomeMethod( "some_method", SomeMethod ); 
static void SomeMethod( NameStrings& str )
{
   // implement me; 
} 

COMMAND( some_method ) 
{
   // implementation
}

// DO NOT USE THIS IN A HEADER
// Macro is defined in header
// Commands are always defined in a cpp
#define COMMAND( name ) \
   static void name##_impl( NameStrings& args ); \                // static void some_method_impl( NamedStrings& args );
   static EventRegistrar name##_register( #name, name##_impl ); \ // static EventRegistrar some_method_register( "some_method", some_method_impl ); 
   static void name##_impl( NameStrings& args )                   // static void some_method_impl( NamedStrings& args )



a.cpp
#include "common.hpp"   // COMMAND in this

b.cpp   
#include "common.hpp"

"help"