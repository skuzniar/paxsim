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

target_include_directories(PAXSim PRIVATE 
)

target_link_libraries(PAXSim PRIVATE 
)

target_include_directories(PAXSimClient PRIVATE 
)

target_link_libraries(PAXSimClient PRIVATE 
)

