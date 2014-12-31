/* 
 * File:   EglScreen.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 * ---------------------------------------
 * SCREEN CLASS --------------------------
 * initialisition of screen
 * -> with egl
 * -> create display, surface object
 * 
 * swap buffer fuction
 * -> prints ogl renderd on screen
 * ---------------------------------------
 */

#ifndef SCREEN_H
#define	SCREEN_H
#ifdef pl_pi
#include "bcm_host.h"
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#else
#include <stdint.h>
#include "GL.h"
#endif


class Screen 
{
    public:
        // create , destroy
        Screen();
        ~Screen();
        
        // ini screen
        bool initScreen();
        
        // init sreen with egl
        bool eglInitScreen();
        
        // init screen with sdl
        bool sdlInitScreen();
        
        // bind context to current thread
        // bool eglBindToCurrentThread();
        
        // egl swap buffers -> display new buffer
        void swapBuffer();
        
//#ifdef pl_pi
        static uint32_t display_width;
        static uint32_t display_height;
//#endif
        
        
    private:
        // -- Var
        //static EGL_DISPMANX_WINDOW_T nativewindow;
#ifdef pl_pi
        DISPMANX_ELEMENT_HANDLE_T dispman_element;
        DISPMANX_DISPLAY_HANDLE_T dispman_display;
        DISPMANX_UPDATE_HANDLE_T dispman_update;

        VC_RECT_T dst_rect;
        VC_RECT_T src_rect;

        


        EGLConfig  config;
        EGLint     num_config;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
#endif
        
};

/*
// static 
uint32_t Screen::display_width;
uint32_t Screen::display_height;
 */

#endif	/* SCREEN_H */

