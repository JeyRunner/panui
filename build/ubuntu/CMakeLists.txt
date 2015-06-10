
# ####################
# ## BUILD UBUNTU  ##
# ###################


# source files
set (srcDir ../../)
file( GLOB srcs ${srcDir}*.cpp ${srcDir}*.h )

# output
set (CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set (target ${PROJECT})
set (UBUNTU_SO_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/ubuntu)


# include ######################################################
include_directories ( /opt/vc/include )
include_directories ( /opt/vc/include/interface/vcos/pthreads )
include_directories ( /opt/vc/include/interface/vmcs_host/linux )
include_directories ( /usr/local/include/freetype2 )
include_directories ( /usr/include/SDL )

# link libs
link_directories    ( /opt/vc/lib/ /usr/local/lib )


# define #######################################################
add_definitions (-Dpl_ubuntu)
add_definitions (-DGLM_FORCE_RADIANS)


# c++ 11 ########################################################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	message(STATUS "compiler supports c++11 flag")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

elseif(COMPILER_SUPPORTS_CXX0X)
	message(STATUS "compiler supports c++0x flag")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#################################################################
# output path
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# create lib
add_library (${target} SHARED ${srcs})

# set the output directory for the .so files to point to the android project's 'lib/[cpuarch] directory
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${UBUNTU_SO_OUTDIR})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${UBUNTU_SO_OUTDIR})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${UBUNTU_SO_OUTDIR})

# link libs
target_link_libraries 	( ${target} pthread rt freetype GLEW GL SDL )
