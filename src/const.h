/* 
 * File:   const.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 *  ---------------------------------------
 * CONSTANT
 * contains constants
 *  ---------------------------------------
 */


#ifndef CONST_H
#define	CONST_H
namespace ui
{


// -- CONST
// -- calculation Task types --------------------------
const int UI_CALCTASK_NONE                      = 0,
          UI_CALCTASK_LAYOUT_SIZE               = 1,
          UI_CALCTASK_LAYOUT_CHILDREN_POSITION  = 2,
          UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND  = 3,    // calc own height dependent on contend
          UI_CALCTASK_LAYOUT_SIZE_VERTICES      = 4,
          UI_CALCTASK_TEXT_FAMILY               = 5,
          UI_CALCTASK_TEXT_SIZE                 = 6,
          UI_CALCTASK_TEXT_ATLAS                = 7,
          UI_CALCTASK_TEXT_TEXT                 = 8,
          UI_CALCTASK__SIZE                     = 8;

// -- view ---------------------------------------------
const int UI_VIEW_SELF                          = 0,
          UI_VIEW_PARENT                        = 1;

// -- style attribute properties -----------------------
enum UI_ATTR_POSITION_ {
    UI_ATTR_POSITION_ABSOLUTE,
    UI_ATTR_POSITION_RELATIVE
};
enum UI_ATTR_OVERFLOW_ {
    UI_ATTR_OVERFLOW_VISIBLE,
    UI_ATTR_OVERFLOW_HIDDEN
};

// -- style attribute auto -----------------------------
enum UI_ATTR_AUTO {
     UI_ATTR_AUTO_AUTO,
     UI_ATTR_AUTO_NONE
};

// -- Int-, NumericAttribute mode ------------------------
const bool UI_ATTR__MODE_VALUE      = true,
           UI_ATTR__MODE_PERCENT    = false;


// -- touch events -------------------------------------
const int  UI_TOUCH_BUTTON_DOWN     = 0,
           UI_TOUCH_BUTTON_UP       = 1,
           UI_TOUCH_BUTTON_LEFT     = 2,
           UI_TOUCH_BUTTON_MIDDLE   = 3,
           UI_TOUCH_BUTTON_RIGHT    = 4;

// -- 2D point -----------------------------------------
struct Point 
{ float x, y; };

// -- LOG LEVEL ----------------------------------------
const char  UI_LOG_LEVEL_NOTHING       = 0b00000000,
            UI_LOG_LEVEL_ERROR         = 0b00000001,
            UI_LOG_LEVEL_WARNING       = 0b00000010,
            UI_LOG_LEVEL_OK            = 0b00000100,
            UI_LOG_LEVEL_DEBUG         = 0b00001000,
            UI_LOG_LEVEL_INFORMATION   = 0b00010000,
            UI_LOG_LEVEL_TRACE         = 0b00100000,
            UI_LOG_LEVEL_ALL           = 0b11111111;


};     /* END NAMESPACE */
#endif	/* CONST_H */

