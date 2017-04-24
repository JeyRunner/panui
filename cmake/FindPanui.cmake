# Try to find panui library
# This will define
#  PANUI_FOUND - if panui is found
#  PANUI_INCLUDE_DIRS - The panui include directories
#  PANUI_LIBRARIES - The libraries needed to use panui
#  PANUI_DEFINITIONS - Compiler switches required for using panui
message("<FindPanui.cmake>")

SET(PANUI_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /opt/local
        /opt
        )
# Uncomment the following line to print which directory CMake is looking in.
#MESSAGE(STATUS "PART4_ROOT_DIR: " ${PART4_ROOT_DIR})

find_path(
        PANUI_INCLUDE_DIR
        NAMES panui/View.h
        PATH_SUFFIXES include/SDL2 include
        PATHS ${PANUI_SEARCH_PATHS}
        DOC "The panui include directory"
        )

find_library(PANUI_LIBRARY
        NAMES panui
        PATH_SUFFIXES lib64 lib lib/x64 lib/x86
        PATHS ${PANUI_SEARCH_PATHS}
        DOC "The panui library"
        )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(PANUI DEFAULT_MSG PANUI_INCLUDE_DIR PANUI_LIBRARY)

if (PANUI_FOUND)
    set(PANUI_LIBRARIES ${PANUI_LIBRARY} )
    set(PANUI_INCLUDE_DIRS ${PANUI_INCLUDE_DIR} )
    set(PANUI_DEFINITIONS )
endif()


message("</FindSDL2.cmake>")