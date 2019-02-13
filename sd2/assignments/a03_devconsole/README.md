SD2-A03:  Developement Console
======

## Goal
Get D3D11 renderer to feature parity, and use this to finish out the development console; 

## Rubric
- [ ] **40%**: `RenderContext` SD1 Refactor Complete [Dev Console Renders!]
  - If text is rendering - that is usually enough to see you did everything correctly; 
- [ ] **50%**: DevConsole base features are implemented
- [ ] **10%**: DevConsole Extras;
    - Make note of which extras you attempted.

## Final RenderContext Example

```cpp

void Game::Render()
{
	RenderContext *ctx = App::GetRenderContext();

	ctx->BeginCamera( m_camera ); 

	ctx->BindShader( "shader/default_unlit.hlsl" ); 
	ctx->BindTextureViewWithSampler( 0, "image/someImage.jpg" ); 

	// draw a quad wiht a texture, tinted light blue and with 75% opacity. 
	std::vector<VectorPCU> quad; 
	AddVerticesForQuad( &quad, aabb2::FromPoints( vec2(-10.0f), vec2(10.0f) ), rgba( .5f, .5f, 1.0f, .75f ) ); 
	ctx->DrawVertexArray( quad ); 

	ctx->EndCamera(); 
}

```

## Steps

### Finish Renderer
- [ ] Implement `Texture2D` - this replaces how you were 'loading' a `Texture` from SD1
- [ ] Implement `TextureView2D` - this replaces how you were 'using' a `Texture` from SD1
- [ ] Replace `RenderContext::GetOrCreateTexture` with `RenderContext::GetOrCreateTextureView` 
- [ ] Implement `Sampler`
- [ ] Refactor `GetOrCreateTexture` to `GetOrCreateTextureView2D`
- [ ] Implement `BindtextureViewWithSampler` and whatever functions it depends on; 
    - [ ] Implemnet `BindTextureView`
    - [ ] Implement `BindSampler`
    - [ ] Implement any convenience methods you feel you'll want for now (can always implement more later), but at least implement the one used in the example; 
- [ ] Compile new `default_unlit.hlsl` shader (found [here](./shader/default_unlit.hlsl))
- [ ] Do a test run with above code - should be able to see textures
- [ ] Implement blending on the Shader
- [ ] Text with an alpha enabled texture; 


### Dev Console Base Feature Set
- [ ] Implement the DevConsole input to spec;  
  - [ ] See `WM_CHAR` windows message for capturing user input - forward this to your `DevConsole`
      - This is only for text input.  You will still use your normal input system for control keys (arrows, etc...)
  - [ ] There should be a visible carot (line showing where text will appear).  
      - [ ] The carot blinks (every quarter to half second usually looks okay)
  - [ ] *Arrow Keys* move the carot.
  - [ ] *Backspace* removes the character BEFORE the carot.  It no text is before the carot, it does nothing
  - [ ] *Delete* removes the character AFTER the carot.  If no text is after the carot, this does nothing
  - [ ] Enter will submit the command and clear the input field; 
      - [ ] Enter on an empty line should do nothing; 
  - [ ] Pressing escape will clear the input field. 
  - [ ] If input field is already empty, Escape should close the dev-console; 

### Dev Console Bonus Features
The last 10% of the grade is from optional tasks.  Some are highly recommended, but all of guildhall can be done without them.  

*Note:  Extras are usable on any assignment with an **Extras** section.  So if there are dev console features you want, but don't have time, you may do them on a later assignment instead of the listed extras there at a 50% point penalty (ie, an extra worth 8 points here, is only worth four on a future assignment).  I do this so you can pick things that interest you - please don't use it as a way to just get out of work you don't want to do.*

*Note 2: Extras tend to have a lower point-per-hour work return than required tasks.*

#### Highly Recommended
- [ ] X03.01: [5%] AutoComplete
- [ ] X03.02: [4%] Command History
- [ ] X03.03: [3%] Persistant Command History (requires X03.02)
- [ ] X03.04: [5%] Text Selection 

#### Recommended
- [ ] X03.10: [4%] Cut, Copy & Paste from Window's Clipboard (Requires X03.04)
- [ ] X03.11: [2%] Text Scrolling
- [ ] X03.12: [2%] Scroll Bar (Requires X03.11)
- [ ] X03.13: [6%] Contextual Auto Complete (Requies X03.01)

#### Fun Suggestions
- [ ] X03.20: [4%] Text Color Markup
- [ ] X03.21: [2%] Command Scripts
- [ ] X03.22: [2%] Mouse Input
- [ ] X03.23: [4%] Mouse Selection (Requires X03.04 and X03.22)
- [ ] X03.24: [2%] Navigation Hotkeys
- [ ] X04.25: [2%] Audio Cues
- [ ] X05.26: [2%] Open & Close Animations (Slides Open for Example)


## Bonus Feature Notes
*Note:  My engine does not support all the features, so if you need a reference for a feature let me know and I'll point you in the right direction if my engine doesn't support it yet.*


### Auto Complete
When typing, show a list of commands that match the text you have written so far.

This list should highlight one of the options, and allow the user to select the other options using arrow keys. 

*Note: if you have text selection - you could place the remaining part of the word in a selection area to make it a little more intuitive to use.*

If the user presses "Enter", run the selected command.  If you press "Space", complete the word add a space, and leave the cursor there.  If 
the user presses "Escape", close the autocomplete list (and remove selection if doing that).  

*Note:  This adds another case to the escape key in additional to the two required.*

Outside of that, normal input rules should be followed, with the auto-complete list updating to reflect the current input.  If no commands match the input, do not show the autocomplete list. 


### Command History
Keep a history of commands that have been run.  Pressing "up" or "down" will cycle the input to one of those previous commands, withou up going to the last command entered, and down going to the first command entered; 

