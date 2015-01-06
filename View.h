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

//#include "Renderer.h"
//#include "BoxView.h" // => problems with circular including
class Renderer;
class Box;
class StyleRule;

#include <string>
#include <stdlib.h>
#include <iostream>
#include <list>

using namespace std;


class View 
{
    public:
        View();
        View(string id, string class_);
        View(bool DoNothing);
        ~View();

        
        // View properties ---------------
        string id, class_;
        string type; // View, Box, ...
        
        // parrent view
        Box *parent;
        
        // own Style
        StyleRule *style;
        
        // list of bonded styles
        list<StyleRule*> styles;
        list<StyleRule*>::iterator stylesIter;
        
       
        // renderer
        Renderer *renderer;
        
        /* get styles and bind ...              \n
         * call after assign id, class ,... !! */
        void getStyle();
        

    private:
        

};

#endif	/* VIEW_H */

