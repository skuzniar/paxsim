include(FindPackageHandleStandardArgs)

find_path(HFFIX_INCLUDE_DIR hffix/hffix.hpp
    PATHS $ENV{HOME}/local
    PATH_SUFFIXES include
    REQUIRED)

find_package_handle_standard_args(HfFix
    REQUIRED_VARS HFFIX_INCLUDE_DIR)

