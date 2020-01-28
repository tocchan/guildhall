## Mouse Clicks

Mouse clicks are told to the user with the following **Windows Messages**, handled by your window procedure (in MP2, this will still be in `Main_Windows.cpp`, in SD2, this is in `Window.cpp`)

- `WM_LBUTTONDOWN`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttondown
- `WM_LBUTTONUP`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-lbuttonup
- `WM_RBUTTONDOWN`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttondown
- `WM_RBUTTONUP`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-rbuttonup
- `Wm_MBUTTONDOWN`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttondown
- `WM_MBUTTONUP`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mbuttonup

All those messages contain the same data, and can be handled the same way, so I'll just do `WM_LBUTTONDOWN` as an example, though be sure to handle *all* the messages.

```cpp

case WM_LBUTTONDOWN:
   // doesn't matter which message it was, the wParam and lParam all contain the same information

   // wparam tells us which buttons are down
   bool leftButtonDown = wParam & MK_LBUTTON; 
   bool middleButtonDown = wParam & MK_MBUTTON; 
   bool rightButtonDown = wParam & MK_RBUTTON; 
   // there are others - see docs if you're intersted...

   // tell input system - this should be similar to how you 
   // handle keyboard input, but now for however many mouse buttons you want to 
   // track;  For example, this one just tracks the 3. 
   inputSystem->UpdateMouseButtonState( leftButtonDown, middleButtonDown, rightButtonDown ); 

   // break, don't return - let windows still do its default thing even though we're intercepting the message
   break; 

case ...
```

---

## Mouse Wheel

Again, this will be passed though the windows procedure; 

- `WM_MOUSEWHEEL`: https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-mousehwheel

The `wParam` in this case stores button state, similar to the button messages above, but now the **High Word**, or high 16-bits of the message store the wheel delta;

From the documentation; 

> The high-order word indicates the distance the wheel is rotated, expressed in multiples or factors of WHEEL_DELTA, which is set to 120.
> A positive value indicates that the wheel was rotated to the right; a negative value indicates that the wheel was rotated to the left.

So to get the highword, we'll shift it down 16-bits, and then devide by `120` to get the 'amount' of scroll as a float;  

```cpp
DWORD scrollFixedPoint = wParm >> 16; // shift away low word part, leaving only the highword
float scrollAmount = (float)scrollFixedPoint / 120.0f; // convert to a numeric value
inputSystem->AddMouseWheelScrollAmount( scrollAmount ); // let the input system know that the mouse wheel has moved - again, interface design is up to you - this is just a suggestion
```

*Note:  The original format of the scroll value is a form of **fixed point notation**, which is a way to store decimal values in binary by assume a common divisor.  Most fixed points use a power of two, or a number of fits for the decimal part though to make the math faster...*
 
---

## Input System
Update input system to have the interface you want, for example;

```cpp
// one option - use an enum to denote which button
bool InputSystem::WasMouseButtonJustPressed( eMouseButton btn ) { ... }

// another option - just have specialty functions
bool InputSystem::WasLeftMouseButtonJustPressed() { ... }
```

The interface you want is left up to you.  Suggest coding up A02 and 'make up' whatever feels natural as you're writing, and then fix up your interface to match; 

You will want accessors for left-click and mouse wheel by default; 