set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(QuickFix REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(Json5Cpp REQUIRED)

add_executable(Simulator) 

target_sources(Simulator PRIVATE 
    Simulator.cpp
    )

target_include_directories(Simulator PRIVATE 
    ${QUICKFIX_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${JSON5CPP_INCLUDE_DIR}
    ../..
    Fix42
    )

target_link_libraries(Simulator PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    Boost::boost
    Threads::Threads
    )

add_subdirectory(Fix42) 
