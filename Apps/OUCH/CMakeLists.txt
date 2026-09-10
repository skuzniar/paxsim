target_sources(PAXSim 
    PUBLIC
    OUCH50Server.h
    PRIVATE
    OUCH50Server.cpp
)

target_sources(PAXSimClient 
    PUBLIC
    OUCH50Client.h
    PRIVATE
    OUCH50Client.cpp
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

