/* 
 * File:   BoxView.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 *  ---------------------------------------
 * BOX(VIEW) CLASS
 * extends VIEW
 * render multible views 
 * -> its children
 *  ---------------------------------------
 */


#ifndef BOX_H
#define	BOX_H

// include
#include "View.h"
#include "GL.h"
#include <string>
#include <list>
#include <iostream>
namespace ui
{
class BoxRenderer;

using namespace std;


/* Box Class
 */
class Box : public View
{
    public:
        Box();
        Box(string id, string class_);;
        ~Box();
        
        
        // children list
        list<View*> children;
        list<View*>::iterator childrenIterator; // pos pointer 
   
        
        
        void addChild(View* child);     // add new child to children
        View* getChild(string id);      // get child by id
        void removeChild(string id);    // remove child by id
        void removeChild(View* child);  // remove child by reference
        
        // renderer
        // BoxRenderer *renderer;
        
    private:
        

};


};     /* END NAMESPACE */
#endif	/* BOXVIEW_H */

