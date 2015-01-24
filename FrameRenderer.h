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

//#include "Ui.h
#include "Screen.h"
#include <pthread.h>
#include <chrono>     // system time 
#include <stdio.h>
#include <functional>
#include <deque>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

class Ui;
class View;
class TouchPoint;

class FrameRenderer 
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
        
        // Touch Point
        TouchPoint *touchPoint;
        
        
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

#endif	/* FRAMERENDERER_H */

