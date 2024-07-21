find_path(JSON5CPP_INCLUDE_DIR json5cpp.h
    PATHS $ENV{HOME}/local/include
    PATH_SUFFIXES json5cpp
    REQUIRED)

find_package_handle_standard_args(Json5Cpp
    REQUIRED_VARS JSON5CPP_INCLUDE_DIR
    )

