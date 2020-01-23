//------------------------------------------------------------------------
// PRAGMA TODO 
// Idea from my friend Micheal Bosley
// Source from http://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml
// Stringify the argument; 
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)

#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define DEBUGBREAK()  __debugbreak()
#define PRAGMA(p)  __pragma( p )
#define NOTE( x )  PRAGMA( message(x) )
#define FILE_LINE  NOTE( __FILE__LINE__ )

// THE IMPORANT BITS
#define TODO( x )  NOTE( __FILE__LINE__"\n"           \
        " --------------------------------------------------------------------------------------\n" \
        "|  TODO :   " ##x "\n" \
        " --------------------------------------------------------------------------------------\n" )
#define FIXME( x )  NOTE(  __FILE__LINE__"\n"           \
        " -=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n" \
        "|  FIXME :  " ##x "\n" \
        " -=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n" )
#define todo( x )  NOTE( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  NOTE( __FILE__LINE__" FIXME:   " #x "\n" ) 

// Some additional helpers
#define UNIMPLEMENTED()          TODO( "IMPLEMENT: " QUOTE(__FILE__) " (" QUOTE(__LINE__) ")" ); DEBUGBREAK()
#define UNIMPLEMENTED_MSG(msg)   TODO( "IMPLEMENT '" msg "': " QUOTE(__FILE__) " (" QUOTE(__LINE__) ")" ); DEBUGBREAK()

#define UNIMPLEMENTED_RETURN(v)  UNIMPLEMENTED(); return v