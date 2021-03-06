/* 
 * File:   FrameRenderer.h
 * Author: Joshua Johannson | Pancake
 * 
 *
 * ---------------------------------------
 * FRAMERENDERER CLASS
 * render rootView of UI CLASS
 *   prints renderd views on screen
 *   -> SCREEN->eglSwapBuffer()
 * in time intervalles -> fps
 * ---------------------------------------
 */



#ifndef FRAMERENDERER_H
#define	FRAMERENDERER_H

// include
#include "Screen.h"
#include <pthread.h>
#include <stdio.h>
#include <functional>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
namespace ui
{
class Ui;
class View;
class TouchPoint;

using namespace std;


/* FrameRenderer Class
 */
class FrameRenderer : protected Log
{
    public:
        FrameRenderer(Ui *ui);
        ~FrameRenderer();
        
        
        // FrameRenderer properties
        Ui  *ui;                // ui class to render
        int framesPerSecond;    // fixed render frame rate
        
        // Screen to swap buffers to display image
        Screen *screen;
        
        // start, stopp render
        void start();
        void stop();
        
        /* @Events:
         * -> set function
         *    -> on frames per seoncond change */
        void onFpsChange(function<void (float fps, float fpsRender)> fpsChangeFunction);
        
        
        
    private:      
      
        // render thread
        static void* thread_render(void* frameRenderer); // thread function
        pthread_t    thread_render_id;                   // thread id
        
        
        // calculate layoutchanges of views -> in 
        void exe_calcTasks();
        
        // render views of ui, swarp buffers
        void exe_render();
        
        
        // events
        function<void (float fps, float fpsRender)> onFpsChangeFunc;
        
        
};


};     /* END NAMESPACE */
#endif	/* FRAMERENDERER_H */

