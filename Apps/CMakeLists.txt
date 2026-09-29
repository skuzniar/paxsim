set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

set(ALL_APPS
    PAXSim
    PAXSimClient
)

message("Apps to be built: ${ALL_APPS}")

foreach(app ${ALL_APPS})
    add_executable(${app})

    target_sources(${app} PRIVATE ${app}.cpp)

    target_include_directories(${app} PRIVATE 
        .
    )

    target_link_libraries(${app} PRIVATE
        PaxSim::Core
        Boost::boost 
        Threads::Threads
        az-json
    )
endforeach()

add_subdirectory(Common)

add_subdirectory(FIX)
add_subdirectory(OUCH)
add_subdirectory(BOE)
