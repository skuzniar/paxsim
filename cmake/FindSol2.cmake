include(FindPackageHandleStandardArgs)

find_path(SOL2_INCLUDE_DIR sol/sol.hpp
    PATHS $ENV{HOME}/local/opt/sol2
    PATH_SUFFIXES include
    REQUIRED)

find_package_handle_standard_args(Sol2
    REQUIRED_VARS SOL2_INCLUDE_DIR)

