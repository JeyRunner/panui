# Target
set(TARGET ${PROJECT_NAME})


# files
file(GLOB   SRC_FILES
            ui/source/*.cpp
            ui/source/*.c
            ui/include/*.h)
file(GLOB   HEADER_FILES
            ui/include/*.h)


#
# Library
#
add_library(
    ${TARGET}
    SHARED
    ${SRC_FILES})

# c++11
setCpp11()

# glm
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
        ${PROJECT_BINARY_DIR}/source/include
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/include

        PUBLIC
        ${DEFAULT_INCLUDE_DIRECTORIES}
        )


#
# Used libraries
#
target_link_libraries(${TARGET}
        ${DEFAULT_LIBRARIES}
        )
message(STATUS LinkLibs: "${DEFAULT_LIBRARIES}")
message(STATUS LinkLibDir: "${DEPENDENCIES_FREETYPE_DIR}")


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

# version header
configure_file(meta/version.h.in ${OUTPUT_INCLUDE}/version.h)

# Header files
install(DIRECTORY
        ${OUTPUT_INCLUDE} DESTINATION ${INSTALL_INCLUDE}
        COMPONENT develope
        )

# copy header to output
file(COPY ${HEADER_FILES} DESTINATION ${OUTPUT_INCLUDE})
