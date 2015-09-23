# ####################
# ## BUILD ANDROID ##
# ###################


# source files
set (srcDir ../../)
file( GLOB srcs ${srcDir}*.cpp ${srcDir}*.h )

# output
set (CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set (target ${PROJECT}Android)


# ### panui ###################################################################################################
add_definitions (-Dpl_andr)
add_definitions (-DGLM_FORCE_RADIANS)
include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/include )



# ### android #################################################################################################
# var
set (ANDROID_TOOLCHAIN 		/home/joshua/Dokumente/android/android-toolchain )
set (ANDROID_TOOLCHAIN_INCLUDE	${ANDROID_TOOLCHAIN}/include )
set (ANDROID_SYSROOT		${ANDROID_TOOLCHAIN}/sysroot/usr )
set (ANDROID_SYSROOT_LIB  	${ANDROID_SYSROOT}/lib )
set (ANDROID_SYSROOT_INCLUDE  	${ANDROID_SYSROOT}/include )
set (LIB_FREETYPE_STATIC	/home/joshua/Dokumente/android/android_libs/freetype2/objs/.libs/)

set (ANDROID_NDK_CPU		armeabi-v7a)
set (LINK_LIBS			SDL2  GLESv2 log android c stdc++ gnustl_shared libfreetype.a) #GLESv1_CM
set (GCC_NAME 			arm-linux-androideabi-gcc)
set (ANDROID_SDL_PATH		${CMAKE_CURRENT_SOURCE_DIR}/SDL2/src/main/android)

# SDL files
file(GLOB srcSDL ${srcDir}/SDL/*.cpp )


# compiler
set (CMAKE_CXX_COMPILER 	${ANDROID_TOOLCHAIN}/bin/${GCC_NAME})

# lib
link_directories(${ANDROID_SYSROOT_LIB})
link_directories(${ANDROID_TOOLCHAIN}/arm-linux-androideabi/lib/armv7-a)
link_directories(${LIB_FREETYPE_STATIC})
#find_library (LIB_FRT ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfreetype2-static.a)
#set(CMAKE_EXE_LINKER_FLAGS "-libfreetype2-static")

# create lib
add_library (${target} SHARED ${srcs} ${srcSDL})


# set the output directory for the .so files to point to the android project's 'lib/[cpuarch] directory
set(ANDROID_SO_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/${ANDROID_NDK_CPU})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${ANDROID_SO_OUTDIR})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ANDROID_SO_OUTDIR})
set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ANDROID_SO_OUTDIR})


# include
include_directories(${ANDROID_TOOLCHAIN_INCLUDE})
include_directories(${ANDROID_SYSROOT_INCLUDE})
include_directories(${ANDROID_SYSROOT_INCLUDE}/SDL)
include_directories(${ANDROID_SYSROOT_INCLUDE}/android)
include_directories(${ANDROID_SYSROOT_INCLUDE}/freetype2)
include_directories(${ANDROID_SDL_PATH})



# copy shared libraries over from the Android toolchain directory
file(COPY ${ANDROID_TOOLCHAIN}/arm-linux-androideabi/lib/armv7-a/libgnustl_shared.so DESTINATION ${ANDROID_SO_OUTDIR})
file(COPY ${LIB_FREETYPE_STATIC}/libfreetype.a DESTINATION ${ANDROID_SO_OUTDIR})




# link libs
target_link_libraries(${target} ${LINK_LIBS} )
################################################################################################################








# c++ 11
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

