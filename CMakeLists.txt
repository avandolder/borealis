cmake_minimum_required(VERSION 3.23)
project(borealis
    LANGUAGES CXX C
    VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(raylib 4.0.0 QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib"
        GIT_TAG 4.0.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

find_package(raylib_cpp 4.2.2 QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY "https://github.com/RobLoach/raylib-cpp.git"
        GIT_TAG v4.2.2
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()
 
find_package(EnTT 3.10.1 QUIET)
if (NOT EnTT_FOUND)
    FetchContent_Declare(
        EnTT
        GIT_REPOSITORY "https://github.com/skypjack/entt"
        GIT_TAG v3.10.1
    )
    FetchContent_MakeAvailable(EnTT)
endif()

find_package(tmx 1.2.0 QUIET)
if (NOT tmx_FOUND)
    FetchContent_Declare(
        tmx
        GIT_REPOSITORY "https://github.com/baylej/tmx"
        GIT_TAG 268f1cdb8c4811c6aac5addf7c4e7063342d3af1
    )
    FetchContent_MakeAvailable(tmx)
endif()

find_package(TCLAP 1.4 QUIET)
if (NOT TCLAP_FOUND)
    FetchContent_Declare(
        TCLAP
        GIT_REPOSITORY "git://git.code.sf.net/p/tclap/code"
        GIT_TAG 3feeb7b2499b37d9cb80890cadaf7c905a9a50c6
    )
    set (BUILD_UNITTESTS OFF)
    set (BUILD_EXAMPLES OFF)
    set (BUILD_DOC OFF)
    set (CMAKE_POLICY_DEFAULT_CMP0077 NEW CACHE INTERNAL "")
    FetchContent_MakeAvailable(TCLAP)
endif()

find_package(box2d 2.4.1 QUIET)
if (NOT box2d_FOUND)
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY "https://github.com/erincatto/box2d"
        GIT_TAG 9dc24a6fd4f32442c4bcf80791de47a0a7d25afb
    )
    FetchContent_MakeAvailable(box2d)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE raylib raylib_cpp tmx EnTT::EnTT TCLAP box2d)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()
