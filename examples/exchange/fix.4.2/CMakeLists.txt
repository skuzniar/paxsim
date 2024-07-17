set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED)
find_package(QuickFix REQUIRED)

add_executable(fix.4.2 fix.4.2.cpp)

target_include_directories(fix.4.2 PRIVATE 
    ${QUICKFIX_INCLUDE_DIR}
    ../../..
    ../../../examples
    )

target_link_libraries(fix.4.2 PRIVATE 
    ${QUICKFIX_LIBRARIES}
    Boost::boost
    )

