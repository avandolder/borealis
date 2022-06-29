cmake_minimum_required(VERSION 3.23)
project(borealis CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib"
    GIT_TAG 4.0.0
    OVERRIDE_FIND_PACKAGE
)
 
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY "https://github.com/skypjack/entt"
    GIT_TAG v3.10.1
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    tmx
    GIT_REPOSITORY "https://github.com/baylej/tmx"
    GIT_TAG 268f1cdb8c4811c6aac5addf7c4e7063342d3af1
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    TCLAP
    GIT_REPOSITORY "https://git.code.sf.net/p/tclap/code"
    GIT_TAG 3feeb7b2499b37d9cb80890cadaf7c905a9a50c6
    OVERRIDE_FIND_PACKAGE
)

find_package(raylib)
find_package(EnTT)
find_package(tmx)
find_package(TCLAP)

file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

add_library(EnTT INTERFACE)
add_library(TCLAP INTERFACE)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib tmx INTERFACE EnTT TCLAP)
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall -Wextra -pedantic)
target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
