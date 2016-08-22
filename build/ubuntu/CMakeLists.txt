# ####################
# ## BUILD UBUNTU  ##
# ###################

# var
set (target     ${PROJECT})
set (LINK_LIBS  pthread rt freetype GLEW GL SDL)


# include
include_directories ( /opt/vc/include )
include_directories ( /opt/vc/include/interface/vcos/pthreads )
include_directories ( /opt/vc/include/interface/vmcs_host/linux )
include_directories ( /usr/include/freetype2 )
include_directories ( /usr/include/SDL )

# link libs
link_directories    ( /opt/vc/lib/ /usr/local/lib )

# ubuntu platform
add_definitions (-Dpl_ubuntu)

# c++11
checkCompilerCXX()

# create lib
add_library (${target} SHARED ${SRC_FILES})

# outdir
setSoOutDir(${UBUNTU_SO_OUTDIR})

# link libs
target_link_libraries 	( ${target} ${LINK_LIBS})
