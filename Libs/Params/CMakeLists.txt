add_library(Params INTERFACE)

target_include_directories(Params INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(Params 
    INTERFACE Config
    INTERFACE PaxSim
)
