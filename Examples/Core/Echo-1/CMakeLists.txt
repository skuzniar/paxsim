set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-1 Echo-1.cpp)

target_include_directories(Echo-1 PRIVATE ../../..)

target_link_libraries(Echo-1 PRIVATE Boost::boost Threads::Threads)

