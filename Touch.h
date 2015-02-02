/* 
 * File:   Touch.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * TOUCH CLASS
 * contains TouchPoints
 *   currently only one
 *   @TODO multiple touchpoints
 * ---------------------------------------
 */


#ifndef TOUCH_H
#define	TOUCH_H

#include "TouchPoint.h"


class Touch {
    public:
      // init
      static void init(Ui *ui);
        
      // touchPoint
      static TouchPoint *touchPoint;
        
      /* re check if is Over
       * call after layout changes */ 
      static void exe_reCheck();
        
      // need reCheck
      static bool needReCheck;
      
      // touch isOver call count
      static int isOverCount;
};

//TouchPoint *Touch::touchPoint;
// void Touch::exe_reCheck();


#endif	/* TOUCH_H */

