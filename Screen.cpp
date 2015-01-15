/* 
 * File:   EglScreen.cpp
 * Author: Pancake
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


// ## INCLUDE
#include <stdio.h>
#include <iostream>


#ifdef pl_pi
#include <GLES2/gl2.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Screen.h"
using namespace std;

// static
uint32_t Screen::display_width;
uint32_t Screen::display_height;


// ############################################
// -- Var
// -- egl attributes
#ifdef pl_pi
 static const EGLint attribute_list[] =
   {
      EGL_RED_SIZE,     8,
      EGL_GREEN_SIZE,   8,
      EGL_BLUE_SIZE,    8,
      EGL_ALPHA_SIZE,   8,
      // EGL_DEPTH_SIZE,   8,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
   };
 
 // -- context attributes
 // -- define gl version
 static const EGLint context_attribute_list[] =
 {
     EGL_CONTEXT_CLIENT_VERSION, 2 /* Open Gl ES 2.x */,
     EGL_NONE
 };
#else
 SDL_Window *window;
#endif


// -- CREATE OBJEKT --------------
Screen::Screen() 
{
    // default
    display_width  = 500;
    display_height = 500;
    display_title    = "";
}



// -- INIT SCREEN ---------------
bool Screen::initScreen() 
{
    #ifdef pl_pi
        eglInitScreen();
    #else
        sdlInitScreen(display_width, display_height, display_title);
    #endif
}

// -- RESIZE SCREEN ----------------------------------------------
void Screen::resizeScreen(int width, int height) 
{
#ifndef pl_pi
    // SDL
    SDL_SetWindowSize(window, width, height);
    
#endif
}

// -- SET TITLE -------------------------------------------------
void Screen::setTitle(string title) 
{
 #ifndef pl_pi
     // set window titel
    SDL_SetWindowTitle(window, title.c_str());
#endif
}





