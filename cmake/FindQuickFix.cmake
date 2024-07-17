find_path(QUICKFIX_INCLUDE_DIR quickfix/Session.h
    PATHS $ENV{HOME}/local
    PATH_SUFFIXES include
    REQUIRED
    )

find_library(QUICKFIX_LIBRARIES NAMES quickfix
    PATHS $ENV{HOME}/local
    PATH_SUFFIXES lib
    REQUIRED
    )

find_package_handle_standard_args(QuickFix
    REQUIRED_VARS QUICKFIX_INCLUDE_DIR QUICKFIX_LIBRARIES
    )

