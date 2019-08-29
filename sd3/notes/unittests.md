Unit Tests
======

## Syllabi

## The Unit Test System

```cpp

// one option
#define MAX_TESTS 1024
UnitTest gAllTests[MAX_TESTS];
uint gTestCount = 0; 

void UnitTestsRunAllCategories( uint priority ) 
{
	uint total = 0; 
	uint passed = 0; 
	for (test : gAllTests) {
		if (test->priority <= priority) {
			++total; 
			bool succeeds = test->work_cb(); 
			if (!succeeds) {
				DebuggerPrintf( "Test ['%s'] Failed", test->name ); 
			} else {
				++passed; 
			}
		}
	}

	//  print success/failure if you fancy
	ASSERT( total == passed ); 
}

typedef bool (*test_work_cb)(); 

class UnitTest
{
	UnitTest( char const* name, char const* cat, uint priority, test_work_cb cb )
		: name(name)
		, category(cat)
		, priority(priority)
		, work_cb(cb) 
	{
		ASSERT( gTestCount < MAX_TESTS ); 

		gAllTests.push_back(this); 
	}

	//bool (*work_cb)(); 
	test_work_cb work_cb; 
	char const* name; 
	char const* category; 
	uint priority; 
}; 

static bool TestCallback()
{
	// work of my test

	return true; 
}

static UnitTest someTest( "test", "cat", pri, TestCallback ); 

```

### In Place Linked Lists & Self Registration 

```cpp
class UnitTest
{
	UnitTest( char const* name, char const* cat, uint priority, test_work_cb cb )
		: name(name)
		, category(cat)
		, priority(priority)
		, work_cb(cb) 
	{
		ASSERT( gTestCount < MAX_TESTS ); 

		next = gAllTests; 
		gAllTests = this; 
	}

	//bool (*work_cb)(); 
	test_work_cb work_cb; 
	char const* name; 
	char const* category; 
	uint priority; 

	UnitTest* next; 
}; 

UnitTest* gAllTests = nullptr; 


void UnitTestsRunAllCategories(...)
{
	UnitTest* iter = gAllTests; 
	while (nullptr != iter) {
		// do stuff; 

		iter = iter->next; 
	}
}
```

### Macro
```cpp
// put this in Common.hpp
#define MACRO_COMBINE1(X,Y) X##Y  // helper macro
#define MACRO_COMBINE(X,Y) MACRO_COMBINE1(X,Y)
```

```cpp
static bool TestCallback()
{
	// work of my test

	return true; 
}

static UnitTest someTest( "test", "cat", pri, TestCallback ); 
```

... to become...

```cpp
UNITEST( "Test Name", "cat", pri )
{
	// work
}
```

So, how do I move the body of code to the end...
```cpp
```cpp

static bool TestCallback(); 									// part of macro
static UnitTest someTest( "test", "cat", pri, TestCallback ); 	// part of macro
static bool TestCallback()										// part of macro
{	// NOT PART OF MACRO
	// work of my test

	return true; 
}

#define UNITTEST( name, cat, pri ) 	\
	static bool MACRO_COMBINE(__UnitTest_,__LINE__)(); 	\		// this expands to bool __UnitTest_123();
	static UnitTest MACRO_COMBINE(__UnitTestObj_,__LINE__)( name, cat, pri, MACRO_COMBINE(__UnitTest_,__LINE__) ); \
	static bool MACRO_COMBINE(__UnitTest_,__LINE__)() 

```



### Sorting If Desired

### Running the Tests

## Callstacks

### Stack Frames

### Disassembly

### DbgHelp.lib


