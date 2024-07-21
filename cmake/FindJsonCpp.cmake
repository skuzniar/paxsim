find_path(JSONCPP_INCLUDE_DIR json/json.h
    REQUIRED
    )

find_library(JSONCPP_LIBRARIES NAMES jsoncpp
    REQUIRED
    )

find_package_handle_standard_args(JsonCpp
    REQUIRED_VARS JSONCPP_INCLUDE_DIR JSONCPP_LIBRARIES
    )

