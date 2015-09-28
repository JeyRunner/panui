# ######################
# ## BUILD RASPBERRY  ##
# ######################

# var
set (target     ${PROJECT})
set (LINK_LIBS	GLESv2 EGL openmaxil bcm_host vcos vchiq_arm pthread rt freetype)


# include ######################################################
include_directories ( /opt/vc/include )
include_directories ( /opt/vc/include/interface/vcos/pthreads )
include_directories ( /opt/vc/include/interface/vmcs_host/linux )
include_directories ( /usr/local/include/freetype2 )
#include_directories ( /usr/include/SDL )

# link libs
link_directories    ( /opt/vc/lib/ /usr/local/lib )

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
