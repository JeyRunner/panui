/* 
 * File:   Touch.cpp
 * Author: joshua
 * 
 * Created on 25. Januar 2015, 00:10
 */

#include "Touch.h"
using namespace ui;

// const
TouchPoint *Touch::touchPoint;
bool        Touch::needReCheck;
int         Touch::isOverCount;
//void        Touch::exe_reCheck();


// -- INIT ------------------------
void Touch::init(Ui* ui) 
{
    needReCheck = false;
    isOverCount = 0;
    touchPoint  = new TouchPoint(ui);
}


// -- RECHECK ---------------------
void Touch::exe_reCheck() 
{
    if (needReCheck)
    {
        // cout << "[TOCH] exe reCheck" << endl;
        touchPoint->move(touchPoint->x,touchPoint->y);
        needReCheck = false;
    }
}