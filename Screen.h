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

#include <stdio.h>
#include <string>
#include <functional>
using namespace std;

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
        
        // resize Screen
        void resizeScreen(int width, int height);
        
        // set title
        void setTitle(string title);
        
        // close Screen
        void closeScreen();
        
        
        // init sreen with egl
        bool eglInitScreen();
        
        // init screen with sdl
        bool sdlInitScreen(int width, int height, string title);
        
        // bind context to current thread
        // bool eglBindToCurrentThread();
        
        // egl swap buffers -> display new buffer
        void swapBuffer();
        
        // check for events
        void checkEvents();
        
        // set on resizeScreen event listener
        void onTouchMove(function<void(int x, int y)> onTouchMove);
        
        // set on resizeScreen event listener
        void onResizeScreen(function<void(int width, int height)> onResizeScreen);
        
        // set on closeWindow event listener
        void onCloseScreen(function<void()> onCloseWindow);
        
        static uint32_t display_width;
        static uint32_t display_height;
        string display_title;
        
        
    private:
        // -- Var
        
        // eventListener functions
        function<void (int x, int y)> onTouchMoveFunc;
        function<void (int width, int height)> onResizeScreenFunc;
        function<void ()> onCloseScreenFunc;
        
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

