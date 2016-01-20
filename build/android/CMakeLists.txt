# ####################
# ## BUILD ANDROID ##
# ###################

# var
set (target ${PROJECT}Android)
set (ANDROID_TOOLCHAIN_INCLUDE	${ANDROID_TOOLCHAIN}/include )
set (ANDROID_SYSROOT		    ${ANDROID_TOOLCHAIN}/sysroot/usr )
set (ANDROID_SYSROOT_LIB  	    ${ANDROID_SYSROOT}/lib )
set (ANDROID_SYSROOT_INCLUDE  	${ANDROID_SYSROOT}/include )
set (ANDROID_LINK_LIBS			SDL2  GLESv2 log android c stdc++ gnustl_shared libfreetype.a) #GLESv1_CM
set (CMAKE_CXX_COMPILER 	    ${ANDROID_TOOLCHAIN}/bin/${ANDROID_GCC_NAME})

# android platform
add_definitions (-Dpl_andr)

# link libs
link_directories(   ${ANDROID_SYSROOT_LIB}
                    ${ANDROID_TOOLCHAIN}/arm-linux-androideabi/lib/armv7-a
                    ${ANDROID_LIB_FREETYPE_STATIC})

# include
include_directories(${ANDROID_TOOLCHAIN_INCLUDE})
include_directories(${ANDROID_SYSROOT_INCLUDE})
include_directories(${ANDROID_SYSROOT_INCLUDE}/SDL)
include_directories(${ANDROID_SYSROOT_INCLUDE}/android)
include_directories(${ANDROID_SYSROOT_INCLUDE}/freetype2)
include_directories(${ANDROID_SDL_PATH})

# copy shared libraries over from the Android toolchain directory
file(COPY ${ANDROID_TOOLCHAIN}/arm-linux-androideabi/lib/armv7-a/libgnustl_shared.so    DESTINATION ${ANDROID_SO_OUTDIR})
file(COPY ${ANDROID_LIB_FREETYPE_STATIC}libfreetype.a                                   DESTINATION ${ANDROID_SO_OUTDIR})


# create lib
add_library (${target} SHARED ${SRC_FILES} ${srcSDL})

# outdir
setSoOutDir(${ANDROID_SO_OUTDIR})

# c++11
checkCompilerCXX()

# link libs
target_link_libraries(${target} ${ANDROID_LINK_LIBS} )