set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(telnet-vpipeline telnet-vpipeline.cpp)

target_include_directories(telnet-vpipeline PRIVATE ../../..)

target_link_libraries(telnet-vpipeline PRIVATE Boost::boost Threads::Threads)

