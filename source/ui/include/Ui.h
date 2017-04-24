/* 
 * File:   Ui.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 *  ---------------------------------------
 * UI CLASS
 * render its root view
 * prints renderd views on screen
 * -> SCREEN->eglSwapBuffer()
 * ---------------------------------------
 */


#ifndef UI_H
#define	UI_H

// include
#ifdef pl_pi
#include "GLES/gl.h"
#endif

#include "FrameRenderer.h"
#include "Log.h"
#include "Screen.h"
#include "internSDL/SDL_android_main.c"
namespace ui
{
class View;
class FrameRenderer;



/* Ui Class
 */
class Ui : protected Log
{
    public:
        Ui();
        Ui(int width, int height, string title);
        ~Ui();
        
        // init - call before any other commands !
        static void init();
        
        // var
        static View *rootView;
        static FrameRenderer *frameRenderer;
        
        
        
        // Functions ----------------------
        static void setRootView(View *view);   // set the rootView
        
};


};     /* END NAMESPACE */
#endif	/* UI_H */

