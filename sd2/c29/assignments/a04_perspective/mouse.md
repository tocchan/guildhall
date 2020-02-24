Mouse
======

## Hide Mouse

Windows call to `ShowCursor`: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showcursor

This is a counter - so if I hide twice, show once, results in (-1), it is still hidden.

So if you want to ABSOLUTELY show the mouse (Force Show)

```
while (ShowCursor(TRUE) < 0) {}
```

`ShowCursor` is is persisted with your window - losing focus, your mouse visible state is the currently focused - very little state management on our end;

------

## Clip Cursor
Also referred to `Locking` the cursor (Unity and Unreal).  

Win32 call - `ClipCursor`: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-clipcursor

So if you want to lock to your window, first get your client rectangle;

`GetClientRect`: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclientrect

This does not use a counter - last person to call it, wins.  Desktop wide!

Be sure to unlock the cursor when you lose focus, and relock it when you gain focus;  Listen for `WM_ACTIVATE` windows message - this will tell you when you gain or lose focus; 

`WM_ACTIVATE`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-activate

------

## Relative Mode

- `Absolute Mode` - Move moves around freely - has a position.  I can position.  Can also see how far it moved since frame if it moved.  Good for pointing and clicking (RTS, Dev Console, Debugging, Editor, what have you...)

- `Relative Mode` - Only tells how far it has moved since last frame - does not have a position. 

To accomplish relative mode, during update...

```cpp

void InputSystem::UpdateRelativeMode()
{
   vec2 positionThisFrame = ::GetCursorPos(); 
   m_relativeMovement = positionThis - m_positionLastFrame; 
   // remap relative movement ()

   // move back to center
   vec2 windowCenter = window->GetCenter(); // GetClientRect() to get rectangle, find center of that
   SetCursorPos( windowCenter );

   // one little trick... without - will cause drift (maybe)
   GetCursorPos( &point ); 
   windowCenter = vec2( point.x, point.y ); 

   // recenter
   m_positionLastFrame = windowCenter; 
}

void InputSystem::SetCursorMode( eMousePositionMode mode )
{
   if (mode == MOUSE_MODE_RELATIVE) {
      // 1. locking the cursor - no reason for it to go anywhere
      // 2. hide cursor (if not hidden, it will just wiggle in the middle)
      // 3. Recenter the mouse and save off previous position
   }
}

```

Windows Calls;
- `GetCursorPos` - https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getcursorpos
- `SetCursorPos` - https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setcursorpos