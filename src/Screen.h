/* 
 * File:   EglScreen.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 * ---------------------------------------
 * SCREEN CLASS --------------------------
 * initialisation of screen
 * -> with egl
 * -> create display, surface object
 * 
 * swap buffer function
 * -> prints ogl rendered on screen
 * ---------------------------------------
 */

#ifndef SCREEN_H
#define	SCREEN_H

// include
#include <stdio.h>
#include <string>
#include <functional>
#include "Log.h"
#ifdef pl_pi
#include <bcm_host.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#else
#include <stdint.h>
#include "GL.h"
#endif

using namespace std;
namespace ui
{

/* Screen Class
 */
class Screen : public Log
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
        void onTouchMove(function<void(int x, int y)> onTouchMove)
        { onTouchMoveFunc = onTouchMove; };
        
        // set on touchButton event listener
        void onTouchPress(function<void(int button, int type)> onTouchPress)
        { onTouchPressFunc = onTouchPress; };
        
        // set on resizeScreen event listener
        void onResizeScreen(function<void(int width, int height)> onResizeScreen)
        { onResizeScreenFunc = onResizeScreen; };

        // set on closeWindow event listener
        void onCloseScreen(function<void()> onCloseWindow)
        { onCloseScreenFunc = onCloseWindow; };

        // set on weel scroll event listener
        void onWeelScroll(function<void (int x, int y)> onWeelScroll)
        { onWeelScrollFunc = onWeelScroll; };

        // set on text edit event listener
        //void onTextEdit(function<void (SDL_Event event)> onTextEdit)
        //{ onTextEditFunc = onTextEdit; };


        static uint32_t display_width;
        static uint32_t display_height;
        string display_title;
        
        
    private:
        // -- Var
        
        // eventListener functions
        function<void (int x, int y)> onTouchMoveFunc;
        function<void (int button, int type)>onTouchPressFunc;
        function<void (int width, int height)> onResizeScreenFunc;
        function<void (int x, int y)> onWeelScrollFunc;
        //function<void (SDL_Event event)> onTextEditFunc;
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


};     /* END NAMESPACE */
#endif	/* SCREEN_H */

