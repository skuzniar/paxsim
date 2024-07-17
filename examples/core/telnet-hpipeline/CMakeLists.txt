set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-hpipeline telnet-hpipeline.cpp)

target_include_directories(telnet-hpipeline PRIVATE ../../..)

target_link_libraries(telnet-hpipeline PRIVATE Boost::boost Threads::Threads)

