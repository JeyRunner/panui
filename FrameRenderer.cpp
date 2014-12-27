/* 
 * File:   FrameRenderer.cpp
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


#include <bits/local_lim.h>
#include <glm/gtc/matrix_transform.hpp> // gl Math matix transform
#include <stdlib.h>
//#include <bits/sigthread.h>
#include "FrameRenderer.h"
#include "Ui.h"
#include "View.h"
#include "Renderer.h"
#include "GL.h"

using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
FrameRenderer::FrameRenderer(Ui* ui) 
{
    this->ui         = ui;
    
    // default params
    framesPerSecond = 24;
    
    // create screen object
    // -> to init screen
    // -> display renderd stuff
    screen = new Screen();
    //screen->eglInitScreen();
}




// ## START STOP #########################################
// -- START ---------------------
void FrameRenderer::start()
{
    int result;
    result = pthread_create(&thread_render_id   /*get thread id*/,
                            NULL                /*thread params => default*/,
                            &thread_render      /*thread function*/,
                            this                /*transpher classes to thread*/);
    // check if successful
    switch(result)
    {
        case 0:
            printf("[ FR ] create render thread [OK] \n");
            break;
            
        default:
            printf("[ FR ] create render thread [ERR] \n");
    }
}

// -- STOP ---------------------
void FrameRenderer::stop() 
{
    int result;
    result = pthread_kill(thread_render_id, 1);
    
    // check if successful
    switch(result)
    {
        case 0:
            printf("[ FR ] kill render thread [OK] \n");
            break;
            
        default:
            printf("[ FR ] kill render thread [ERR] \n");
    }
}



// ## THREAD ##############################################
void *FrameRenderer::thread_render(void* frameRenderer)
{    
    // var
    FrameRenderer *fr = ((FrameRenderer*)frameRenderer);
    
    // bind openGl context to render Thread
    fr->screen->eglInitScreen();
    //fr->screen->eglBindToCurrentThread();
    
    
    // @TODO gl bending settings !!
    // gl bending between overlapping pixels
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)/*GL_ONE, GL_SRC_ALPHA*/;
    
    // create shaders
    GL::init();
    
    // enable depth buffer
    // glEnable(GL_DEPTH_TEST);
    
    // calculate + set projection Matix
    GL::projectionMatix = glm::ortho(  -(float)Screen::display_width/2  /* left */,
                                        (float)Screen::display_width/2  /* right */,
                                       -(float)Screen::display_height/2 /* bottom */,
                                        (float)Screen::display_height/2 /* top */,
            
                                        1.0f                     /* zNear */,
                                       -1.0f                     /* zFar */);
    
    
    
    // set transform Matix for all shaders to middel of screen
    GL::transfomMatix = glm::translate(glm::vec3(0, 0, 0));
    
    int frame_counter = 0;
    
    // -- render loop
    while (true)
    {    
        //cout << "---------- FRAME ----------------------------" << endl;
        // -- timestamp --------
        // get current time since 1.1.1970 in milli seconds
        
//        float time_start = duration_cast<milliseconds>(  system_clock::now().time_since_epock()).count() * 0.001;
        
        
        // -----------------------
        
        
        // calc Layouts for views
        fr->exe_calcTasks();
        
        // render all views
        fr->exe_render();
        
        //cout << endl << endl;
        
        // clac time to wait
        int sleepSeconds;
        sleepSeconds = 100000000* (1/fr->framesPerSecond); // 1 micro S = 10^-6 S

        
        
        // -- calc FPS
//        float time_end = duration_cast<milliseconds>(  system_clock::now().time_since_epock()).count() * 0.001;
//        float time_frame = time_end - time_start;
        
        // change fps event
        if (fr->onFpsChangeFunc)
                fr->onFpsChangeFunc(frame_counter);
        
        
        // wait
        //sleep(sleepSeconds);
        //usleep(100000000* (1/fr->framesPerSecond)); // 1 micro S = 10^-6 S
        frame_counter++;
    }
}


// -- EXECUTE CALCULATION TASKS --------
void FrameRenderer::exe_calcTasks() 
{
    ui->rootView->renderer->exeCaclTasks();
    
    /*
    if (!calcTasks.empty())
    {
        // 10 calculation Tasks per frame
        for (int i; i < calculationTasksPerSecond; i++)
        {
            cout << "[FR] exe calc task" << endl;
            // check if is empty
            if (calcTasks.empty()) 
            { break; }
            
            // get next view
            View *v = calcTasks.front();

            // extrude
            v->renderer->calcLayout();

            // remove view from list
            calcTasks.pop_front();
        }
    }
    else 
    {
        //cout << "[FR] no more calc tasks" << endl;
    }
     */
}


// -- EXECUTE RENDER -------------------
void FrameRenderer::exe_render() 
{
    glViewport(0,0, screen->display_width, screen->display_height);
    
    
    // set background color of screen -> white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    // clear Buffer from before
    glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT */ );
    
    // remove cursor -> centor of screen
    glLoadIdentity();
    
    
    
    // setup camera size ----
    glOrthof(-(float)(screen->display_width/2), (float)(screen->display_width/2)     /* y: from -100 to 100 */, 
             -(float)(screen->display_height/2),  (float)(screen->display_height/2)  /* x: from -100 to 100 */,
             -1, 1                                                                   /* z: from -1   to 1   */);
    
    
    
    // render Root View
    ui->rootView->renderer->render();
    ui->rootView->renderer->resetCursor();
    
    
    // swap Buffers 
    // => display new renderd image
    screen->eglSwapBuffer();
}



// -- SET EVENTS ------------------------------------------
void FrameRenderer::onFpsChange(function<void (float)> fpsChangeFunction)
{
    onFpsChangeFunc = fpsChangeFunction;
}




// ###########################################
// -- DESTROY OBJEKT -----------
FrameRenderer::~FrameRenderer() 
{
}

