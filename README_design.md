# Arboria Engine Architecture and Design

## Input System

The architecture for the input system can be divided into two layers: the low-level layer and the high-level layer.

The low-level layer handles all raw inputs from the player by leveraging SDL_PollEvent. If the event is a key/button release, an joystick axis motion, closing the window, or quitting, the event gets translated into a more generic format for input events and pushed into an event queue. Each event in the queue is processed and consumed in the main loop by the high-level layer.

The high-level layer dequeues the last event in the event queue and sends it to the current ui context for processing. Though the exact processing methods are implementation specific, most - if not all - of them send the event to their root ui widget to handle, which operates on the Chain of Responsiblity behavioral principle. 
The event is passed to the deepest active child in the ui tree, which sends the input data to the ActionManager to find a matching hotkey binding. If a binding is found and there is a callback assigned to it in the widget, the callback will execute, and then widget specific processing will occur. If there is no further processing to be done,
the event will be marked as *handled*, which informs the parent widget to turn on its *dirty* flag not trigger any callbacks. The *dirty* flag essentially says that a widget's render state has been updated and should be passed into the rendering pipeline. When the render is complete, the *dirty* flag is turned off, and the widget can process events again.

Because the events in the event queue are processed in a FIFO manner, the queue is implemented through the use of a **ring buffer**. This data structure does not need to shuffle its contents when an element is consumed, meaning that with a well-sized queue, operations on it are in constant-time, making it the ideal choice.

## GUI System

The UI system is centered around a **stack of contexts**, with one active at a time. This is because of the ability to open submenus or cancel and exit out of a menu to return to a previous one with the previous state maintained. However, there are also situations where we may want to clear out all previous contexts when a new one is made, or where we want to directly swap the
current context with a new context. To handle such operations, a command system is used, which performs the appropriate operation(s) depending on the command type.

Much like the DOM tree in HTML, a UIContext is composed of a tree of ui elements, known as **widgets**, which have their own properties, children, etc. and come in different types.

All widgets have the following properties:
	- An array of callback functions, each index representing a specific action type.
	- A prerender function callback
	- Image data (if any)
	- Background and foreground colors
	- Border color
	- Border size
	- Selection color (the color used when the cursor hovers over the widget)
	- Max scaling value (default is 1)
	- Name
	- Children
	- Rectangle containing position and size
	- Flags

The flags may be one of the most important parts of a widget. Not only do they describe the state of the widget but they can inform the render system on how it should be drawn. For example, the VISIBLE flag means the widget can be rendered, INTERACTIVE means that it can process user input, and DIRTY means that the widget has changed and needs to be rerendered.
The flags can also potentially be used to prioritize what widgets should be rendered over others or which ones have focus, but as of this writing, the mechanisms where these are important have not been fully implemened.

## Render System

## CVariable System

**Console Variables** (**CVars**) are mainly used to hold variables that can be accessed and changed from a console or that can be read from or be written to configuration files. They can also be used to hold global variables, such as default values for configurable values. Their presence can aid significantly in the debugging process, and they are the backbone of the configuration
system. The Options Menu in particular revolves around manipulating these variables.