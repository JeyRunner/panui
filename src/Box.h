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

//class View;
#include "View.h"

//#include "GLES/gl.h"
#include "GL.h"

class BoxRenderer;
//class BoxRenderer;

#include <string>
#include <list>
#include <iostream>
using namespace std;



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

#endif	/* BOXVIEW_H */