**Do not keep two entries in the history directly next to each other** - besides that, you are free to implement the history to match your preferred console of choice.  

For example, my console removes duplicate entires, and only keeps the latest one.  Though some people prefer history stays, and the command is duplicated both as the last command, and at some point in history. 

Either add a `MAX_HISTORY` config to control how long the history can get, or, if you prefer to have no history limit, add the command "clear_history" so the user may manually clean it.  Make note of which option you picked. 


### Persistant Command History
Working off the **X03.02**, have the command history persist between runs of your program.  I recommend just saving it to file when the program shuts down, or even right after running a command (better once you have threading).  When the program boots up, load the command history back in.


### Text Selection
Allow user to select a piece of text - allowing the user to delete it all in one go.  This feature is required for a few different extras. 

If the user is holding **SHIFT** while pressing **LEFT** or **RIGHT**, a selection is formed between where the selection started and the currenet carot position (note that the selection could be going forward or backward).  

While having a selection, the following input changes; 
- Pressing **LEFT** or **RIGHT** without **SHIFT** will result in the carot moving to the beginning or end of the selection, and the selection is stopped.
- Pressing **UP** or **DOWN** stops the selection, leaving the cursor at the initial position.
- Pressing **DELETE** or **BACKSPACE** will remove the selected text, leaving the cursor in the spot where the selection was. 


### Cut, Copy & Paste
Allow the user to copy the current selection to, or paste text from the Windows Clipboard.  

See `GetClipboardData` and `SetClipboadData` in MSDN.  Do appropriate error checking (don't try to paste non-text for example).  These commands require other windows calls to work. 

Recommend adding this functionality to a "Engine/Platform/Clipboard.hpp" style file, as you may want to use this functionality in different systems; 

Hotkeys for this follow convention;
- **CTRL+C** - Copy.  Does nothing to the text, but copies the selection to the clipboard.  If no selection is present, copy the entire line. 
- **CTRL+V** - Pastes clipboard text as cursor.  If there is a selection, it is replaced by the clipboard text. 
- **CTRL+X** - Cuts.  Does a copy, then removes the selected text and selection. 


### Text Scrolling & ScrollBar 
Allow user to scroll up to see previous lines that may have left the screen.

If new text appears - the starting line shown should stay the same *unless* you are already at the bottom, in which case it should auto-scroll to the end again (maintain current functionality). 


The following hotkeys are required; 
- **PAGE-UP** - move the starting visible line by the max number of visible lines, minus 2 (so the user can see the same top line at the bottom after a scroll).  You should always at least show one line of text. 
- **PAGE-DOWN** - Same but in reverse.  Should always show as much text as possible when reaching the end
- **CTRL+UP & CTRL+DOWN** - Move one line at a time; 
- **CTRL+HOME & CTRL+END** - Go to the beginning/end of your console history.
- **MOUSE WHEEL** - optional, but if you have mouse, let the mouse wheel control scrolling; 


### Contextual Auto Complete
*Note: This one is fairly tricky, and it may require some refactoring depending on how far you want to go with it*. 

Allow auto complete of arguments to a command.  This means you should expose meta-data about the options/arguments your command takes.  

If an argument is being typed - auto-complete with list of matching available arguments for the command.  If the argument is a path, show available options in the current relative directory only; 

So say I have a folder structure that is; 

- ./shader/debug.hlsl
- ./shader/debug_lit.hlsl"
- ./shader_errors.txt

If I was to type "./sha", the only two options should be "./shader/" and "./shader_log.txt".  One I get into "./shader/", then the options would become 
"debug.hlsl" and "debug_lit.hlsl".  

Both file and argument autocomplete must work for this extra to count. 


### Text Color Markup
Be able to markup text printed to the dev console so you can color parts of it;  The markup style is up to you, though I tend to use a variant of **BBCode**. 

So for example, if you were to print to your console in yellow...

"Command [0000ff]net_join[-] requires an address."

Would print everything in yellow, except the "net_join" which would be blue. 

If you prefer a different markup, that is up to you, I like this one since it is very compact. 


### Command Scripts
Be able to run text files as if they were a series of commands but running a command such as;

`run_script  some_file.commands`

It should treat each line as if it were a command.  Ignore lines that start with a '#' (hash) symbol, as that will denote a commented out line.  


### Mouse Input
While the console is open, you should show the mouse, and disable mouse input in the game;

You can left-click the mouse to position the carot (it should behave logically, ie, the carot goes to the most obvious postion for the current mouse position).


### Mouse Selection
You can use the mouse by left-clicking and dragging to make selections.  


### Navigation Hotkeys
Add the following convienience keys to your dev console;  (you may add more, but these are the ones required for the points)

- **HOME** - move carot to beginning of the line
- **END** - move carot to the end of the line
- **CTRL+LEFT** - move carot to beginning of current word.  If already at the beginning of the word, or in white space, go to beginning of previous word. 
- **CTRL+RIGHT** - move carot to end of current word.  If already at the end of the word, or in white space, go to end of next word. 


### Audio Cues
Add sound effects for when a command is submitted, and when a command has an error or does not exists.  Be subtle about it, you don't want to drive yourself crazy with extremely aggressive sound design. 

Should at least have two sounds.  If you want to add more (key typed, etc...) that is up to you. 


### Open & Close Animations
When opening and closing the dev console - do more than just have it appear and disapear. 

This one actually gets a little easier when you have seperate render targets, but there is still a few options with what you have; 

- Slide-In/Out:  Have the console appear from off screen and slide into position.  I recommend an ease-out curve for entering, and an ease-in curve for exiting. 
- Fade-In/Out: Apply an overall alpha to things rendered.  Really simple using an extra uniform buffer and custom shader for for your dev console; 

