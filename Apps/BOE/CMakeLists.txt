target_sources(PAXSim 
    PUBLIC
    BOE2Server.h
    PRIVATE
    BOE2Server.cpp
)

target_sources(PAXSimClient 
    PUBLIC
    BOE2Client.h
    PRIVATE
    BOE2Client.cpp
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

