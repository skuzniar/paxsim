set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(ALL_EXAMPLES 
    Echo-0 
    Echo-1 
    Echo-2 
    Echo-3 
    Echo-4 
    Echo-5 
    Echo-6 
)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(${example})

    target_sources(${example} PRIVATE ${example}.cpp)

    target_include_directories(${example} PRIVATE 
    )

    target_link_libraries(${example} PRIVATE
        PaxSim
        Boost::boost 
        Threads::Threads
    )
endforeach()
