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
#include "View.h"
#include "TextRenderer.h"

using namespace std;



// ###########################################

// -- CREATE OBJEKT --------------
Text::Text() : View(true)
{
    // create renderer
    View::renderer =  new TextRenderer(this);
}

Text::Text(string id, string class_)  : View(true)
{
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
    ((TextRenderer*)renderer)->calcText();
    
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

