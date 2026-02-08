set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-7 Echo-7.cpp)

target_include_directories(Echo-7 PRIVATE ../../..
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(Echo-7 PRIVATE ${LUA_LIBRARIES} Boost::boost Threads::Threads)

