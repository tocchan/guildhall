- Pull CPython
- Create a PCBuild
  - ./PCBuild/Build.bat
  - Open the project, make appropriate changes for your needs; 
  - Build debug/release versions ofr "python3dll"


------
## Embedding Python Setup

```cpp
// Our Goal!
#include "script/python.h"
void PythonTest()
{
   wchar_t const* program = L"TestProgram"; 

   // presetup
   Py_SetProgramName( program ); 
   Py_SetPath( L"./python39c.zip" ); 

   // initialize the interpreter
   Py_Initialize(); 

   // do things?
   PyRun_SimpleString( 
      "from time import time,ctime\n"
      "print('Today is', ctime(time()) )"
   );

   // cleanup the system
   Py_FinalizeEx(); 
}
```

1.  Get CPython
2.  Build `pythoncore`, debug and release versions.
    - Set appropriate options that match your build; 
3.  Copy dlls/libs generated to your engine/project
4.  In code, be sure to link/import the libs
5.  Be sure to copy the dlls to the .exe directory on build (so the exe can see/load them)
6.  *Important* - Make sure the core modules are accessible.
    - Option A: Copy the `Lib` directory from `CPython` to your exe directory
    - Option B: Zip the contents of Lib to a file, and rename it the same name as your dll
      - ie: `python39_d.dll` would use the zip `python39_d.zip`
    - Option C: Same as above, but use a custom zip name so you don't have to have a debug and release one
      - Name is anything you want - I use `python39.zip`
      - Call `Py_SetPath( L"./your_zip_name.zip" )` before calling `Py_Initialize` to set the path to use; 
    - Option D: Before putting them into a zip folder, compile them.
      - Command line: `python -m compileall './Lib/' -b`
      - Go delete all the `.py` files
      - Zip up as normal

If you did options B, C, or D, you may want to put this zip file in an engine folder and add a post-build copy to move it to the same directory
as your executable.  

Library Reference: https://docs.python.org/2/library/index.html
Feel free to remove libraries not needed for your project (for example, most graphic user interface libraries don't make sense in our engine)

Recommend delete all graphical user interface and `test`, as it'll reduce file size considerably. 

*Note: Still new to this.  Not sure quite yet if there is a way to hide all required modules internally or within a compressed folder.  If anyone wants to do the digging let me know!*

*Note 2:  Recommend switching your app to a console application so you can see any python errors during startup.*

------
## Calling Files
Before we move on, we'll going to split this into a couple parts; 

```cpp
//------------------------------------------------------------------------
void PythonSystemStartup()
{
   wchar_t const* program = L"MyProgarm"; 

   // presetup
   Py_SetProgramName( program ); 
   Py_SetPath( L"./python39c.zip" ); 

   // initialize the interpreter
   Py_Initialize(); 
}

//------------------------------------------------------------------------
void PythonSystemShutdown()
{
   // cleanup the system
   Py_FinalizeEx(); 
}

//------------------------------------------------------------------------
void PythonTest()
{
  // do our tests in here
  // ...
}
```


So while running python, you are running a 'python instance', meaning everything you do affect a global 
python state.  You can spin up multiple sub instances, but for now, assume we have one, and all interactions with it must be single threaded; 

Next, let's create a python file, and make sure we can run it; 

```py
# test.py
def SomeFunction(a, b): 
  c = a + b
  print( "Sum: " + str(c) )

SomeFunction(5, 7)
```

To run this, in our sample code; 

```cpp
//------------------------------------------------------------------------
void PythonTest()
{
   char const* filename = "data/scripts/main.py"; 
   FILE* file = fopen( filename, "r+" ); 
   int auto_close = 1; 
   
   PyRun_SimpleFileExFlags( file, filename, auto_close, nullptr ); 

   PyRun_SimpleString( "SomeFunction(99,123)\n" ); 
}
```

This lets us run a single file.  `auto_close` will close the file once complete.  Notice
that once it runs, `SomeFunction` exists for us to call in subsequent `Python` calls.


------
## Manually Calling Python Functions

So next we're going to...
1. Load `py` files into our code as modules
2. Bind python functions
3. Call python functions from C

Before we start, we need to setup our python directories.  By default, only python files in the zip 
file we setup are going to be seen, so we'll edit that to include our scripts folder (and/or our current 
working directory).

```cpp
// In PythonSystemStartup
// In windows, we use semicolon seperated list.  In Linux/Mac, it
// would be a colon (according to documentation)
//
// I set it up to use the compile core modules, the root directory, and 
// then my scripts directory
Py_SetPath( L".\\;.\\data\\scripts\\;.\\python39c.zip" ); 
```

Next, the code to load it `data\scripts\test.py`

```cpp
//------------------------------------------------------------------------
void PythonTest()
{
   char const* filename = "test"; 
   char const* func_name = "SomeFunction"; 

   PyObject* py_name; 
   PyObject* py_module; 
   PyObject* py_func;

   // convert a locale filesystem string into a python string
   py_name = PyUnicode_DecodeFSDefault( filename ); 

   // load the file into an module
   py_module = PyImport_Import( py_name ); 
   if (py_module != nullptr) {
      // successfully loaded
      // all code in the file would be "run" at this point

      // think of the entire file as an "object" or namespace
      // and a function in the file as one of its attributes
      // (any global variable can be accessed in this way)
      py_func = PyObject_GetAttrString( py_module, func_name );
      if (py_func != nullptr) {
         // If you want to make sure it is callable
         if (PyCallable_Check( py_func )) {

            // make a python object to pass in the arguments (should be 
            // as what I need to pass in)
            PyObject* args = PyTuple_New(2);

            PyObject* value = PyLong_FromLong(32); 
            PyTuple_SetItem( args, 0, value );  // this "steals" the refernce of value

            value = PyLong_FromLong(20);
            PyTuple_SetItem( args, 1, value );  // set the second argument

            // call the function with this arguments
            PyObject_CallObject( py_func, args );


            // done using it, release the local reference to it
            Py_DECREF( args );    // args will release itself and all references it has
            Py_DECREF( py_func ); // done using this function
         }
   
        Py_DECREF( py_module ); 
      }
   }

   // remove a reference from the object (potenetially freeing memory)
   Py_DECREF( py_name ); 

   // error checking
   if (PyErr_Occurred()) {
      PyErr_Print(); 
   }
}
```

I could have also referenced this by doing `data.scripts.test` to access the folder from the working *exe*'s directory.  **Notice the dots and not slashes!**

Also, after running this, if you look at your scripts folder, you'll notice it made a `__pycache__` directory containing the compiled version of your script.  Be sure you don't check this in.  If you prefer not to have
the compiled versions laying around, you can specify the option `dont_write_byte_code`, see https://docs.python.org/3/library/sys.html#sys.dont_write_bytecode

### PyBind11
So this sort of binding and marshalling of data across in tuples is a pretty good candidate for some modern C++ template meta-programming, and it is fairly straight forward if you're interested in the excercise.  

Or, if you prefer, you can also use something like `pybind11` that will do a lot of it for it;  I won't be covering it in class, just want to put it out there as an option. 


------
## Calling C from Python


### PyBind11
...

------
## Visual Studio Mixed Debugger
... (may not be possible with newest verison, looking into it)

------
## Links
- https://github.com/python/cpython
  CPython code repot for building the libs/dlls
- https://docs.python.org/3/c-api/init.html
  Init functions - good notes on what can be called and when;
- https://docs.python.org/3/extending/embedding.html
  Notes on embedding python3 into a C++ application
- C Module Def Documentation
- https://docs.python.org/3/c-api/module.html