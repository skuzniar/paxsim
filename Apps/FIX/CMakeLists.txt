#--- TODO
target_sources(PAXSim 
    PRIVATE
    FIX42Server.cpp
    PUBLIC
    FIX42Server.h
)

target_sources(Client 
    PRIVATE
    FIX42Client.cpp
    PUBLIC
    FIX42Client.h
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(HfFix REQUIRED)
find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)

target_include_directories(Client PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_include_directories(PAXSim PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(Client PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${LUA_LIBRARIES} 
)

target_link_libraries(PAXSim PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${LUA_LIBRARIES} 
)

