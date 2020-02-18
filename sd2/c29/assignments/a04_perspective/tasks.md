## Mouse Info
- We always debate whether to put the clip/hide/show functionality on `Window` or `InputContext`, and have done both ways in the past.  In the end though, I think it is more important all the info lives together then it live in the absolute correct place.  If you feel `Window` makes more sense to you, just make a note in your readme. 

- Hiding a cursor use the windows funtion: `ShowCursor`
- Clipping a cursor will need the windows calls;
  - `GetClientRect` - determines the bounds in which to clip
  - `ClipCursor` - actually causes it to click.
- `WM_ACTIVATE` is the message you can listen for to disable/reenable mouse state when the window loses/gains focus; 
- 