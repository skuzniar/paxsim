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
         "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    target_link_libraries(${app} PRIVATE
        PaxSim
        Config
        Params
        Boost::boost 
        Threads::Threads
        az-json
    )
endforeach()

add_subdirectory(FIX)
add_subdirectory(OUCH)
add_subdirectory(BOE)
