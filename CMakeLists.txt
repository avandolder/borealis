cmake_minimum_required(VERSION 3.20)
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

#[[
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v4.2.2
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()
]]

add_executable(${PROJECT_NAME}
    src/main.cpp
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

target_link_libraries(${PROJECT_NAME} raylib #[[raylib_cpp]])
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
