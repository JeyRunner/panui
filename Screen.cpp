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
#include <GLES2/gl2.h>


#include "Screen.h"
using namespace std;

// static
uint32_t Screen::display_width;
uint32_t Screen::display_height;



// ############################################
// -- Var
// -- egl attributes
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
 


// -- CREATE OBJEKT --------------
Screen::Screen() 
{
}



// -- INIT SCREEN WITH EGL ------
bool Screen::eglInitScreen()
{   
    // -- Var
    static EGL_DISPMANX_WINDOW_T nativewindow;
    
    // init bmc host -> for display
    bcm_host_init();

    // start ----
    printf("[DISP] egl Init \n");


    // create EGL(nessasary at no xwidow)indow surface, passing context width/height
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
}




// -- EGL BIND TO CURRENT THREAD ------
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



// -- EGL SWAP BUFFERS ---------
void Screen::eglSwapBuffer()
{
    // EGL prints drawed stuff from openGL on screen
    eglSwapBuffers(display, surface);
}


// ###########################################
// -- DESTROY OBJEKT -----------
Screen::~Screen() 
{}

