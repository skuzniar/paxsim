set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_executable(Echo-5 Echo-5.cpp)

target_include_directories(Echo-5 PRIVATE ../../..)

target_link_libraries(Echo-5 PRIVATE Boost::boost Threads::Threads)

