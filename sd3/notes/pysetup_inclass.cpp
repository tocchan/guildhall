// PyConfig line 265
/* For an MSVC DLL, we can nominate the .lib files used by extensions */
#ifdef MS_COREDLL
#       if !defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_BUILTIN)
                /* not building the core - must be an ext */
#               if defined(_MSC_VER)
                        /* So MSVC users need not specify the .lib
                        file in their Makefile (other compilers are
                        generally taken care of by distutils.) */
#                       if defined(_DEBUG)
#                               pragma comment(lib,"ThirdParty/Python/python37_d.lib")
#                       elif defined(Py_LIMITED_API)
#                               pragma comment(lib,"python3.lib")
#                       else
#                               pragma comment(lib,"ThirdParty/Python/python37.lib")
#                       endif /* _DEBUG */
#               endif /* _MSC_VER */
#       endif /* Py_BUILD_CORE */
#endif /* MS_COREDLL */

//---------------------------------------------------------------------------------------

// In Python.hpp
#include "ThirdParty/Python/Include/Python.h"
// -or-- (depending on your folder structure)
#include "ThirdParty/Python/Python.h"


//---------------------------------------------------------------------------------------
// Engine/Scripting/Python.cpp
#include "Engine/Scripting/Python.hpp" 

void PythonSystemStartup()
{
   wchar_t const* program = L"TestProgram"; 

   // presetup
   Py_SetProgramName( program ); 
   Py_SetPath( L"./python37.zip" ); 

   // initialize the interpreter
   Py_Initialize(); 

   // do things?
   PyRun_SimpleString( 
      "from time import time,ctime\n"
      "print('Today is', ctime(time()) )\n"
   );

   PyRun_SimpleString(
      "i = 100\n"
   );


   PyRun_SimpleString(
         "print(i)\n"
   );

}

void PythonSystemShutdown()
{
   // cleanup the system
   Py_FinalizeEx(); 
}


//------------------------------------------------------------------------
static PyObject* PyHookLog( PyObject* self, PyObject* args )
{
   UNREFERENCED( self ); 
   
   int arg_count = (int) PyTuple_Size(args);
   if (arg_count > 0) {
      PyObject* arg0 = PyTuple_GetItem( args, 0 );

      PyObject* str = nullptr; 
      if (PyUnicode_CheckExact(arg0)) {
         str = PyUnicode_AsEncodedString(arg0, "utf-8", "~E~");
      } else {
         PyObject* repr = PyObject_Repr(arg0);
         str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
         Py_DecRef(repr);
      }

      char const* bytes = PyBytes_AS_STRING(str);
      if (!StringIsWhitespace(bytes)) {
         LogTagf( "python", bytes ); 
      }
      
      Py_DecRef(str);
   }

   Py_INCREF(Py_None); 
   return Py_None; 
}

//------------------------------------------------------------------------
// next, describe it to python
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

/*
import squire 
squire.log()
*/

//------------------------------------------------------------------------
static PyObject* MyModuleInit() 
{
   return PyModule_Create( &MyModule );
}


void PythonSystemStartup()
{
   wchar_t const* program = L"TestProgram"; 

   // presetup
   Py_SetProgramName( program ); 
   Py_SetPath( L"./python37.zip" ); 

   // add in my modules
    PyImport_AppendInittab( "squire", &MyModuleInit ); 

   // initialize the interpreter
   Py_Initialize(); 

   // hook up stdout to my log system; 
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

   PyRun(
      "print(234)\n"
      "squire.log(124)\n"
   );
}