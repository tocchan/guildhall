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
   Py_SetPath( L"./python37c.zip" ); 

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

1.  Get CPython (3.7 if you want to use mixed mode debugging in Visual Studio)
2.  If pulling and building yourself, run `./PCBuild/build_env.bat` to build the solution
3.  Build `pythoncore`, debug and release versions.
    - Set appropriate options that match your build; 
4.  Copy dlls/libs generated to your engine/project
5.  In code, be sure to link/import the libs
6.  Be sure to copy the dlls to the .exe directory on build (so the exe can see/load them)
7.  *Important* - Make sure the core modules are accessible.
    - Option A: Copy the `Lib` directory from `CPython` to your exe directory
    - Option B: Zip the contents of Lib to a file, and rename it the same name as your dll
      - ie: `python37_d.dll` would use the zip `python37_d.zip`
    - Option C: Same as above, but use a custom zip name so you don't have to have a debug and release one
      - Name is anything you want - I use `python37.zip`
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
   Py_SetPath( L"./python37c.zip" ); 

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
Py_SetPath( L".\\;.\\data\\scripts\\;.\\python37c.zip" ); 
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

### Adding a Module
Most typically, we'll expose all the functions of our engine before we call `PyInitialize` by constructing modules; 

Below is an example of us exposing my engine's `Logf` function to Python...

1. Define the C function I want python to call `PyHookLog`
2. Define all the methods I want to expose as part of this module - `MyPyMethods` global
3. Define the module definition - `MyModule`
4. Provide a function for creating this module. 
   - **IMPORTANT**: The memory for this module/method **does not** get copied, and must stay valid for the lifetime of your python interpreter instance.
5. Before `PyInitialize`, register the module into the system.

```cpp
//------------------------------------------------------------------------
// Function I want Python to be able to call
// "self" is the object it is being called on
// "args" is the tuple of arguments being passed in
// @return is value returned by method (can be nullptr)
//
// (don't worry about references unless you need to hold onto something
// in which case add a method manually
static PyObject* PyHookLog( PyObject* self, PyObject* args )
{
   UNREFERENCED( self ); 
   // int arg_count = (int) PyTuple_Size(args);
   
   char const* str; 
   int success = PyArg_ParseTuple( args, "s", &str );
   if (!success) {
      LogWarningf( "PythonLog failed to pass a string" ); 
   } else {
      if (!StringIsWhitespace(str)) {
         LogTagf( "python", str ); 
      }
   }

   // return nullptr is an error, 
   // so always return something
   // there is a global called "Py_None", but be sure to add a reference
   Py_INCREF(Py_None); 
   return Py_None; 
}

//------------------------------------------------------------------------
// define the methods that will be in my exposed module
static PyMethodDef MyPyMethods[] = {
   // python func name,     C Func to Call,     Type,           Help Message
   { "log",                 PyHookLog,          METH_VARARGS,   "Logs using the system log." }, 
   { nullptr,               nullptr,            0,              nullptr }  // terminator element; 
};

//------------------------------------------------------------------------
// define the module
static PyModuleDef MyModule = {
   // base,                name,       documentation,    size,          methods,       slots,   tarverse_cb,   clear_cb,   free_cb
   PyModuleDef_HEAD_INIT,  "squire",   nullptr,          -1,            MyPyMethods,   nullptr, nullptr,       nullptr,    nullptr 
};


//------------------------------------------------------------------------
static PyObject* MyModuleInit() 
{
   return PyModule_Create( &MyModule );
}
```

Registering the module; 
```cpp
 // register my modules
 PyImport_AppendInittab( "squire", &MyModuleInit ); 

 // initialize the interpreter
 Py_Initialize();
 ``` 

### `PyCFunction_New`
Other option is you could create functions on the fly and add them to the global scope (or module scope)

So, same as before, we have to define our C Function...

```cpp
static PyObject* CFuncTest( PyObject* self, PyObject* args ) 
{
   UNREFERENCED( self ); 
   UNREFERENCED( args ); 

   Logf( "Test Called" ); 

   Py_INCREF(Py_None); 
   return Py_None; 
}

static PyMethodDef TestFunc = { "testfunc", CFuncTest, METH_NOARGS, nullptr }; 
```

But unlike before, this happens *after* `Py_Initialize`.  You can also add functions to other objects
by adding it to their dictionary instead of the global dictionary (may have to create it as a method - did not test)

