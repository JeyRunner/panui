/* 
 * File:   TouchPoint.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * TOUCHPOINT CLASS
 * touch point values: x,y,state
 * 
 * ---------------------------------------
 */


#ifndef TOUCHPOINT_H
#define	TOUCHPOINT_H

#include <stdlib.h>
#include <functional>
#include "View.h"
#include "Ui.h"
using namespace std;


class TouchPoint 
{
    public:
      TouchPoint(Ui *ui);
      ~TouchPoint();
      
      // position, read only
      float x, y;
      
      /* state, ready only
       * @param STATE_UP or STATE_DOWN */
      bool state;
      
      
      // finger/mouse pressed
      void press(int button, int type);
      
      // move point
      void move(float x, float y);
      
//      // views add themselves when isOver
//      void addOver(View *view);
//      
//      // views remove themselves when not isOver
//      void removeOver(View *view);
      
      // state values
      bool STATE_DOWN = true;
      bool STATE_UP    = false;
            
    
      // uis root view
      Ui *ui;
      
      // actual top view point is over
      View *over;
      
      // actual view touch on top is down
      View *down;
      
      
      // actual views point is over
//      list<View*> overList;
      
    private:
        void eachView(View *container,function<void (View *view)> func);
};

#endif	/* TOUCHPOINT_H */

