
Solution was a new Empty Win32 Solution.

Pulled in log.h, log.cpp from ../common/util


Project Property Chagnes;
- "General -> Project Defaults -> CharacterSet" was changed to "Use Multi-Byte Character Set"


== NOTES == 

- I have a hardcoded "Destroy this window when Escape is pressed" inside the windows proc.  Remove this after cleanup.
