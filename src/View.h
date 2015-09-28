/* 
 * File:   View.h
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * VIEW CLASS
 * basic ui element
 * render itself
 * ---------------------------------------
 */


#ifndef VIEW_H
#define	VIEW_H

//#include "StyleRule.h"
//class StyleRule;
#include "Log.h"
//#include "Renderer.h"
//#include "BoxView.h" // => problems with circular including
class Renderer;
class Box;
class StyleRule;

#include <string>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <functional>

#include "const.h"

using namespace std;


class View : public Log
{
    public:
        View();
        View(string id, string class_);
        View(bool DoNothing);
        virtual ~View();

        
        // View properties ---------------
        string id, class_;
        string type; // View, Box, ...
        
        // parent view
        Box *parent;
        
        // own Style
        StyleRule *style;
        
        
        // set events
        void onTouchMove(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchMoveFunc);
        void onTouchEnter(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchEnterFunc);
        void onTouchLeave(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchLeaveFunc);
        void onTouchDown(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDownFunc);
        void onTouchUp(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchUpFunc);
        void onTouchDrag(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDragFunc);
        void onTouchDragMove(function<void (View *v, Point relativeSelfDrag, Point relativeParent, Point absolute)> onTouchDragMoveFunc);
        void onTouchDrop(function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDropFunc);
        
        
        // list of bonded styles
        list<StyleRule*> styles;
        list<StyleRule*>::iterator stylesIter;
        
       
        // renderer
        Renderer *renderer;
        
        /* get styles and bind ...              \n
         * call after assign id, class ,... !! */
        void getStyle();
        


        // events        
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchMoveFunc; 
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchEnterFunc; 
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchLeaveFunc; 
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchUpFunc; 
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDownFunc; 
        function<void (View *v, Point relativeSelfDrag, Point relativeParent, Point absolute)> onTouchDragFunc;
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDragMoveFunc; 
        function<void (View *v, Point relativeSelf, Point relativeParent, Point absolute)> onTouchDropFunc; 
};

#endif	/* VIEW_H */

