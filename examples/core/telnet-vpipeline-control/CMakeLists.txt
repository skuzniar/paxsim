set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-vpipeline-control telnet-vpipeline-control.cpp)

target_include_directories(telnet-vpipeline-control PRIVATE ../../..)

target_link_libraries(telnet-vpipeline-control PRIVATE Boost::boost Threads::Threads)

