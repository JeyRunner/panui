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
#include "Touch.h"
using namespace ui;


bool TouchPoint::STATE_DOWN = true;
bool TouchPoint::STATE_UP    = false;

// ############################################
// -- CREATE OBJEKT --------------
TouchPoint::TouchPoint(Ui* ui)
{
    setLogName("TOCH");
    trace("create");
    
    // set view
    this->ui   = ui;
    this->over = NULL;
    this->down = NULL;
    this->focus= NULL;
    
    // default parameter
    x = 0;
    y = 0;
    state = STATE_UP;
}

// -- MOVE -----------------------
void TouchPoint::move(float x, float y) 
{
    // if not moved
    if ((this->x == x) && (this->y == y))
        return;

    // new over
    View *newOver;
    
    // set
    this->x = x;
    this->y = y;
    
    // make x,y relative to gl coordinate system
    x = +x - (ui->rootView->renderer->renderAttributes.width  /2);
    y = -y + (ui->rootView->renderer->renderAttributes.height /2);
    
    // reset is over count
    Touch::isOverCount = 0;

    // check over
    newOver = ui->rootView->renderer->isOver(x,y);
    if (newOver == NULL)
        //cout << "touch return null" << endl;
        return;

    // check for null 
    if (over == NULL)
    {
        info("over is null");
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
        //down = NULL;
    }
    
    
    // for all parents
    for (View *i = newOver; i; i = i->parent) 
    {
        if (i->onTouchMoveFunc) 
        {i->onTouchMoveFunc(i, i->renderer->touchAttributes.relativeSelf, i->renderer->touchAttributes.relativeParent, {x,y});}   
    }

    
    // drag and drop
    if (down != NULL)
    {
        // drag start
        if (!down->renderer->touchAttributes.drag)
        {
            if (down->onTouchDragFunc)
            {down->onTouchDragFunc(down, down->renderer->touchAttributes.relativeSelf, down->renderer->touchAttributes.relativeParent, {x,y});}
            down->renderer->touchAttributes.drag = true;
            down->renderer->touchAttributes.relativeSelfDrag = down->renderer->touchAttributes.relativeSelf;
            
            // for all parents
            for (View *i = down; i; i = i->parent) 
            {
                // child need touch move updates
                i->renderer->touchAttributes.childNeedIsOver = true;
            }
        }
        else
        {
            // scroll
            scroll(-x +lastScroll.x, -y +lastScroll.y);
        }
        
        if (down->onTouchDragMoveFunc)
        {down->onTouchDragMoveFunc(down, down->renderer->touchAttributes.relativeSelfDrag, down->renderer->touchAttributes.relativeParent, {x,y});}
    }
    
    lastScroll.x = x;
    lastScroll.y = y;
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


// -- SCROLL ----------------------
void TouchPoint::scroll(float x, float y)
{
    // currently only scroll y
    //debug("scroll y: " + str(y));

    if (over != NULL)
    {
        View *view = focus;//over;
        bool search = true;

        while (search && (view != NULL))
        {
            //debug("check view #" + view->id + "|." + view->class_ + " ");
            //debug("compare: "+ str((view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height)) +" >= " + str(-( view->renderer->renderAttributes.scrollY + y)) + "  " +
            //str(((view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height) >=  -( view->renderer->renderAttributes.scrollY + y)) && (-( view->renderer->renderAttributes.scrollY + y) >= 0)));
            // check is view is scrollable
            if (view->renderer->renderAttributes.contendHeight > view->renderer->renderAttributes.height)
            {
                //debug("view #" + view->id + "|." + view->class_ + " is scrollable");
                //debug("contendHeight: " + str(view->renderer->renderAttributes.contendHeight) + "  real height: " + str(view->renderer->renderAttributes.height));
                // if not reach border
                if (    ((view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height) >=  -( view->renderer->renderAttributes.scrollY + y))
                     && (-( view->renderer->renderAttributes.scrollY + y) >= 0))
                {
                    // scroll this view
                    //view->renderer->renderAttributes.scrollX += x * 15;
                    view->renderer->renderAttributes.scrollY += y;
                    view->renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);

                    // done
                    search = false;
                    break;
                }
                else
                {
                    // reach bottom
                    if (-( view->renderer->renderAttributes.scrollY + y) <= 0)
                    {
                        //debug("reach top");

                        view->renderer->renderAttributes.scrollY = 0;
                        view->renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);
                        //y-= (view->renderer->renderAttributes.scrollY + y);
                    }
                    // reach top
                    if (((view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height) + (view->renderer->renderAttributes.scrollY + y)) <= 0 )
                    {
                        //debug("reach bottom");

                        view->renderer->renderAttributes.scrollY = -(view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height);
                        view->renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);
                        //y-= (view->renderer->renderAttributes.contendHeight - view->renderer->renderAttributes.height) + y;
                    }

                    // check parent
                    view = view->parent;
                    //search = false;
                }
            }
            else
            {
                // check parent
                view = view->parent;
            }
        }
    }
}



// -- PRESS -----------------------
void TouchPoint::press(int button, int type) 
{
    // cout << "[TOCH] press" << endl;
    
    switch (type)
    {
        case UI_TOUCH_BUTTON_DOWN:
            focus = over;
            down = over;
            if (over->onTouchDownFunc)
            {
                over->onTouchDownFunc(over, {0,0}, {0,0}, {x,y});                
            }
            break;
            
        case UI_TOUCH_BUTTON_UP:
            if (over->onTouchUpFunc)
                over->onTouchUpFunc(over, {0,0}, {0,0}, {x,y});
                
            // drop (drag end)
            if (down && down->renderer->touchAttributes.drag)
            {
                if (down->onTouchDropFunc)
                {down->onTouchDropFunc(down, down->renderer->touchAttributes.relativeSelf, down->renderer->touchAttributes.relativeParent, {x,y});}
                
                down->renderer->touchAttributes.drag = false;

                // for all parents
                for (View *i = down; i; i = i->parent) 
                {
                    // child not need touch move updates
                    i->renderer->touchAttributes.childNeedIsOver = false;
                }
            }
            down = NULL;
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
    
    //if (dynamic_cast<Box*>(container))
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

