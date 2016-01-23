/* 
 * File:   Text.cpp
 * Author: Joshua Johannson | Pancake
 *
 * 
 *  ---------------------------------------
 * TEXT(VIEW) CLASS
 * extends VIEW
 * render text
 *  ---------------------------------------
 */


#include "Text.h"
#include "Box.h"
#include "View.h"
#include "TextRenderer.h"

using namespace std;



// ###########################################

// -- CREATE OBJEKT --------------
Text::Text() : View(true)
{
    setLogName("TEXT");

    // create renderer
    View::renderer =  new TextRenderer(this);
    
    // -- bind rules and attributes
    getStyle();
}

Text::Text(string id, string class_)  : View(true)
{
    setLogName("TEXT");

    // set var
    this->id = id;
    this->class_ = class_;
    
    // create own renderer
    View::renderer =  new TextRenderer(this);
    
    // -- bind rules and attributes
    getStyle();
}


// -- SET AND GET TEXT ----------------
string Text::text(string text) 
{
    // set var
    this->text_str = text;
    
    // recalc textImg
    renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);
    if (parent && parent != 0 && parent != NULL)
    {
        parent->renderer->addCalcTask(UI_CALCTASK_LAYOUT_CHILDREN_POSITION);
    }
    //((TextRenderer*)renderer)->addCalcTask(UI_CALCTASK_TEXT_TEXT);
    
    return this->text_str;
}

string Text::text() 
{
    return this->text_str;
}






// ###########################################
// -- DESTROY OBJEKT -----------
Text::~Text() {
}

