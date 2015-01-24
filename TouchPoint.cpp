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
    cout << "[TOCH] move" << endl;
    
    // new over
    View *newOver;
    
    // make x,y relative to gl coordinate system
    x = +x - (ui->rootView->renderer->renderAttributes.width  /2);
    y = -y + (ui->rootView->renderer->renderAttributes.height /2);
    cout << "before isOver" << endl;
    
    // check over
    newOver = ui->rootView->renderer->isOver(x,y);
    cout << "after isOver" << endl;
    if (newOver == NULL)
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
        cout << "over view changed" << endl;
        
        if (over->onTouchLeaveFunc)
        { cout<<"function is set"<<endl; over->onTouchLeaveFunc(over, {0,0}, {0,0}, {x,y});}
            
        if (newOver->onTouchEnterFunc)
        {newOver->onTouchEnterFunc(newOver, {0,0}, {0,0}, {x,y});}
            
        over = newOver;
    }
    
    if (over->onTouchMoveFunc) 
    {over->onTouchMoveFunc(newOver, {0,0}, {0,0}, {x,y});}   
}

// -- DOWN -----------------------
void TouchPoint::down() 
{
    cout << "[TOCH] down" << endl;
    
    if (over->onTouchDownFunc)
        over->onTouchDownFunc(over, {0,0}, {0,0}, {x,y});
}

// -- UP -------------------------
void TouchPoint::up() 
{
    cout << "[TOCH] up" << endl;
    
    if (over->onTouchUpFunc)
        over->onTouchUpFunc(over, {0,0}, {0,0}, {x,y});
}



// ###########################################
// -- DESTROY OBJEKT -----------
TouchPoint::~TouchPoint() {
}

