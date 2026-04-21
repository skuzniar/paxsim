set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

find_package(Lua REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(ALL_APPS
    Server
    Client
)

message("Apps to be built: ${ALL_APPS}")

foreach(app ${ALL_APPS})
    add_executable(${app})

    target_sources(${app} PRIVATE ${app}.cpp)

    target_include_directories(${app} PRIVATE 
        .
        Protocol
        ${LUA_INCLUDE_DIR}
        ${SOL2_INCLUDE_DIR}
    )

    target_link_libraries(${app} PRIVATE
        PaxSim::Core
        ${LUA_LIBRARIES} 
        Boost::boost 
        Threads::Threads
    )
endforeach()

add_subdirectory(FIX)
add_subdirectory(OUCH)

