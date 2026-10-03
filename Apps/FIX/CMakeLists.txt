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

find_package(HfFix REQUIRED)

target_include_directories(PAXSim PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
)

target_link_libraries(PAXSim PRIVATE 
    ${QUICKFIX_LIBRARIES}
)

target_include_directories(PAXSimClient PRIVATE 
    ${HFFIX_INCLUDE_DIR}
    ${QUICKFIX_INCLUDE_DIR}
)

target_link_libraries(PAXSimClient PRIVATE 
    ${QUICKFIX_LIBRARIES}
)

