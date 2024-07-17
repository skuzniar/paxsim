set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-hpipeline-control telnet-hpipeline-control.cpp)

target_include_directories(telnet-hpipeline-control PRIVATE ../../..)

target_link_libraries(telnet-hpipeline-control PRIVATE Boost::boost Threads::Threads)

