/* 
 * File:   Ui.cpp
 * Author: Pancake
 * 
 * 
 * ---------------------------------------
 * UI CLASS
 * render its root view
 * prints renderd views on screen
 * -> SCREEN->eglSwapBuffer()
 * ---------------------------------------
 */


#include "Ui.h"
#include "View.h"
#include "Renderer.h"
#include "GL.h"
using namespace std;


// ############################################
// -- Var
// -- init static var
View *(Ui::rootView);
FrameRenderer *(Ui::frameRenderer);


// -- CREATE OBJEKT --------------
Ui::Ui() 
{    
    // create frameRenderer object
    // -> init screen
    // -> render in intervalles
    frameRenderer = new FrameRenderer(this);
    
    // init pre
    // => before any View is created
    GL::initPre();
}


// -- SET ROOT VIEW ------------
void Ui::setRootView(View *view)
{
    // set var
    rootView = view;
    
    // position of rootView
    // -> middel of screen
    rootView->renderer->renderAttributes.positionX = 0;
    rootView->renderer->renderAttributes.positionY = 0;
}




// ###########################################
// -- DESTROY OBJEKT -----------
Ui::~Ui() 
{
}