```cpp
// Get the "__main__", module (or any module you want - main is just a special name for base/global module) 
PyObject* m = PyImport_AddModule("__main__"); 

// Create a function, and add it to the global dictionary of that object
PyObject* func = PyCFunction_New( &TestFunc, nullptr );
PyObject* dict =  PyModule_GetDict( m ); 
PyDict_SetItemString( dict, TestFunc.ml_name, func ); 

// release the reference to our function (dictionary will hold onto it)
Py_DecRef( func ); 

// we don't release "dict" or "m", as they are "borrowed references" - ie, don't add a reference on return
 ```

------
## Visual Studio Mixed Debugger
... (may not be possible with newest verison, looking into it)

- Install `Python development` with Visual Studio 2019, and make sure `Python native development tools` is selected
  - If you don't have it installed - go to *Tools -> Get Tools and Features* to run Visual Studio Installer
- When you build your dll/libs, be sure to save off the symbol files (or remember where they built), so you can point Visual Studio toward them
  - *Tools -> Options :: Debugging -> Symbols*
- It may request symbols.  You can install this with your python distribution, or if you build local, add this directory;
  - https://docs.microsoft.com/en-us/visualstudio/python/debugging-symbols-for-mixed-mode-c-cpp-python?view=vs-2019

If everything worked - you should be able to hit breakpoints in your python scripts.  


------
## Questions I Had (and answers I found)
*Note: Not sure if these are the good answers, but they worked, so we'll go for it for now;*

Q. Can I create an object from C++
A. Yes.  Most basic types (like longs, bools, etc) have a constructor.  Complex types you fetch the object type name (like `Entity`) and call it like a function,
   it'll return the new instance of the project.

Q. How do I forward output from Python to something in game?
A. `sys.stdout` in python determines where things go.  It can be any object with a `write` method that takes a string.  Just replace it with one that fowards
   to your logging system.

```cpp
  // initialize the interpreter
   Py_Initialize(); 

   // hook up stdout to my log system;
   // "squire" is my engine's module 
   PyRun_SimpleString( 
      "import sys\n"
      "import squire\n"
      "class LogOutput:\n"
      "   def write(self, txt):\n"
      "      squire.log(txt)\n"
      "   def flush(self):\n"
      "      pass\n"
      "logOutput = LogOutput()\n"
      "sys.stdout = logOutput\n"
      "sys.stderr = logOutput\n"
   );
```

Q. How to associate a local C++ object (`void*` for example) with a PythonObject?
A. There are two options I was able to find; 

   1. Use a `PyCapsule` - an object which is meant to be an opaque type (this `PyObject` is this local object) 

   2. Add an interger value and just store the pointer there; 
      ```cpp
      // setting it; 
      PyObject* pyobj; 
      void* ptr; 
      PyObject* ptr_obj = PyLong_FromLongLong( (long long)ptr ); 
      PyObject_SetAttrString( pyobj, "this", ptr_obj ); 
      Py_DecRef( ptr_obj ); // I'm no longer holding onto it; 

      // Now, fetching it later; 
      PyObject* obj; 
      PyObject* ptr_obj = PyObject_GetAttrString( obj, "this" ); 
      if ((ptr_obj != nulltpr) && PyLong_Check(ptr_obj)) {
         void* pointer =  (void*) PyLong_AsLongLong(ptr_obj);
         Py_DecRef( ptr_obj );

         return pointer;
      } else {
        return nullptr; 
      } 
      ```

Q. How do I export a "class" from C++
A. See: https://docs.python.org/3/extending/newtypes_tutorial.html
   Recommend using `pybind11` or similar to make this simpler;  

Q. When do I need to increment/decrement references
A. Assume any create needs to be released by you, except when the documentation explicitly states that a function steals a reference.
   *Note: This is **NOT** obvious.  `PyTuple_SetItem` steals a reference, but `PyObject_SetItem` does not!  Fired.*

Q. Can I embed python modules in memory and have python import from there?
A. **NO IDEA** - Could find no resources on it, outside of seeing Panda3D preload a binary object before initialize; 

------
## Links
- CPython code repot for building the libs/dlls
  https://github.com/python/cpython

- Init functions - good notes on what can be called and when;
  https://docs.python.org/3/c-api/init.html

- Notes on embedding python3 into a C++ application: 
  https://docs.python.org/3/extending/embedding.html
  
- C Module Def Documentation: 
  https://docs.python.org/3/c-api/module.html

- More Information on Exporting C to Python (including classes/objects)
  http://www.speedupcode.com/c-class-in-python3/

- Official Documentation on Creating Types from C/C++
  - https://docs.python.org/3/extending/newtypes_tutorial.html
  - https://docs.python.org/3/extending/newtypes.html

- Some motivation for using a scripting language
  https://www.gamasutra.com/view/feature/131372/gdc_2002_game_scripting_in_python.php