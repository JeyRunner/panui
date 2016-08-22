# CMake config
# panui cross-platform gui lib 

# min version
cmake_minimum_required(VERSION 2.8)

# project name
set    (  PROJECT panui)
project(${PROJECT} )


############################################
## importend variables #####################
# raspberry
set (RASPBERRY_CROSS_COMPILE        ON )
set (RASPBERRY_TOOLCHAIN            /home/joshua/Dokumente/raspberrypi)
set (RASPBERRY_GCC_NAME             arm-linux-gnueabihf-g++)


# android
set (ANDROID_TOOLCHAIN 		        /home/joshua/Dokumente/android/android-toolchain-v23 )
set (ANDROID_LIB_FREETYPE_STATIC	/home/joshua/Dokumente/android/android_libs/freetype2/objs/.libs/)
set (ANDROID_NDK_CPU		        armeabi-v7a)
set (ANDROID_GCC_NAME 			    arm-linux-androideabi-gcc)
set (ANDROID_SDL_PATH		        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/src/main/android)


# out
set (ANDROID_SO_OUTDIR              ${CMAKE_CURRENT_SOURCE_DIR}/build/android/${ANDROID_NDK_CPU})
set (RASPERRY_SO_OUTDIR             ${CMAKE_CURRENT_SOURCE_DIR}/build/raspberry/raspberry)
set (UBUNTU_SO_OUTDIR               ${CMAKE_CURRENT_SOURCE_DIR}/build/ubuntu/ubuntu)
############################################


# source files
set (       SRC_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/src/)
file( GLOB  SRC_FILES ${SRC_DIR}*.cpp ${SRC_DIR}*.h )

# glm
add_definitions (-DGLM_FORCE_RADIANS)

# include
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )


## fuctions ################################
# c++ 11
function(checkCompilerCXX)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "compiler supports c++11 flag")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" PARENT_SCOPE)

    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "compiler supports c++0x flag")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" PARENT_SCOPE)

    else()
            message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endfunction()

# set so outdir
function(setSoOutDir path)
    set(                  CMAKE_LIBRARY_OUTPUT_DIRECTORY                        ${path})
    set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY         ${path})
    set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${path})
    set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${path})
endfunction()

###########################################


# sub
#add_subDirectory (build/android)
add_subDirectory (build/ubuntu)
#add_subDirectory (build/raspberry)
