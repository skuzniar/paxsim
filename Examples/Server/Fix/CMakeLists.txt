target_sources(Simulator 
    PRIVATE
        Fix42.cpp
    PUBLIC
        Fix42.h
)

set(CMAKE_CXX_FLAGS "-Wno-unknown-attributes")

target_include_directories(Simulator PRIVATE 
    . 
    ../../..
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
)

