set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-6 Echo-6.cpp)

target_include_directories(Echo-6 PRIVATE ../../..)

target_link_libraries(Echo-6 PRIVATE Boost::boost Threads::Threads)

