## General Mouse Drag Notes

- When your press the mouse button, determine the object to drag.
  - Grab the top-most object that you are overlapping
  - Keep track of what object this is.
  - Keep track of a local offset of your mouse to their center.
- While and object is selected...
  - Update its position to be your mouse position added to that saved local offset 
  - This will keep your object from "jumping" when selecting it.     
- If you delete an object, make sure you clear your selection!

## Painters' Algorithm

When selecting an object to drag - be sure to always select the top object if the mouse overlaps with multiple.

If you draw first to last, the "last" object should be showing up on top, meaning if you select and object, be sure the select the "last" object you overlap in terms of render order.

While dragging the object - the object is "locked in", and you do not select a new object until you release the one you are currently holding. 