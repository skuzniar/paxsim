target_sources(PAXSim 
    PUBLIC
    BOE2EQTServer.h
    PRIVATE
    BOE2EQTServer.cpp
)

target_sources(PAXSimClient 
    PUBLIC
    BOE2EQTClient.h
    PRIVATE
    BOE2EQTClient.cpp
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)

target_include_directories(PAXSim PRIVATE 
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(PAXSim PRIVATE 
    ${LUA_LIBRARIES} 
)

target_include_directories(PAXSimClient PRIVATE 
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(PAXSimClient PRIVATE 
    ${LUA_LIBRARIES} 
)

