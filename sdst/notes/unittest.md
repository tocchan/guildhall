Simple Unit Test Framework
======

*Note: Uses [Self Registration](../selfregistration.md)*


## End Goal

```cpp
// Math/Utils.cpp
#include "Core/Debug/UnitTest.hpp"

UNIT_TEST(clamp_test, "filter")
{
	ASSERT( Clamp01( 4.2f ) == 1.0f ); 
	ASSERT( Clamp01( -2.3f ) == 0.0f ); 
	ASSERT( Clamp01( .4f ) == .4f );

	return true; 
}

// main.cpp
void main()
{
	SystemInit(); 

	RunUnitTests(nullptr);

	InitRendering();
	RunUnitTests("render");  
}
```

## How Do?

What is the function signature?
`typedef bool (*test_cb)(); `

How does `RunUnitTests` work?
```cpp

```

What is `UNIT_TEST` doing?
```cpp
// UnitTest.hpp
struct UnitTestRegistration
{
 	UnitTestRegistration( char const *name, create_cb cb, char const *filter );
};

#define UNIT_TEST( test_name, filter ) 	\
	static bool UnitTestFunction_##test_name(); 			\
	static UnitTestRegistration g_testRegObj_##test_name( #test_name, UnitTestFunction_##test_name, filter );  \
	static bool UnitTestFunction_##test_name() 

void RunUnitTests( std::string const &filter );

// UnitTest.cpp
struct UnitTest
{
	char const *m_name; 
	char const *m_filter; 
	test_cb m_callback; 
};

static uint const MAX_UNIT_TESTS = 2048; 
static UnitTest gAllUnitTests[MAX_UNIT_TESTS];
static uint gUnitTestCount = 0U; 

UnitTestRegistration::UnitTestRegistration( char const *name, create_cb cb, char const *filter ) 
{
	if (gUnitTestCount >= MAX_UNIT_TESTS) {
		__debugbreak(); // yo, go up MAX_UNIT_TESTS
	} else {
		UnitTest &test = gUnitTests[gUnitTestCount]; 
		++gUnitTestCount; 

		test.m_name = name; 
		test.m_filter = filer;
		test.m_callback = cb; 
	}
}

void RunUnitTests( std::string const &filter )
{
	for (uint i = 0; i < gUnitTestCount; ++i) {
		UnitTest &test = gUnitTests[i]; 
		if ((test.m_fitler == filter) && !test.m_callback()) {
			LogErrorf( "Unit Test '%s' failed", test.get_name() ); 
		} 
	}
}
```

Using it (how it break apart)
```cpp

// Code to register a unit, not macro
static bool TestWork();
static UnitTestRegistration g_testRegObj( "test_name", TestWork, "some_filter" ); 
static bool TestWork()
{
	// run tests; 
	return true; 
}



UNIT_TEST( foo, "whatever" ) 
{

}

// static bool UnitTestFunction_hey
UNIT_TEST( hey, "thing" )
{

}
