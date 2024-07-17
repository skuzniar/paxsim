set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-basic telnet-basic.cpp)

target_include_directories(telnet-basic PRIVATE ../../..)

target_link_libraries(telnet-basic PRIVATE Boost::boost Threads::Threads)

