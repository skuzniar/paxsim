set(SOURCES
    Error.cpp
    Value.cpp
    Path.cpp
    Reader.cpp
    Writer.cpp
)

# Define the library
add_library(az-json STATIC ${SOURCES})

# Make headers available to targets linking this library
target_include_directories(az-json PUBLIC .)   

