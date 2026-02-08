# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindSol2
-------------

Locate Sol2 This module defines:

``SOL2_FOUND``
``SOL2_INCLUDE_DIR``
#]=======================================================================]

find_path(SOL2_INCLUDE_DIR sol/sol.hpp
    PATHS $ENV{HOME}/local/opt/sol2
    PATH_SUFFIXES include
    REQUIRED)

