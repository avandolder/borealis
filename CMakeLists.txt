cmake_minimum_required(VERSION 3.23)
project(borealis CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER gcc)

find_package(raylib 4.0.0 QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib"
        GIT_TAG 4.0.0
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        # Disable raylib examples
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif ()
endif ()
 
find_package(EnTT QUIET)
if (NOT EnTT_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.10.1
    )
    FetchContent_MakeAvailable(EnTT)
endif()

find_package(tmx QUIET)
if (NOT tmx_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        tmx
        GIT_REPOSITORY https://github.com/baylej/tmx
        GIT_TAG 268f1cdb8c4811c6aac5addf7c4e7063342d3af1
    )
    FetchContent_MakeAvailable(tmx)
endif()

add_executable(${PROJECT_NAME}
    src/state.mpp
    src/state_manager.mpp
    src/tile_map.mpp
    src/tile_map_manager.mpp
    src/main.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib tmx)
target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts -Wall -Wextra -Werror -pedantic)
target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG="")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
