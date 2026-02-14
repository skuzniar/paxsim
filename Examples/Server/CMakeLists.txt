cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)
find_package(QuickFix REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Simulator 
    Simulator.cpp 
)

add_subdirectory(Fix)

target_include_directories(Simulator PRIVATE 
    . 
    ../..
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
)

target_link_libraries(Simulator PRIVATE 
    ${LUA_LIBRARIES} 
    ${QUICKFIX_LIBRARIES}
    Boost::boost 
    Threads::Threads
)

