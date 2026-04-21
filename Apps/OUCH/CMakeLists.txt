target_sources(Server 
    PRIVATE
    OUCH50Server.cpp
    PUBLIC
    OUCH50Server.h
)

target_sources(Client 
    PRIVATE
    OUCH50Client.cpp
    PUBLIC
    OUCH50Client.h
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)

target_include_directories(Client PRIVATE 
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_include_directories(Server PRIVATE 
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(Client PRIVATE 
    ${LUA_LIBRARIES} 
)

target_link_libraries(Server PRIVATE 
    ${LUA_LIBRARIES} 
)

