# Arboria Engine

The Arboria Engine is an open-source 2D game engine I began developing during my time at Recurse to learn and master low-level programming. As such, the engine is written in C and C++14 and built with CMake. It uses OpenGL for rendering and SDL2 for cross-platform window/input/audio handling, though as of now, Windows is the primary OS.

## Table of Contents

* [Building](#building)
	* [Building on Windows](#building-on-windows)
* [Updates](#updates)
* [Roadmap](#roadmap)
* [Development Notes](#development-notes)

## Building

The Arboria Engine is built using several libraries, including:

* [OpenGL](https://www.opengl.org/) (Version 3.3)
* [SDL2](https://www.libsdl.org) (For input handling and window handling)
	* [SDL2-image](https://wiki.libsdl.org/SDL2_image/FrontPage)
* [GLEW](https://github.com/nigels-com/glew) (Cross-platform C/C++ library that provides OpenGL core and extension functionality)
* [GLM](https://glm.g-truc.net) (Math library)
* [physfs](https://icculus.org/physfs/) (Library for reading data from directory trees or from archive file formats such as .iso files)
* [harfbuzz](https://harfbuzz.org/)
* [FreeType](https://freetype.org/)

### Building on Windows

* Install Visual Studio (2017 or later)
* Install Git
* Checkout ArboriaEngine from Github
* The dependencies are included separately. Install [Vcpkg](https://github.com/Microsoft/vcpkg) and integrate with Visual Studio. If you prefer manual installation, run the following commands:

	* x64 builds:

```cmd
vcpkg install sdl2 sdl2-image opengl glew glm freetype harfbuzz physfs --triplet=x64-windows
```

	* x86 builds:

```cmd
vcpkg install sdl2 sdl2-image opengl glew glm freetype harfbuzz physfs --triplet=x86-windows
```

* Open the Arboria directory in Visual Studio or generate a new project with CMake.
* Set your configuration to x64-Debug or x86-Debug
* If Vcpkg was not integrated with Visual Studio, go to `CMakeSettings.json` and set `CMAKE_TOOLCHAIN_FILE` to `<path to vcpkg>\\scripts\\buildsystems\\vcpkg.cmake`
* Build Arboria

## Updates
* Basic main loop implemented. Input is read and passed into active scenes and/or UI elements to be processed appropriately.
* Context/Scene switching. Player can switch between the Title Screen and Options Menu.
* User Input and Hotkey Binding implemented. Keyboard and Game Controller inputs are linked to generic button/axis "actions" and it is a simple task to change the configurations if needed.
* UI system and elements are implemented and integrated with Scenes/Contexts.
* UI interaction and callbacks successfully implemented. Based on the UI and context, players can navigate menus and select options.

## Roadmap
* Rendering Engine
* Multi-platform support for Linux/Apple architectures
* Lua Scripting
* Audio mixing
* File compression
* Improved resource loading
* More robust debugging
* Options Menu

## Development Notes

For info on the design and architecture of the Engine, go to [README_design.md](/README_design.md)

For info on the hotkey mapping, go to [README_Hotkeys.md](/README_Hotkeys.md)
