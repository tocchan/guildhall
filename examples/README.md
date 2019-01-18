# Examples
======

Repository contains bare-bones examples of API specific operations (Win32, D3D11, WinSock, etc...).  These are not code pieces you should just throw into your own project - but just the minimal (readable) amount of code required to accomplish that specific action.

## Compilation Notes
All examples are compiled with VS2015 Community Edition.

## Project Layout
Each project is self contained and does not rely on any other project.  All projects are contained within the "guildhall_samples.sln" solution.  Just set the project you want to run as the Startup Project, compile, and run.  

A most projects will contain the following files;
- __main.cpp__: Entry point for the program.  Basic setup and then calls DemoRun();
- __demo.cpp__: Contains code most relevant to the lesson.  
- __demo.inl__: Old code that is part of demo.cpp, but not related to the task.  Moved here to keep demo.cpp as short as possible.

Projects are categorized under folders that most relate to what they accomplish.  
- __windows__: Win32 API Examples
- __d3d11__: DirectX D3D11 Examples
- __common__: Snippets of code I use in multiple projects or one off projects.

## Topics 

### D3D11 
1. [window/create](https://github.com/tocchan/guildhall_samples/tree/master/window/create)
2. [d3d11/setup](https://github.com/tocchan/guildhall_samples/tree/master/d3d11/setup)
3. [d3d11/shader](https://github.com/tocchan/guildhall_samples/tree/master/d3d11/shader)
4. [d3d11/texture](https://github.com/tocchan/guildhall_samples/tree/master/d3d11/texture)

## Project Descriptions
###Windows
- [__window/create__](https://github.com/tocchan/guildhall_samples/tree/master/window/create): Creates a simple fixed sized window. 
- [__window/displays__](https://github.com/tocchan/guildhall_samples/tree/master/window/displays): Example how to get desktop resolution using Win32 API
