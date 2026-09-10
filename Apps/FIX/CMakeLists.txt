target_sources(PAXSim 
    PUBLIC
    FIX42Server.h
    PRIVATE
    FIX42Server.cpp
)

target_sources(PAXSimClient 
    PUBLIC
    FIX42Client.h
    PRIVATE
    FIX42Client.cpp
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(HfFix REQUIRED)
find_package(Lua REQUIRED)
find_package(Sol2 REQUIRED)

target_include_directories(PAXSim PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(PAXSim PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${LUA_LIBRARIES} 
)

target_include_directories(PAXSimClient PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
)

target_link_libraries(PAXSimClient PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${LUA_LIBRARIES} 
)

