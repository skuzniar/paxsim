set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(telnet-control telnet-control.cpp)

target_include_directories(telnet-control PRIVATE ../../..)

target_link_libraries(telnet-control PRIVATE Boost::boost)

