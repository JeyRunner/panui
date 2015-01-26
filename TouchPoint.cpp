/* 
 * File:   TouchPoint.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * TOUCHPOINT CLASS
 * touch point values: x,y,state
 * 
 * ---------------------------------------
 */


#include "TouchPoint.h"
#include "Renderer.h"
#include "FrameRenderer.h"
#include "View.h"


// ############################################
// -- CREATE OBJEKT --------------
TouchPoint::TouchPoint(Ui* ui)
{
    cout << "[TOCH] create" << endl;
    
    // set view
    this->ui   = ui;
    this->over = NULL;
    
    // default parameter
    x = 0;
    y = 0;
    state = STATE_UP;
}

// -- MOVE -----------------------
void TouchPoint::move(float x, float y) 
{   
    // new over
    View *newOver;
    
    // set
    this->x = x;
    this->y = y;
    
    // make x,y relative to gl coordinate system
    x = +x - (ui->rootView->renderer->renderAttributes.width  /2);
    y = -y + (ui->rootView->renderer->renderAttributes.height /2);
    
    
    // check over
    newOver = ui->rootView->renderer->isOver(x,y);
    if (newOver == NULL)
        //cout << "touch return null" << endl;
        return;
            
    // check for null 
    if (over == NULL)
    {
        cout << "over is null" << endl;
        over = newOver;
    }
    
    // over new view
    if (newOver != over)
    {
        if (over->onTouchLeaveFunc)
        {over->onTouchLeaveFunc(over, {0,0}, {0,0}, {x,y});}
            
        if (newOver->onTouchEnterFunc)
        {newOver->onTouchEnterFunc(newOver, {0,0}, {0,0}, {x,y});}
            
        over = newOver;
    }
    
    if (over->onTouchMoveFunc) 
    {over->onTouchMoveFunc(newOver, {0,0}, {0,0}, {x,y});}   
}



// -- PRESS -----------------------
void TouchPoint::press(int button, int type) 
{
    // cout << "[TOCH] press" << endl;
    
    switch (type)
    {
        case UI_TOUCH_BUTTON_DOWN:
            if (over->onTouchDownFunc)
                over->onTouchDownFunc(over, {0,0}, {0,0}, {x,y});
            break;
            
        case UI_TOUCH_BUTTON_UP:
            if (over->onTouchUpFunc)
                over->onTouchUpFunc(over, {0,0}, {0,0}, {x,y});
            break;
    }
}




// ###########################################
// -- DESTROY OBJEKT -----------
TouchPoint::~TouchPoint() {
}

