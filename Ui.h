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

#include "FrameRenderer.h"
#include "GLES/gl.h"
#include "Screen.h"
//#include "View.h"
class View;
class FrameRenderer;

class Ui 
{
    public:
        Ui();
        ~Ui();
        
        // var
        static View *rootView;
        static FrameRenderer *frameRenderer;
        
        
        
        // Functions ----------------------
        static void setRootView(View *view);   // set the rootView
};



#endif	/* UI_H */

