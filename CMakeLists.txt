cmake_minimum_required(VERSION 3.30...4.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

project(
    PaxSim
    DESCRIPTION "Protocol Agnostic Exchange Siamulator Library"
    LANGUAGES CXX
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(third_party)

add_subdirectory(Libs)
add_subdirectory(Apps)
add_subdirectory(Examples)

