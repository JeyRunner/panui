# Target
set(TARGET ${PROJECT_NAME}Example)

# files
set(SRC_FILES
        source/main.cpp
        source/SDL_android_main.c)


#
# extrutable / Lib
#
if (ANDROID)
    add_library(
            ${TARGET}
            SHARED
            ${SRC_FILES})
else ()
    add_executable(
            ${TARGET}
            ${SRC_FILES})
endif ()


# c++11
setCpp11()

# glm
# @TODO use panui via findPanui.cmake
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

#
# Project options
#
set_target_properties(${TARGET}
        PROPERTIES
        ${DEFAULT_PROJECT_OPTIONS}
        FOLDER "appfolder/"
        )


#
# Include directories
#
target_include_directories(${TARGET}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include

        PUBLIC
        ${DEFAULT_INCLUDE_DIRECTORIES}
        )


#
# Libraries
#
target_link_libraries(
        ${TARGET}
        panui)

message(STATUS "Example LinkLibs: ${DEFAULT_LIBRARIES}")


# ########################################
#
# Deployment
#

# Self
install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
        LIBRARY DESTINATION ${INSTALL_LIB} COMPONENT runtime
        ARCHIVE DESTINATION ${INSTALL_ARCH} COMPONENT develope
        )

# Header files
install(DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${INSTALL_INCLUDE}
        COMPONENT develope
        )


# android build apk
if (ANDROID)
    android_create_apk(
            ${TARGET}                               # name
            "com.panui.panuiExample"                # package name
            ${OUTPUT}/${WRAPPER_ANDROID}            # build dir for all android files
            ${OUTPUT}/lib                           # libs to use from java
            ${WRAPPER_ANDROID_DIR}                  # dir of AndroidManifest..
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"    # asserts dir
    )
endif ()
