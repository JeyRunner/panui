# compile dependencies


# -- SDL2 -------------------------------------------------
find_package(SDL2 QUIET)

if (SDL2_FOUND)
    message(STATUS "[DEPENDENCY] SDL2 => Found includeDir: ${SDL2_INCLUDE_DIRS}, libDir: ${SDL2_LIBDIR}")

    set(DEPENDENCIES_SDL2_INCLUDE ${SDL2_INCLUDE_DIRS} PARENT_SCOPE)
    set(DEPENDENCIES_SDL2_DIR ${SDL2_LIBDIR} PARENT_SCOPE)
    set(DEPENDENCIES_SDL2_LIB SDL2 PARENT_SCOPE)
else ()
    message(STATUS "[DEPENDENCY] SDL2 => NOT found => will compile SDL")
    execute_process(COMMAND git submodule update --init --recursive dependencies/SDL2)

    if (ANDROID)
        set(SDL_STATIC ON CACHE BOOL "Build the static SDL library")
        set(SDL_SHARED OFF CACHE BOOL "Build the shared SDL library")
        set(DEPENDENCIES_SDL2_LIB SDL2 PARENT_SCOPE)
    else ()
        set(SDL_STATIC OFF CACHE BOOL "Build the static SDL library")
        set(SDL_SHARED ON CACHE BOOL "Build the shared SDL library")
        set(DEPENDENCIES_SDL2_LIB SDL2 PARENT_SCOPE)
    endif ()
    set(PTHREADS ON CACHE BOOL "Pthread support")
    #set(DISKAUDIO  OFF)
    add_subdirectory(SDL2)

    # copy libSDL2.so into sdl build folder => needed for creating package
    add_custom_command(
            OUTPUT output
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_LIB}/libSDL2*.so.*.*      ./SDL2/
            COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_LIB}/libSDL2*.so          ./SDL2/
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libSDL2.so     ./SDL2/
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/libSDL2.so     ${OUTPUT_LIB}
            SDL2)
        message("out: ${output}")

    # copy libSDL2.so into sdl build folder => needed for creating package
    #file(GLOB SDL_LIB_FILES ${OUTPUT_LIB}/libSDL2*.so.*.*)
    #file(GLOB SDL_LIB_FILES_LINK ${OUTPUT_LIB}/libSDL2*.so)
    #file(GLOB SDL_LIB_FILES_LINK_SDL ${CMAKE_BINARY_DIR}/libSDL2.so)
    #file(COPY ${SDL_LIB_FILES} DESTINATION SDL2)
    #file(COPY ${SDL_LIB_FILES_LINK} DESTINATION SDL2)
    #file(COPY ${SDL_LIB_FILES_LINK_SDL} DESTINATION SDL2)
    #file(COPY ${SDL_LIB_FILES_LINK_SDL} DESTINATION ${OUTPUT_LIB})

    set(DEPENDENCIES_SDL2_INCLUDE ${CMAKE_SOURCE_DIR}/dependencies/SDL2/include PARENT_SCOPE)
    set(DEPENDENCIES_SDL2_DIR ${CMAKE_SOURCE_DIR}/dependencies/SDL2 PARENT_SCOPE)
endif ()


# -- FREETYPE --------------------------------------------
find_package(Freetype QUIET)

if (FREETYPE_FOUND)
    message(STATUS "[DEPENDENCY] freetype2 => Found includeDir: ${SDL2_INCLUDE_DIRS}, libDir: ${SDL2_LIBDIR}")

    set(DEPENDENCIES_FREETYPE_INCLUDE ${FREETYPE_INCLUDE_DIRS} PARENT_SCOPE)
    set(DEPENDENCIES_FREETYPE_DIR ${FREETYPE_LIBRARIES} PARENT_SCOPE)
    set(DEPENDENCIES_FREETYPE_LIB ${FREETYPE_LIBRARIES} PARENT_SCOPE)
