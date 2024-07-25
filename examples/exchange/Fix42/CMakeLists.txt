set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(QuickFix REQUIRED)
find_package(JsonCpp REQUIRED)
find_package(Json5Cpp REQUIRED)

add_executable(Fix42 Fix42.cpp)

target_include_directories(Fix42 PRIVATE 
    ${QUICKFIX_INCLUDE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${JSON5CPP_INCLUDE_DIR}
    ../../..
    ../../../examples
    )

target_link_libraries(Fix42 PRIVATE 
    ${QUICKFIX_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    Boost::boost
    Threads::Threads
    )

