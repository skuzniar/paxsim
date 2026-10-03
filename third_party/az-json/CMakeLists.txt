add_library(az-json STATIC 
    src/Error.cpp
    src/Value.cpp
    src/Path.cpp
    src/Reader.cpp
    src/Writer.cpp
)

target_include_directories(az-json 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)   

