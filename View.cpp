/* 
 * File:   View.cpp
 * Author: Pancake
 *
 *  
 *  ---------------------------------------
 * VIEW CLASS
 * basic ui element
 * render itself
 *  ---------------------------------------
 */


#include "View.h"
#include "Renderer.h"
#include "Style.h"
using namespace std;


// -- CREATE OBJEKT --------------
View::View(bool DoNothing) 
{
    // create own style rule
    style = new StyleRule("~");
    
    cout << "[VIEW] do nothing" << endl; 
}

View::View() 
{
    // create own style rule
    style = new StyleRule("~");
    
    // create own renderer
    renderer = new Renderer(this);
    
    // -- bind rules and attributes
    getStyle();
}
View::View(string id, string class_) 
{
    // set var
    this->id = id;
    this->class_ = class_;
    
    // create own style rule
    style = new StyleRule("~");
    
    // create own renderer
    renderer = new Renderer(this);
    
    // -- bind rules and attributes
    getStyle();
}


// -- GET STYLE -------------------
void View::getStyle() 
{
    // -- bind rules and attributes
    renderer->bindRuleAutomatic();
    renderer->bindAttributeAutomatic();
}


// -- SET EVENTS ---------------------------
void View::onTouchDown(     function<void(View*, Point, Point, Point) > onTouchDownFunc)     { this->onTouchDownFunc = onTouchDownFunc; }
void View::onTouchEnter(    function<void(View*, Point, Point, Point) > onTouchEnterFunc)    { this->onTouchEnterFunc = onTouchEnterFunc; }
void View::onTouchLeave(    function<void(View*, Point, Point, Point) > onTouchLeaveFunc)    { this->onTouchLeaveFunc = onTouchLeaveFunc; }
void View::onTouchMove(     function<void(View*, Point, Point, Point) > onTouchMoveFunc)     { this->onTouchMoveFunc = onTouchMoveFunc; }
void View::onTouchUp(       function<void(View*, Point, Point, Point) > onTouchUpFunc)       { this->onTouchUpFunc = onTouchUpFunc;}
void View::onTouchDrag(     function<void(View*, Point, Point, Point) > onTouchDragFunc)     { this->onTouchDragFunc = onTouchDragFunc;}
void View::onTouchDragMove( function<void(View*, Point, Point, Point) > onTouchDragMoveFunc) { this->onTouchDragMoveFunc = onTouchDragMoveFunc;}
void View::onTouchDrop(     function<void(View*, Point, Point, Point) > onTouchDropFunc)     { this->onTouchDropFunc = onTouchDropFunc;}



// ###########################################
// -- DESTROY OBJEKT -----------
View::~View() 
{
    
}

