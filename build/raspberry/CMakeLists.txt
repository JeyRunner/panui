# ######################
# ## BUILD RASPBERRY  ##
# ######################

# var
set (target     ${PROJECT}Raspberry)
set (LINK_LIBS	GLESv2 EGL openmaxil bcm_host vcos vchiq_arm pthread rt freetype)


### cross compile
if(${RASPBERRY_CROSS_COMPILE})
# host system
SET(CMAKE_SYSTEM_NAME    Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER     ${RASPBERRY_TOOLCHAIN}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc )
SET(CMAKE_CXX_COMPILER   ${RASPBERRY_TOOLCHAIN}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/${RASPBERRY_GCC_NAME})

# target environment
SET(CMAKE_SYSROOT        ${RASPBERRY_TOOLCHAIN}/root)
SET(CMAKE_FIND_ROOT_PATH ${RASPBERRY_TOOLCHAIN}/root)
SET(RASPBERRY_ROOT       ${RASPBERRY_TOOLCHAIN}/root)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()


# include ######################################################
include_directories ( ${RASPBERRY_ROOT}/opt/vc/include )
include_directories ( ${RASPBERRY_ROOT}/opt/vc/include/interface/vcos/pthreads )
include_directories ( ${RASPBERRY_ROOT}/opt/vc/include/interface/vmcs_host/linux )
include_directories ( ${RASPBERRY_ROOT}/usr/local/include/freetype2 )
#include_directories ( /usr/include/SDL )

# link libs
link_directories    (  ${RASPBERRY_ROOT}/opt/vc/lib/
                       ${RASPBERRY_ROOT}/usr/local/lib )

# raspberry platform
add_definitions (-Dpl_pi)

# c++11
checkCompilerCXX()

# create lib
add_library (${target} SHARED ${SRC_FILES})

# outdir
setSoOutDir(${RASPERRY_SO_OUTDIR})

# link libs
target_link_libraries 	( ${target} ${LINK_LIBS} )
