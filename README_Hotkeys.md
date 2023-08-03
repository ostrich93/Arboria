# Arboria Engine Hotkeys

In order to enable multiple controller types to be used and allow players to change their controls to their liking, the input system of the Arboria Engine translates the input from the user into a generic "action". This action is read by the engine and can translate to the execution of one or more callbacks,
depending on the current context. In most games, while a button/key press can result in two very different actions in two different contexts, it is far more common for the input to result in the same or similar actions in multiple contexts. E.g. a directional button moves a cursor up in a menu or on a grid,
a cancel input in a menu can be used to exit submenus or take back movemets in a strategy game, etc.

- **Confirm**: Confirms a selection in a menu. Sometimes, this can change the context to a new menu, change the focus to a different element, etc.
- **Cancel**: Cancels a selection in a menu, or returns to the last focused or parent widget. If the player is at the root of the ui in a scene and the scene is subordinate to a previous one (e.g the Options Menu to the Title Screen), it will pop the scene from the stack and return to the previous state. If the top scene in the stack is the only scene in such a case, then the Cancel action will do nothing.
- **Unit Menu**: The game meant to use this engine is a turn-based strategy game, and in such a game, when hovering over a unit in the game world, this action can open up a ui widget that contains the selected unit's information. However, the designer could add extra cases for this to be used and in non strategy games, this input could mean something very different.
- **Help**: In menu/ui contexts, if ui elements have descriptions associated with them, then this input will enter into *help* mode, which displays a tooltip with the element's description and allows the player to navigate to other ui widgets. This can be exited with Cancel or inputting Help again. Currently does nothing.
- **Start**: Can be used to pause, open certain menus in the game world, skip cutscenes or dialogue, and so on. Currently does nothing.
- **Directional Input**: Moves the cursor in the current menu context. If the data in a menu is part of a list, *Up* and *Down* will display the next item in the list. For menus with multiple pages, *Left* and *Right* are usually used to navigate between them. In all cases, the cursor wraps around if the index is < 0 or > length of the list.
- **LShift**: For tabbed uis, this can decrement the currently selected tab. Currently does nothing.
- **RShift**: For tabbed uis, this can increment the currently selected tab. Currently does nothing.
- **Quit**: Can exit the game altogether. Only available for keyboard configurations and not on game controllers.

## Keyboard Mappings

- **Confirm**: X
- **Cancel**: Z
- **Unit Menu**: A
- **Help**: S
- **Start**: Enter/Return
- **Directional Input**: Arrow keys
- **LShfit**: D
- **RShift**: C
- **Quit**: Escape

## Controller Mappings (Xbox 360 controller)

- **Confirm**: A
- **Cancel**: B
- **Unit Menu**: X
- **Help**: Y
- **Start**: Start
- **Directional Input**: D-Pad/Analog stick
- **LShfit**: LB/LT
- **RShift**: RB/RT
- **Quit**: None