// -- SDL INIT SCREEN --------------------------------------------
bool Screen::sdlInitScreen(int width, int height, string title) 
{
    cout << "[DISP] create Window with sdl [...]" << endl;
    
#ifndef pl_pi
    
    display_width  = width;
    display_height = height;
    int error = 0;
    SDL_Surface *surface = NULL;
    
    // init sdl -----------------------------
    error = SDL_Init(SDL_INIT_VIDEO);
    if (error != 0)
    {
        cout << "[DISP] SDL_Init [ERR]" << endl;
        return false;
    }
    else 
        cout << "[DISP] SDL_Init [OK]" << endl;
    
    
    // activate doublebuffering
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // enable antialasing with multisampeling
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    

    // open window - init projection matrix ...
    window = SDL_CreateWindow(title.c_str(), 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                width, height, 
                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
   
    // create context
    SDL_GL_CreateContext(window);
    
    // init matrix ...
    onResizeScreenListener(display_width, display_height);
    
    // init glew
    glewInit();
    
    // output ok
    cout << "[DISP] create Window with sdl [OK]" << endl;
    
    // print open gl info
    printf("[DISP] Open Gl version: '%s' \n",     glGetString(GL_VERSION));
    printf("[DISP] Open GlSl version: '%s' \n",   glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("[DISP] Open Gl renderer: '%s' \n",    glGetString(GL_RENDERER));
    printf("[DISP] Open Gl extensions: '%s' \n",  glGetString(GL_EXTENSIONS));
    printf("[DISP] Open Gl max texture size: '%s' \n",  glGetString(GL_MAX_TEXTURE_SIZE));
    
    // enable antialiasing with multisampeling
    // glEnable(GL_MULTISAMPLE);
    
#endif
}



// -- INIT SCREEN WITH EGL ----------------------------------------
bool Screen::eglInitScreen()
{   
    cout << "[DISP] init screen with egl ..." << endl;
    
#ifdef pl_pi
    // -- Var
    static EGL_DISPMANX_WINDOW_T nativewindow;
    
    // init bmc host -> for display
    bcm_host_init();

    // start ----
    printf("[DISP] egl Init \n");


    // create EGL(necessary at no xwidow)indow surface, passing context width/height
    int success = graphics_get_display_size(0 /*LCD Mode*/,
                                            &display_width,  &display_height);
    if (success < 0)
        printf("[DISP] at graphics_get_display_size() [ERR]");
    else
        printf("[DISP] graphics_get_display_size [OK] \n");
        printf("[DISP] width: %d  height: %d  \n", display_width, display_height);


    // manual displaysize
    display_width = display_width;
    display_height = display_height; //atoi(argv[1]);
    

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = display_width;
    dst_rect.height = display_height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = display_width << 16;
    src_rect.height = display_height << 16;




    // open display ----------
    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    // alpha -----------------
    // VC_DISPMANX_ALPHA_T         dispman_alpha;
    // dispman_alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_EXCEED_0X07;//DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS; 
    // dispman_alpha.opacity = 0xFF; 
    // dispman_alpha.mask = NULL; 
    
    dispman_element = vc_dispmanx_element_add ( dispman_update,
          dispman_display, 0/*layer*/, &dst_rect, 0/*src*/,
          &src_rect, DISPMANX_PROTECTION_NONE, 0 /*&dispman_alpha */ /*alpha*/,
          0/*clamp*/, DISPMANX_NO_ROTATE/*transform*/);


    nativewindow.element = dispman_element;
    nativewindow.width = display_width;
    nativewindow.height = display_height;
    vc_dispmanx_update_submit_sync( dispman_update );


    // get an EGL display connection
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if ( display == EGL_NO_DISPLAY )
        printf("[DISP] at eglGetDisplay() [ERR] \n");
    else
        printf("[DISP] eglGetDisplay [OK] \n");



    // initialize the EGL display connection ----------
    int major,minor;

    EGLBoolean result = eglInitialize(display, &major, &minor);
    printf("[DISP] EGL init version %d.%d \n", major, minor);
    if(result == EGL_FALSE)
        printf("[DISP] at eglInitialize() [ERR] \n");
    else
        printf("[DISP] eglInitialize [OK] \n");
    

    // get, set our config from the config class
    eglChooseConfig(display, attribute_list, &config, 1, &num_config); //m_config->chooseConfig(display);
    //config = m_config->getConfig();

    // bind the OpenGL API to the EGL
//    result = eglBindAPI(EGL_OPENGL_ES_API);
//    if(result ==EGL_FALSE)
//        printf("[DISP] at eglBindAPI() [ERR] \n");
//    else
//        printf("[DISP] eglBindAPI [OK] \n");
    
    
    // create context ----------------------------
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribute_list /* define Open Gl version (default 1.x) */);
    if(context ==EGL_NO_CONTEXT)
        printf("[DISP] at eglCreateContext() [ERR] \n");
    else
        printf("[DISP] eglCreateContext [OK] \n");

    
    
    
    // create a new surface using this window
    surface = eglCreateWindowSurface(display, config,
                             &nativewindow, NULL);
    if ( surface == EGL_NO_SURFACE )
        printf("[DISP] at eglCreateWindowSurface() [ERR] \n");
    else
        printf("[DISP] eglCreateWindowSurface [OK] \n");
   
    
    
    // connect the context to the surface
    // and connect to actual thread !!!!!!!
    if ( !eglMakeCurrent(display, surface, surface, context) )
        printf("[DISP] at eglMakeCurrent() [ERR] \n");
    else
        printf("[DISP] eglMakeCurrent [OK] \n");
              
    
    
    // print open gl info
    printf("[DISP] Open Gl version: '%s' \n",     glGetString(GL_VERSION));
    printf("[DISP] Open GlSl version: '%s' \n",   glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("[DISP] Open Gl renderer: '%s' \n",    glGetString(GL_RENDERER));
    printf("[DISP] Open Gl extensions: '%s' \n",  glGetString(GL_EXTENSIONS));
    printf("[DISP] Open Gl max texture size: '%s' \n",  glGetString(GL_MAX_TEXTURE_SIZE));
    
    // init projection matrix ...
    onResizeScreenListener(display_width, display_height);
#endif
}




// -- EGL BIND TO CURRENT THREAD ------
/*
bool Screen::eglBindToCurrentThread()
{
     // bind the OpenGL API to the EGL
    int result = EGL_FALSE;
    
    result = eglBindAPI(EGL_OPENGL_ES_API);
    if(result ==EGL_FALSE)
    {
        // something goes wrong
        printf("[ERR] at eglBindAPI() \n");
    }
    else
    {
        printf("[OK] eglBindAPI \n");
    }
    
    
    // connect the context to the surface
    // and connect to actual thread !!!!!!!
    if ( !eglMakeCurrent(display, surface, surface, context) )
    {
        // something goes wrong
        printf("[ERR] at eglMakeCurrent() \n");
        
    }
    else
    {
        printf("[OK] eglMakeCurrent \n");
        
    }  
}
 */



// -- EGL SWAP BUFFERS ---------
void Screen::swapBuffer()
{
    // prints drawed stuff from openGL on screen
#ifdef pl_pi
    // EGL
    eglSwapBuffers(display, surface);
    
#else
    SDL_GL_SwapWindow(window);
#endif
}




// -- EVENTS ----------------------------------------------------------------------
// -- CHECK FOR NEW EVENTS -----------------
void Screen::checkEvents() 
{
#ifdef pl_pi
    /* @TODO check for pi screen events
     * - mouse move ... */
#else
    // with sdl ---------
    SDL_Event event;
    
    // get all sdl events in loop
    while (SDL_PollEvent(&event))
    {
        // switch types
        switch (event.type)
        {
            
            // event from window
            case SDL_WINDOWEVENT:
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        int display_width = event.window.data1; 
                        int display_height = event.window.data2;
                            
                        onResizeScreenListener(display_width,
                                                  display_height);
                            
                        break;
                }
                break;
                
                
            case SDL_MOUSEMOTION:
                int x, y;
                x = event.motion.x;
                y = event.motion.y;
                onTouchMoveListener(x, y);
                
                break;
                
            // window closed
            case SDL_QUIT:
                onCloseScreenListener();
                break;
        }
    }
    
    
#endif
}




// -- SET ON RESIZE SCREEN EVENT LISTENER --
void Screen::onTouchMove(function<void(int x, int y)> onTouchMove) 
{ this->onTouchMoveListener = onTouchMove; }

// -- SET ON RESIZE SCREEN EVENT LISTENER --
void Screen::onResizeScreen(function<void(int width, int height)> onResizeScreen) 
{ this->onResizeScreenListener = onResizeScreen; }

// -- SET ON CLOSE WINDOW EVENT LISTENER --
void Screen::onCloseScreen(function<void()> onCloseWindow) 
{ this->onCloseScreenListener = onCloseWindow; }



// -- CLOSE SCREEN -------------------------------------------
void Screen::closeScreen() 
{
#ifndef pl_pi
    // sdl
    SDL_Quit();
#endif
}



// ###########################################
// -- DESTROY OBJEKT -----------
Screen::~Screen() 
{}

