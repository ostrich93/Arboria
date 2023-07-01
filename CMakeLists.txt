# CMakeList.txt : CMake project for Arboria, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

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

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

option(ASAN_ENABLED "Build this target with AddressSanitizer" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

Find_Package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
Find_Package(SDL2 CONFIG REQUIRED)
target_compile_definitions(SDL2::SDL2 INTERFACE SDL_MAIN_HANDLED)
Find_Package(SDL2_image REQUIRED)
Find_Package(GLEW REQUIRED)
Find_Package(GLM REQUIRED)
Find_Package(HarfBuzz REQUIRED)
Find_Package(PhysFS REQUIRED)

set (ARBORIA_SOURCE_FILES
		./src/FileSystem.cpp
		./src/Heap.cpp
		./src/containers/HashIndex.cpp
		./src/events/Event.cpp
		./src/framework/ActionManager.cpp
		./src/framework/Camera.cpp
		./src/framework/CVariableSystem.cpp
		./src/framework/Engine.cpp
		./src/framework/InputManager.cpp
		./src/framework/Lexer.cpp
		./src/framework/RenderDevice.cpp
		./src/framework/ResourceManager.cpp
		./src/framework/ScreenManager.cpp
		./src/framework/String.cpp
		./src/framework/SystemCVars.cpp
		./src/framework/Token.cpp
		./src/framework/UIContext.cpp
		./src/game/scenes/MainMenu.cpp
		./src/game/scenes/OptionsMenu.cpp
		./src/gui/Label.cpp
		./src/gui/ListBoxWidget.cpp
		./src/gui/ScrollAreaWidget.cpp
		./src/gui/ScrollbarWidget.cpp
		./src/gui/SliderWidget.cpp
		./src/gui/Widget.cpp
		./src/renderer/DrawColoredPrimitiveShader.cpp
		./src/renderer/DrawSurfaceShader.cpp
		./src/renderer/DrawTextShader.cpp
		./src/renderer/Font.cpp
		./src/renderer/Renderer.cpp
		./src/renderer/ShaderProgram.cpp
		./src/renderer/Texture.cpp
		#./src/renderer/TextRenderer.cpp
		./src/utils/Color.cpp
		./src/utils/Rectangle.cpp
		./src/main.cpp
)
set (ARBORIA_HEADER_FILES
		./src/definitions.h
		./src/globals.h
		./src/FileSystem.h
		./src/Heap.h
		./src/containers/HashIndex.h
		./src/containers/HashTable.h
		./src/containers/List.h
		./src/events/Event.h
		./src/framework/ActionManager.h
		./src/framework/Camera.h
		./src/framework/ConfigOptions.h
		./src/framework/Cursor.h
		./src/framework/CVariableSystem.h
		./src/framework/Engine.h
		./src/framework/Exception.h
		./src/framework/InputActionType.h
		./src/framework/InputManager.h
		./src/framework/Lexer.h
		./src/framework/RenderDevice.h
		./src/framework/ResourceManager.h
		./src/framework/ScreenManager.h
		./src/framework/Session.h
		./src/framework/String.h
		./src/framework/SystemCVars.h
		./src/framework/Token.h
		./src/framework/UIContext.h
		./src/game/scenes/MainMenu.h
		./src/game/scenes/OptionsMenu.h
		./src/gui/Label.h
		./src/gui/ListBoxWidget.h
		./src/gui/ScrollAreaWidget.h
		./src/gui/ScrollbarWidget.h	
		./src/gui/SliderWidget.h
		./src/gui/Widget.h
		./src/gui/WidgetEnums.h
		./src/renderer/DrawColoredPrimitiveShader.h
		./src/renderer/DrawSurfaceShader.h
		./src/renderer/DrawTextShader.h
		./src/renderer/Font.h
		./src/renderer/Renderer.h
		./src/renderer/ShaderProgram.h
		./src/renderer/stb_image.h
		./src/renderer/Texture.h
		#./src/renderer/TextRenderer.h
		./src/utils/check_static_cast.h
		./src/utils/Color.h
		./src/utils/combine_hash.h
		./src/utils/math.h
		./src/utils/Rectangle.h
		./src/utils/Vector.h
)

#set(ARBORIA_LIST_SOURCE_FILES
#	./src/Heap.cpp
#	./src/main.cpp
#	./src/framework/String.cpp
#)

#set(ARBORIA_LIST_HEADER_FILES
#	./src/definitions.h
#	./src/globals.h
#	./src/Heap.h
#	./src/containers/List.h
#	./src/framework/String.h
#	./src/utils/math.h
#)

#add_subdirectory("assets")

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable (${CMAKE_PROJECT_NAME} ${ARBORIA_SOURCE_FILES} ${ARBORIA_HEADER_FILES})
if(ASAN_ENABLED)
  if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC /fsanitize=address /GR)
  else()
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -fsanitize=address)
    target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fsanitize=address)
  endif()
  set(ASAN_SAVE_DUMPS, "ArboriaCrash.dmp")
endif()
#add_executable (${CMAKE_PROJECT_NAME} ${ARBORIA_LIST_SOURCE_FILES} ${ARBORIA_LIST_HEADER_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PHYSFS_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PHYSFS_LIBRARY})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${FREETYPE_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${HarfBuzz_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${HarfBuzz_LIBRARIES} ${FREETYPE_LIBRARY} OpenGL::GL ${SDL2IMAGE_LIBRARIES} SDL2::SDL2 GLEW::GLEW)

# TODO: Add tests and install targets if needed.
