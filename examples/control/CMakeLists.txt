set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(JsonCpp REQUIRED)
find_package(Json5Cpp REQUIRED)

add_executable(test test.cpp)

target_include_directories(test PRIVATE 
    ${JSONCPP_INCLUDE_DIR}
    ${JSON5CPP_INCLUDE_DIR}
    ../..
    )

target_link_libraries(test PRIVATE 
    ${JSONCPP_LIBRARIES}
    )

