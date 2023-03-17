# CMakeList.txt : CMake project for Arboria, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

if(NOT DEFINED ENV{VCPKG_FEATURE_FLAGS})
    set(ENV{VCPKG_FEATURE_FLAGS} "manifests,binarycaching")
endif()

if(NOT DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit os
        if(WIN32)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-windows")
        elseif(UNIX AND NOT APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-linux")
        elseif(APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x64-osx")
        endif()
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit os
        if(WIN32)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-windows")
        elseif(UNIX AND NOT APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-linux")
        elseif(APPLE)
            set(ENV{VCPKG_DEFAULT_TRIPLET} "x86-osx")
        endif()
    endif()
endif()

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
Find_Package(GLM REQUIRED)
Find_Package(HarfBuzz REQUIRED)
Find_Package(PhysFS REQUIRED)

set (ARBORIA_SOURCE_FILES
		./src/Arboria.cpp
		./src/FileSystem.cpp
		./src/Heap.cpp
		./src/events/Event.cpp
		./src/framework/Camera.cpp
		./src/framework/Engine.cpp
		./src/framework/InputBindings.cpp
		./src/framework/InputManager.cpp
		./src/framework/ResourceManager.cpp
		./src/framework/ScreenManager.cpp
		./src/framework/String.cpp
		./src/game/scenes/MainMenu.cpp
		./src/game/scenes/OptionsMenu.cpp
		./src/gui/Label.cpp
		./src/gui/ButtonWidget.cpp
		./src/gui/Widget.cpp
		./src/gui/WidgetContainer.cpp
		./src/renderer/Font.cpp
		./src/renderer/FontManager.cpp
		./src/renderer/GlyphAtlas.cpp
		./src/renderer/Renderer.cpp
		./src/renderer/Shader.cpp
		./src/renderer/ShaderProgram.cpp
		./src/renderer/Texture.cpp
		./src/renderer/TextRenderer.cpp
		./src/main.cpp
	)
set (ARBORIA_HEADER_FILES
		./src/Arboria.h
		./src/definitions.h
		./src/globals.h
		./src/FileSystem.h
		./src/Heap.h
		./src/containers/HashTable.h
		./src/containers/List.h
		./src/events/Event.h
		./src/framework/Camera.h
		./src/framework/ConfigOptions.h
		./src/framework/Cursor.h
		./src/framework/Engine.h
		./src/framework/InputActionType.h
		./src/framework/InputBindings.h
		./src/framework/InputManager.h
		./src/framework/ResourceManager.h
		./src/framework/Screen.h
		./src/framework/ScreenManager.h
		./src/framework/String.h
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
		./src/renderer/GlyphAtlas.h
		./src/renderer/Renderer.h
		./src/renderer/RenderSystem.h
		./src/renderer/Shader.h
		./src/renderer/ShaderProgram.h
		./src/renderer/stb_image.h
		./src/renderer/stb_rect_pack.h
		./src/renderer/Texture.h
		./src/renderer/TextRenderer.h
		./src/utils/check_static_cast.h
		./src/utils/math.h
		./src/utils/Vector.h
)

add_executable (${CMAKE_PROJECT_NAME} ${ARBORIA_SOURCE_FILES} ${ARBORIA_HEADER_FILES})

If (NOT SDL2MAIN_LIBRARY)
	Set (SDL2MAIN_LIBRARY "")
EndIf (NOT SDL2MAIN_LIBRARY)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PHYSFS_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PHYSFS_LIBRARY})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${FREETYPE_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${HarfBuzz_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${HarfBuzz_LIBRARIES} ${FREETYPE_LIBRARY} OpenGL::GL ${SDL2IMAGE_LIBRARIES} ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY} GLEW::GLEW)

# TODO: Add tests and install targets if needed.
