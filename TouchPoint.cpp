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
#include "Box.h"
#include "View.h"
#include "BoxRenderer.h"


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
        {over->onTouchLeaveFunc(over, over->renderer->touchAttributes.relativeSelf, over->renderer->touchAttributes.relativeParent, {x,y});}
            
        if (newOver->onTouchEnterFunc)
        {newOver->onTouchEnterFunc(newOver, newOver->renderer->touchAttributes.relativeSelf, newOver->renderer->touchAttributes.relativeParent, {x,y});}
            
        over = newOver;
    }
    
    // for all parents
    for (View *i = newOver; i; i = i->parent) 
    {
        if (i->onTouchMoveFunc) 
        {i->onTouchMoveFunc(i, i->renderer->touchAttributes.relativeSelf, i->renderer->touchAttributes.relativeParent, {x,y});}   
    }

    
    // for each view
//    eachView(ui->rootView, [&](View *view)
//    {
//        // move
//        if (view->renderer->touchAttributes.isOver && view->onTouchMoveFunc)
//        {view->onTouchMoveFunc(view, {0,0}, {0,0}, {x,y});}
//        
//        // leave
//        if (view->renderer->touchAttributes.leave)
//        { 
//            view->renderer->touchAttributes.leave = false;
//            if (view->onTouchLeaveFunc) 
//            {view->onTouchLeaveFunc(view, {0,0}, {0,0}, {x,y});}
//        }
//                
//        // enter
//        if (view->renderer->touchAttributes.enter)
//        {
//            view->renderer->touchAttributes.enter = false;
//            if (view->onTouchEnterFunc)
//            {view->onTouchEnterFunc(view, {0,0}, {0,0}, {x,y});}
//        }
//    });
}



// -- PRESS -----------------------
void TouchPoint::press(int button, int type) 
{
    // cout << "[TOCH] press" << endl;
    
    switch (type)
    {
        case UI_TOUCH_BUTTON_DOWN:
            if (over->onTouchDownFunc)
                down = over;
                over->onTouchDownFunc(over, {0,0}, {0,0}, {x,y});
            break;
            
        case UI_TOUCH_BUTTON_UP:
            if (over->onTouchUpFunc)
                down = NULL;
                over->onTouchUpFunc(over, {0,0}, {0,0}, {x,y});
            break;
    }
}


//// -- ADD OVER ---------------------
//void TouchPoint::addOver(View* view) 
//{
//    // not already exists
//    if (find(overList.begin(), overList.end(), view) != overList.end())
//    {
//        overList.push_back(view);
//    }
//}
//
//// -- REMOVE OVER ------------------
//void TouchPoint::removeOver(View* view) 
//{
//    overList.remove(view);
//}

// -- FOR EACH VIEW ------------------
void TouchPoint::eachView(View *container, function<void(View*)> func)
{
    Box *b = (Box*)(container);
    func(container);
    
    if (dynamic_cast<Box*>(container))
    {
        // loop
        for (list<View*>::iterator iter = ((Box*)b)->children.begin(); /* iterator to start pos */
                 iter != ((Box*)b)->children.end();                        /* end if iterator at last pos */
                 iter++)
        {
            eachView(*iter, func);
        }
    }
}




// ###########################################
// -- DESTROY OBJEKT -----------
TouchPoint::~TouchPoint() {
}

