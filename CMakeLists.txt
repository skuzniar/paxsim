cmake_minimum_required(VERSION 3.30...4.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

project(
    PaxSim
    DESCRIPTION "Protocol Agnostic Exchange Siamulator Library"
    LANGUAGES CXX
    VERSION 0.1.0
)

add_library(PaxSim INTERFACE)
add_library(PaxSim::Core ALIAS PaxSim)

add_subdirectory(include)

add_subdirectory(Examples)
add_subdirectory(Apps)

