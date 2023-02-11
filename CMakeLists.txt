# CMakeList.txt : CMake project for Arboria, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Arboria" CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory("src")

# Add source to this project's executable.
add_executable (Arboria "Arboria.cpp" "Arboria.h" "src/containers/List.h" "src/containers/List.cpp" "src/definitions.h")

# TODO: Add tests and install targets if needed.
