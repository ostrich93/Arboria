# CMakeList.txt : CMake project for Arboria, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Arboria")
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

Find_Package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
Find_Package(SDL2 REQUIRED)
Find_Package(SDL2_image REQUIRED)
Find_Package(GLEW REQUIRED)

set (ARBORIA_SOURCE_FILES
		./src/Arboria.cpp
		./src/Heap.cpp
		./src/events/Event.cpp
		./src/framework/Engine.cpp
		./src/framework/InputBindings.cpp
		./src/framework/InputManager.cpp
		./src/framework/ResourceManager.cpp
		./src/framework/ScreenManager.cpp
		./src/game/scenes/MainMenu.cpp
		./src/game/scenes/OptionsMenu.cpp
		./src/gui/Label.cpp
		./src/gui/ButtonWidget.cpp
		./src/gui/Widget.cpp
		./src/gui/WidgetContainer.cpp
		./src/renderer/Font.cpp
		./src/renderer/FontManager.cpp
		./src/renderer/Texture.cpp
		./src/main.cpp
)
set (ARBORIA_HEADER_FILES
		./src/Arboria.h
		./src/definitions.h
		./src/globals.h
		./src/Heap.h
		./src/containers/HashTable.h
		./src/containers/List.h
		./src/events/Event.h
		./src/framework/Cursor.h
		./src/framework/Engine.h
		./src/framework/InputActionType.h
		./src/framework/InputBindings.h
		./src/framework/InputManager.h
		./src/framework/ResourceManager.h
		./src/framework/Screen.h
		./src/framework/ScreenManager.h
		./src/game/scenes/MainMenu.h
		./src/game/scenes/OptionsMenu.h
		./src/gui/Label.h
		./src/gui/ButtonWidget.h
		./src/gui/Widget.h
		./src/gui/WidgetContainer.h
		./src/gui/WidgetEnums.h
		./src/gui/WindowWidget.h
		./src/renderer/Font.h
		./src/renderer/FontManager.h
		./src/renderer/Renderer.h
		./src/renderer/RenderSystem.h
		./src/renderer/stb_image.h
		./src/renderer/Texture.h
		./src/utils/math.h
		./src/utils/Vector.h
)

add_executable (${CMAKE_PROJECT_NAME} ${ARBORIA_SOURCE_FILES} ${ARBORIA_HEADER_FILES})

If (NOT SDL2MAIN_LIBRARY)
	Set (SDL2MAIN_LIBRARY "")
EndIf (NOT SDL2MAIN_LIBRARY)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${FREETYPE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${FREETYPE_LIBRARY} OpenGL::GL ${SDL2IMAGE_LIBRARIES} ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY} GLEW::GLEW)

# TODO: Add tests and install targets if needed.
