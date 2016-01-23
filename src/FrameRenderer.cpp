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


//#include <bits/local_lim.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // gl Math matix transform
#include <stdlib.h>
#include <unistd.h>
//#include <bits/sigthread.h>
#include "TouchPoint.h"
#include "FrameRenderer.h"
#include "Ui.h"
#include "View.h"
#include "Renderer.h"
#include "GL.h"
#include "Style.h"
#include "Touch.h"

using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
FrameRenderer::FrameRenderer(Ui* ui) 
{
    setLogName(" FR ");

    this->ui         = ui;
    
    // default params
    framesPerSecond = 24;
    
    // create screen object
    // -> to init screen
    // -> display renderd stuff
    screen = new Screen();
    //screen->eglInitScreen();
    
    // init Touch
    Touch::init(ui);
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
    out("create render thread", result != 0, "");
}

// -- STOP ---------------------
void FrameRenderer::stop() 
{
    int result = 1;
    //result = pthread_kill(thread_render_id, 1);
    
    // check if successful
    out("kill render thread", result != 0, "");
}



// ## THREAD ##############################################
void *FrameRenderer::thread_render(void* frameRenderer)
{    
    // var
    FrameRenderer *fr = ((FrameRenderer*)frameRenderer);
    
    // set screen events
    fr->screen->onResizeScreen([&](int width, int height){
        fr->screen->resizeScreen(width, height);
        glViewport(0, 0, width, height);
         
        // calculate + set projection Matix
        GL::projectionMatix = glm::ortho(-(float)width/2  /* left */,
                                             (float)width/2  /* right */,
                                            -(float)height/2 /* bottom */,
                                             (float)height/2 /* top */,

                                             1.0f                     /* zNear */,
                                            -1.0f                     /* zFar */);
        
        // update rootView size
        fr->ui->rootView->style->width->set(width);
        fr->ui->rootView->style->height->set(height);
           
        // replace missing parent
        fr->ui->rootView->renderer->renderAttributes.width  = width;
        fr->ui->rootView->renderer->renderAttributes.height = height;
        //fr->ui->rootView->renderer->calcLayoutSize();

        // recalc
        fr->ui->rootView->renderer->addCalcTask(UI_CALCTASK_LAYOUT_CHILDREN_POSITION);
    });
    
    fr->screen->onCloseScreen([&](){
        fr->screen->closeScreen();
        fr->ok("window is closed");
        exit(0);
    });
    
    // touch move
    fr->screen->onTouchMove([&](int x, int y)
    {
        Touch::touchPoint->move(x,y);
    });
    
    // touch press
    fr->screen->onTouchPress([&](int button, int type)
    {
        //cout << "[ FR ] touch press button:"<< button <<"  type:"<< type << endl;
        Touch::touchPoint->press(button, type);
    });

    // on weel scroll
    fr->screen->onWeelScroll([&](int x, int y)
    {
        Touch::touchPoint->scroll(x*20,y*20);
    });
    
    
    
    
    // create screen / window
    fr->screen->initScreen();
    
    // @TODO gl bending settings !!
    // gl bending between overlapping pixels
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)/*GL_ONE, GL_SRC_ALPHA*/;
    
    // create shaders
    GL::init();
    
    // enable depth buffer
    // glEnable(GL_DEPTH_TEST);

    // enable stencil buffer
    glEnable(GL_STENCIL_TEST);

    // stencilBuffer - write if value if func succed
    glStencilOp(GL_KEEP    /* stencil func fail */,
                GL_REPLACE /* stencil func ok, but deep fail =>> set to value */,
                GL_REPLACE /* both ok                        =>> set to value*/);

    fr->debug("------------- v4");
  
    
    // set transform Matix for all shaders to middle of screen
    GL::transfomMatix = glm::translate(glm::vec3(0, 0, 0));
    
    int   frame_counter = 0;
    float timeStart     = 0;
    float timeMiddle    = 0;
    float timeStopp     = 0;
    float fpsCalc       = 0;
    float fpsRen        = 0;
    
    
    // -- render loop
    while (true)
    {    
        //cout << "---------- FRAME ----------------------------" << endl;
        // -- timestamp --------


        // get current time since 1.1.1970 in milli seconds
        #ifndef pl_pi
        timeStart = SDL_GetTicks();
        #endif
        // float time_start = duration_cast<milliseconds>(  system_clock::now().time_since_epock()).count() * 0.001;
        
        
        // check for new screen events
        fr->screen->checkEvents();
        
        
        // calc Layouts for views
        fr->exe_calcTasks();
        
        // reCheck touch if necessary
        Touch::exe_reCheck();
        
        #ifndef pl_pi
        timeMiddle = SDL_GetTicks();
        #endif
        
        // render all views
        fr->exe_render();
       
        //cout << endl << endl;
        
        // clac time to wait
        int sleepSeconds;
        sleepSeconds = 100000000* (1/fr->framesPerSecond); // 1 micro S = 10^-6 S

        
        
        // -- calc FPS
//        float time_end = duration_cast<milliseconds>(  system_clock::now().time_since_epock()).count() * 0.001;
//        float time_frame = time_end - time_start;
        #ifndef pl_pi
        timeStopp = SDL_GetTicks();
        #endif
//        float fpsNew = 1/(((float)timeStopp - (float)timeStart)/1000);
        
//        if ((fpsNew < fps-5) || (fpsNew > fps+5))
//            fps = fpsNew;
        fpsCalc = timeMiddle - timeStart   + fpsCalc;
        fpsRen  = timeStopp  - timeMiddle  + fpsRen;
        
        // change fps event
        if (fr->onFpsChangeFunc && frame_counter >= 5)
        {
            #ifndef pl_pi
            fr->onFpsChangeFunc(fpsCalc / 5, fpsRen / 5);
            fpsCalc = 0;
            fpsRen  = 0;
            frame_counter = 0;
            #else
            fr->onFpsChangeFunc(frame_counter, frame_counter);
            #endif
        }
        
        
        // wait
        //sleep(sleepSeconds);
        //usleep(100000000* (1/fr->framesPerSecond)); // 1 micro S = 10^-6 S
        frame_counter++;
    }
}


// -- EXECUTE CALCULATION TASKS --------
void FrameRenderer::exe_calcTasks() 
{
    ui->rootView->renderer->exeCalcTasks();
    
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
#ifdef pl_pi
    glViewport(0,0, screen->display_width, screen->display_height);
    // -> already set after window resize event
#endif
    // reset projection matrix
    // @TODO bug: add view at runtime -> transform matrix is shifted 
    // set transform Matix for all shades to middle of screen
    GL::transfomMatix = glm::translate(glm::vec3(0, 0, 0));
    


    // set background color of screen -> white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // clear stencil with 0
    glClearStencil(0x00);
    
    // clear Buffer from before
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    // remove cursor -> centor of screen
    //glLoadIdentity();

    
    
    // render Root View
    ui->rootView->renderer->render();
    ui->rootView->renderer->resetCursor();
    

    // swap Buffers 
    // => display new renderd image
    screen->swapBuffer();
}



// -- SET EVENTS ------------------------------------------
void FrameRenderer::onFpsChange(function<void (float, float)> fpsChangeFunction)
{
    onFpsChangeFunc = fpsChangeFunction;
}




// ###########################################
// -- DESTROY OBJEKT -----------
FrameRenderer::~FrameRenderer() 
{
}

