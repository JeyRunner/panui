/* 
 * File:   Box.cpp
 * Author: Pancake
 * 
 * 
 *  ---------------------------------------
 * BOX(VIEW) CLASS
 * extends VIEW
 * render multible views 
 * -> its children
 *  ---------------------------------------
 */


#include "Box.h"
#include "View.h"
#include "BoxRenderer.h"

using namespace std;



// ###########################################

// -- CREATE OBJEKT --------------
Box::Box() : View(true)
{
    // create renderer
    cout << "[BOX ] create BoxRenderer" << endl;
    View::renderer =  new BoxRenderer(this);
    cout << "[BOX ] create BoxRenderer [OK]" << endl;
    
    // -- bind rules and attributes
    getStyle();
}

Box::Box(string id, string class_)  : View(true)
{
    // set var
    this->id = id;
    this->class_ = class_;
    
    // create own renderer
    cout << "[BOX ] create BoxRenderer" << endl;
    View::renderer =  new BoxRenderer(this);
    
    // -- bind rules and attributes
    getStyle();
}





// ## CHILDREN ###############################

// -- ADD CHILD ------------------
void Box::addChild(View* child)
{
    // add view at list end
    children.push_back(child);
    
    // set self as parrent
    child->parent = this;
}

// -- GET CHILD ------------------
View* Box::getChild(string id)
{
    // search for child
    for(childrenIterator =  children.begin() /* set iterator to start pos */;
        childrenIterator != children.end()   /* end if iterator at last pos */; 
        childrenIterator++)
    {
        // if child is found
        if ((*childrenIterator)->id == id)
        {
            // return child
            return (*childrenIterator);
        }
    }
}

// -- REMOVE CHILD ---------------
void Box::removeChild(string id)
{
    // search for child to remove
    for(childrenIterator =  children.begin() /* set iterator to start pos */;
        childrenIterator != children.end()   /* end if iterator at last pos */; 
        childrenIterator++)
    {
        // if child is found
        if ((*childrenIterator)->id == id)
        {
            // remove child
            children.erase(childrenIterator);
            
            // stop loop
            return;
        }
    }
}

void Box::removeChild(View* child)
{
    // search for child to remove
    for(childrenIterator =  children.begin() /* set iterator to start pos */;
        childrenIterator != children.end()   /* end if iterator at last pos */; 
        childrenIterator++)
    {
        // if child is found
        if ((*childrenIterator) == child)
        {
            // remove child
            children.erase(childrenIterator);
            
            // stop loop
            return;
        }
    }
}



// ###########################################
// -- DESTROY OBJEKT -----------
Box::~Box() {
}

