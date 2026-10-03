target_sources(PAXSim 
    PUBLIC
    BOE2EQTServer.h
    BOE3EQTServer.h
    PRIVATE
    BOE2EQTServer.cpp
    BOE3EQTServer.cpp
)

target_sources(PAXSimClient 
    PUBLIC
    BOE2EQTClient.h
    BOE3EQTClient.h
    PRIVATE
    BOE2EQTClient.cpp
    BOE3EQTClient.cpp
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

target_include_directories(PAXSim PRIVATE 
)

target_link_libraries(PAXSim PRIVATE 
)

target_include_directories(PAXSimClient PRIVATE 
)

target_link_libraries(PAXSimClient PRIVATE 
)

