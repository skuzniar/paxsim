set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-xpipeline telnet-xpipeline.cpp)

target_include_directories(telnet-xpipeline PRIVATE ../../..)

target_link_libraries(telnet-xpipeline PRIVATE Boost::boost Threads::Threads)

