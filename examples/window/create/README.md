Window Creation
======

![Preview Image](https://github.com/tocchan/guildhall_samples/blob/master/images/window_create.png "Example Image")

## Setup
Solution was a new Empty Win32 Solution created in Visual Studio 2015.
Pulled in log.h, log.cpp from ../common/util for basic support for printing to the Output Window


## Project Property Chagnes;
- "General -> Project Defaults -> CharacterSet" was changed to "Use Multi-Byte Character Set"


## Notes
- I have a hardcoded "Destroy this window when Escape is pressed" inside the windows proc.  Remove this after cleanup.
