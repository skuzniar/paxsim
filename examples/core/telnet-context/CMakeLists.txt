set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-context telnet-context.cpp)

target_include_directories(telnet-context PRIVATE ../../..)

target_link_libraries(telnet-context PRIVATE Boost::boost Threads::Threads)

