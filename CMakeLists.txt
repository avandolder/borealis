cmake_minimum_required(VERSION 3.5)

project(borealis)

add_executable(borealis main.cpp)

install(TARGETS borealis RUNTIME DESTINATION bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS CACHE INTERNAL ON)
