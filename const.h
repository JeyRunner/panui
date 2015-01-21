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

// -- CONST
// -- calculation Task types --------------------------
const int UI_CALCTASK_NONE                      = 0,
          UI_CALCTASK_LAYOUT_SIZE               = 1,
          UI_CALCTASK_LAYOUT_CHIDREN_POSITION   = 2,
          UI_CALCTASK_TEXT_FAMILY               = 3,
          UI_CALCTASK_TEXT_SIZE                 = 4,
          UI_CALCTASK_TEXT_ATLAS                = 5,
          UI_CALCTASK_TEXT_TEXT                 = 6,
          UI_CALCTASK__SIZE                     = 6;

// -- view ---------------------------------------------
const int UI_VIEW_SELF                          = 0,
          UI_VIEW_PARRENT                       = 1;

// -- style attribute properties -----------------------
const int UI_ATTR_POSITION_ABSOLUTE = 0,
          UI_ATTR_POSITION_RELATIVE = 1;

// -- style attribute auto -----------------------------
enum UI_ATTR_AUTO {
     UI_ATTR_AUTO_AUTO,
     UI_ATTR_AUTO_NONE
};

// -- Int-, FloatAttribute mode ------------------------
const bool UI_ATTR__MODE_VALUE      = true,
           UI_ATTR__MODE_PERCENT    = false;


#endif	/* CONST_H */

