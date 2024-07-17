set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

add_executable(telnet-basic-control telnet-basic-control.cpp)

target_include_directories(telnet-basic-control PRIVATE ../../..)

target_link_libraries(telnet-basic-control PRIVATE Boost::boost)

