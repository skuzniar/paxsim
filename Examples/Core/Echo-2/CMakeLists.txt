set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-2 Echo-2.cpp)

target_include_directories(Echo-2 PRIVATE ../../..)

target_link_libraries(Echo-2 PRIVATE Boost::boost Threads::Threads)

