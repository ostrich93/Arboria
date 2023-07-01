# Arboria Engine
The Arboria Engine is an open-source 2D game engine I began developing during my time at Recurse to learn and master low-level programming. As such, the engine is written in C and C++14 and built with CMake. It uses OpenGL
for rendering and SDL2 for cross-platform window/input/audio handling, though as of now, Windows is the primary OS.

Current Goals
-------------
As of now, I am aiming to complete the first major milestone of the project, which will consist of the following:

* A title screen displaying a background image, the project name, and a list of selectable options for the user.
	* The user can interact with the list through keyboard or controller input. The currently selected option is indicated by a cursor drawn next to it; using the arrow keys or joystick/control stick/dpad changes which option is selected.
		Using the 'Confirm' key/button executes the action assigned to the selected option.
	* The list of options are 'New Game', 'Continue', 'Options', and 'Quit'.
		* 'New Game' and 'Continue' both output a string to the console window due to there being no actual gameplay for now.
		* 'Quit' terminates the program.
		* 'Options' displays a new GUI and gives it the focus
* The Options Menu, which consists of a series of horizontal tabs atop a window with options corresponding to the currently selected tab.
	* The tabs can be navigated using 'LShift'/'RShift' keys/buttons (on a controller, this corresponds to the L/R shoulder buttons). They consist of the following:
		* Display: Enables user to change the screen mode (Fullscreen or Windowed), the screen size, gamma value, etc.
		* Input: Enables user to change the key bindings or button bindings by selecting either the 'Change Key Bindings' or 'Change Buttons' options:
			* These options will disable tab shifting and the window will display a table of bindings that correspond to various actions. When the user selects the row, they are asked to input a key or button to assign to that
			action. After the input, the mapping will change and if the chosen key/button has an existing binding, that binding will take the old key/button. These bindings will be saved by selecting 'Apply Changes'
			* The user can also select options to restore the default key bindings or the default button bindings.
		* There will be a tab for Audio configuration, and a tab for game specific configurations when those systems are implemented.
	* The user can return to the title screen by hitting the 'Cancel' key/button.
	* The configurations are to persist between sessions: if a user changes key bindings in one session, then the game will retain those same bindings the next time the program is run.
