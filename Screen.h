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

#include "bcm_host.h"
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"


class Screen 
{
    public:
        // create , destroy
        Screen();
        ~Screen();
        
        // init sreen with egl
        bool eglInitScreen();
        
        // bind context to current thread
        bool eglBindToCurrentThread();
        
        // egl swap buffers -> display new buffer
        void eglSwapBuffer();
        

        static uint32_t display_width;
        static uint32_t display_height;
        
        
        
    private:
        // -- Var
        //static EGL_DISPMANX_WINDOW_T nativewindow;

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
        
        
};

/*
// static 
uint32_t Screen::display_width;
uint32_t Screen::display_height;
 */

#endif	/* SCREEN_H */

