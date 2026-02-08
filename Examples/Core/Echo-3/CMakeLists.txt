set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-3 Echo-3.cpp)

target_include_directories(Echo-3 PRIVATE ../../..)

target_link_libraries(Echo-3 PRIVATE Boost::boost Threads::Threads)

