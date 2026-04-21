include(FindPackageHandleStandardArgs)

find_path(HFFIX_INCLUDE_DIR hffix.hpp
    PATHS $ENV{HOME}/local/opt/hffix
    PATH_SUFFIXES include
    REQUIRED)

find_package_handle_standard_args(HfFix
    REQUIRED_VARS HFFIX_INCLUDE_DIR)