else ()
    message(STATUS "[DEPENDENCY] freetype2 => NOT found => will compile freetype2")
    execute_process(COMMAND git submodule update --init --recursive dependencies/freetype)

    set(BUILD_SHARED_LIBS true)
    add_subdirectory(freetype)

    set(DEPENDENCIES_FREETYPE_INCLUDE ${CMAKE_SOURCE_DIR}/dependencies/freetype/include PARENT_SCOPE)
    set(DEPENDENCIES_FREETYPE_DIR ${OUTPUT_LIB} PARENT_SCOPE)
    set(DEPENDENCIES_FREETYPE_LIB freetype PARENT_SCOPE)
endif ()


# -- GLEW -------------------------------------------------
if (NOT ANDROID)
    find_package(GLEW QUIET)

    if (GLEW_FOUND)
        message(STATUS "[DEPENDENCY] GLEW => Found includeDir: ${GLEW_INCLUDE_DIRS}, lib:  ${GLEW_LIBRARIES}")

        set(DEPENDENCIES_GLEW_INCLUDE ${GLEW_INCLUDE_DIRS} PARENT_SCOPE)
        set(DEPENDENCIES_GLEW_DIR ${GLEW_LIBDIR} PARENT_SCOPE)
        set(DEPENDENCIES_GLEW_LIB GLEW PARENT_SCOPE)
    else ()
        message(STATUS "[DEPENDENCY] GLEW => NOT found => will compile GLEW => NOT IMPLEMENTED!")
        execute_process(COMMAND git submodule update --init --recursive dependencies/GLEW)

        #add_subdirectory(GLEW)

        #set(DEPENDENCIES_GLEW_INCLUDE   ${CMAKE_SOURCE_DIR}/dependencies/GLEW/include   PARENT_SCOPE)
        #set(DEPENDENCIES_GLEW_DIR       ${CMAKE_SOURCE_DIR}/dependencies/GLEW           PARENT_SCOPE)
        #set(DEPENDENCIES_GLEW_LIB       glew                                            PARENT_SCOPE)
    endif ()
endif ()


# -- GL -------------------------------------------------
if (NOT ANDROID)
    find_package(OpenGL REQUIRED)

    if (OpenGL_FOUND)
        message(STATUS "[DEPENDENCY] OpenGL => Found includeDir: ${OPENGL_INCLUDE_DIR}, lib:  ${OPENGL_gl_LIBRARY}")

        set(DEPENDENCIES_OpenGL_INCLUDE ${OPENGL_INCLUDE_DIR} PARENT_SCOPE)
        set(DEPENDENCIES_OpenGL_DIR ${OPENGL_gl_LIBRARY} PARENT_SCOPE)
        set(DEPENDENCIES_OpenGL_LIB GL PARENT_SCOPE)
    else()
        message(STATUS "[DEPENDENCY] OpenGL => NOT Found => FATAL")
    endif ()
else ()
    message(STATUS "[DEPENDENCY] OpenGL => set for android to GLESv2")
    set(DEPENDENCIES_OpenGL_INCLUDE PARENT_SCOPE)
    set(DEPENDENCIES_OpenGL_DIR $ PARENT_SCOPE)
    set(DEPENDENCIES_OpenGL_LIB GLESv2 PARENT_SCOPE)
endif ()


# -- GLM -------------------------------------------------
set(DEPENDENCIES_GLM_INCLUDE ${CMAKE_SOURCE_DIR}/dependencies/glm PARENT_SCOPE)


# -- TERMCOLOR -------------------------------------------
set(DEPENDENCIES_TERMCOLOR_INCLUDE ${CMAKE_SOURCE_DIR}/dependencies/termcolor PARENT_SCOPE)


# -- ALL -------------------------------------------------
#set(DEPENDENCIES_INCLUDE ${CMAKE_SOURCE_DIR}/dependencies PARENT_SCOPE)