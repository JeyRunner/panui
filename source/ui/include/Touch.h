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

// include
#include "TouchPoint.h"

namespace ui
{

/* Touch Class
 */
class Touch : protected Log {
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


};     /* END NAMESPACE */
#endif	/* TOUCH_H */

