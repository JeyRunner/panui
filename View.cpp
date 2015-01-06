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



// ###########################################
// -- DESTROY OBJEKT -----------
View::~View() 
{
    
}

