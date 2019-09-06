SD3.a01 - Unit Test Framework & Callstacks
======

## Grading
- [ ] 80%: Unit Tests
    - [ ] 20%: Will have you create a single UNITTEST in a source file and set a breakpoint.  I want to see this breakpoint hit. 
    - [ ] 10%: Will change the priority to a high number, and confirm it still runs
    - [ ] 10%: Change your test run priority to a lower number, and the test should not run.
    - [ ] 10%: Give your test a category and low priority again, and it should not run. 
    - [ ] 10%: Add a line to run tests of that category - breakpoint should hit again. 
    - [ ] 20%: Cause the test to fail.  The failed test should be logged to output, and a breakpoint hit once all tests have run. 
- [ ] 20%: Callstack 
    - [ ] 10%: Can get a callstack.  Call `GetCallstack` from your above unit test, returns 
    - [ ] 10%: Can convert a callstack to a human readable format. 

## Motivation
As a system gets more complicated, we would like to have a system in place to maintain that our systems work as expected.  So when we write a system, we'd
also write some `UNITTEST`s to test it.  If someone then changes that system, they know the system will be tested automatically so we don't accidently break the code base due to an unexpected consequence.  This is especially important for Engine systems that can break in very subtle ways.

Another nice habit to get into is that if a `UNITEST` was missing (ie, you had a bug that wasn't caught by a test), you write a test to help you debug that case, and then just *leave the test in*.  

Most `UNITTEST`s should be fairly quick, but sometimes you may have longer tests (testing a memory allocator usually takes a few seconds.  Testing a replay system may take a few minutes).  This gives us motivation to have a way to dial in which tests we want to run.  We may have a nightly build run a full suite of tests, but have developers only run high priority or fast tests.  We can accomplish this filtering using a combination of categories and priorities.   

Making a `UNITTEST` should be quick and easy so people will want to use the system.  This means modifying as few files as possible, and requiring very little code outside of the test itself to be written.  

Finally, it should be easy to compile them out (ie, not run them).   This is as easy as changing the `UNITTEST` macro based on a predefine, or changing the body of the `UnitTestRun...` calls.  

### Callstack
As the `UNITTEST` framework should not take an extremely large time to write and test.  So throwing in being able to get a `Callstack` from code and logging it to round it out; 

Implement the following: [./Callstack.hpp](./Callstack.hpp).

This will be more a excercise in learning an API, so I'm just going to give you a starting point of the functions/libraries you will need; 


## Goal
Goal of this assignment is to implement the following [./UnitTest.hpp](./UnitTest.hpp). 

This will allow you to declare a `UNITTEST` in any *SOURCE* file (not header), and have it automatically be picked up and run
when the user asks to runs tests.

A `UNITTEST` will return `true` if it passes, or `false` if it fails.  `UnitTestRun...` should run all tests, and log each test that failed.  If there are any failures, it should `ASSERT_CONTINUE` at the bottom to let the user know a test failed. 

This should allow you to do things like this...

```cpp
// In Math/Prime.cpp for example...
// Declare a test called "IsPrime Test", in the general category, at the highest (always) priority.
UNITTEST( "IsPrime Test", nullptr, 0 )
{
	if (IsPrime(7) == false) {
		return false; 
	}

	if (IsPrime(4) == true) {
		return false; 
	}

	return true; 
}

// In Math/Vector2
UNITTEST( "Vector2 Test", nullptr, 0 )
{
	CONFIRM( Vector2::FromAngle(0) == Vector2(1, 0) ); 
	CONFIRM( Vector2::FromAngle(90) == Vector2(0, 1) ); 

	return true; 
}
```

Then, somewhere when your program starts up, you can do...

```cpp
// in App.cpp for example
void App::Startup()
{
	// do all system startup
	// ...

	UnitTestsRunAllCategories(); 
}
``` 

With an output that looks similar to the following;

```txt
Unit Tests 'default' Category.
- 'Vector2 Test' Failed
1/2 Tests Passed...
```

### Categories
The purpose of categories is that some tests can not run until a certain point.  For example, if you had a test that you wanted to run once a level was loaded,
you may create a category `"OnLevelStart"` and then call `UnitTestRunAll("OnLevelStart")` when you want those ones to run.  Rendering is another good example of not 
being able to run the tests until certain rendering systems are setup.   

You can also use this when working in a team to have tests are specific to you or your sub-team.  For example using the `"net"` category to do network tests that aren't needed for everyone, but are for people making modifications to net code.  

### Priorities
Priorities are just another form of filter.  Usually used as a way to say about how long a test will take.  Fast tests get low priorities (run more often).  Slow test get higher priority numbers, and may run only at night or during an integration so as to not slow down a build.  

## Callstack Library 
So how you get a callstack depends heavily on your compiler, since it is making assumptions about how the code is constructed, so this code is going to be fairly platform dependent.  

In Windows, we will be using the function [`CaptureStackBackTrace`](https://docs.microsoft.com/en-us/windows/win32/debug/capturestackbacktrace).  This should just be part of windows. 

### Stringify A Callstack
This is going to be using the `DbgHelp` library, which knows how to use your projects `pdb` file to convert execution pointers to symbols (ie, function/file names).  This is a little more involved as we will be using the [`DbgHelp` library](https://docs.microsoft.com/en-us/windows/win32/debug/dbghelp-functions).

You will need to include the `DbgHelp.lib` library into compilation.
*Note: I load this from dll and have not tested the lib.  If you run into problems with the lib let me know!*

We care mostly about symbols, so the functions you want to look into are; 
- `GetCurrentProcess`:  Not part of `DbgHelp`, but most of the following functions require a process ID, and this is how you get it; 
- `SymInitialize`:  Initializes the system we want to use.  Just needs to be called sometime before you try to stringify a callstack
- `SymCleanup`:  Cleanup the system.  Optional, but good habit to be into. 
- `SymFromAddr`:  Convert a callstack pointer into symbol.  Pay special attention to the documentation for `SYMBOL_INFO`!  This is how you get your function name. 
- `SymGetLineFromAddr64`:  Given an address, will give you the file and line from where this is called.  

Try to stick to the format;
`filepath.cpp(line,offset): function_name`

For example; 
`Engine/Image.cpp(127,8): Image::LoadFromFile`

## Extensions
- `CONFIRM` can be changed to automatically log the test that condition that failed, as well as break.  This makes it quicker to figure out exactly which part of the test failed and put the coder there to quickly re-walk it.  