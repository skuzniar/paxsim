cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

project(paxsim)

add_subdirectory(core)
add_subdirectory(control)
add_subdirectory(Simulator)
