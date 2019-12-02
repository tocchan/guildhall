- Pull CPython
- Create a PCBuild
  - ./PCBuild/Build.bat
  - Open the project, make appropriate changes for your needs; 
  - Build debug/release versions ofr "python3dll"


------
## Embedding Python Setup

```cpp
// Out Goal!
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
## Manually Calling Python Functions

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