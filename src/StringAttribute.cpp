/* 
 * File:   StringAttribute.cpp
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STRINGATTRIBUTE CLASS
 * saves String style value
 * set, onchange
 * ---------------------------------------
 */


#include "StringAttribute.h"
using namespace std;
using namespace ui;


// ############################################
// -- CREATE OBJEKT --------------
StringAttribute::StringAttribute(OnChangeListener *listener, Type type)
 : StyleAttribute(listener, type)
{
    // default
    stringValue = "";
}


// ## CHANGE VALUE ##########################
// -- SET
void StringAttribute::set(string value) 
{    
    stringValue = value;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}



// -- GET

string StringAttribute::get()
{
    return stringValue;
}


// ###########################################
// -- DESTROY OBJEKT -----------
StringAttribute::~StringAttribute() {
}

