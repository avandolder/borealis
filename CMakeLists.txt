cmake_minimum_required(VERSION 3.23)
project(borealis
    LANGUAGES CXX C
    VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib"
        GIT_TAG 4.0.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY "https://github.com/RobLoach/raylib-cpp.git"
        GIT_TAG v4.2.2
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()
 
find_package(EnTT QUIET)
if (NOT EnTT_FOUND)
    FetchContent_Declare(
        EnTT
        GIT_REPOSITORY "https://github.com/skypjack/entt"
        GIT_TAG v3.10.1
    )
    FetchContent_MakeAvailable(EnTT)
endif()

find_package(tmx QUIET)
if (NOT tmx_FOUND)
    FetchContent_Declare(
        tmx
        GIT_REPOSITORY "https://github.com/baylej/tmx"
        GIT_TAG 268f1cdb8c4811c6aac5addf7c4e7063342d3af1
    )
    FetchContent_MakeAvailable(tmx)
endif()

find_package(TCLAP QUIET)
if (NOT TCLAP_FOUND)
    FetchContent_Declare(
        TCLAP
        GIT_REPOSITORY "git://git.code.sf.net/p/tclap/code"
        GIT_TAG 3feeb7b2499b37d9cb80890cadaf7c905a9a50c6
    )
    FetchContent_MakeAvailable(TCLAP)
endif()

file(GLOB SRC_FILES src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE raylib raylib_cpp tmx EnTT::EnTT TCLAP)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
