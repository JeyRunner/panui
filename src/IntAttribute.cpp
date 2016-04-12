/* 
 * File:   IntAttribute.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * INTATTRIBUTE CLASS
 * saves Integer style value
 * set, get, onchange
 * ---------------------------------------
 */


#include "IntAttribute.h"
using namespace ui;


// ############################################
// -- CREATE OBJEKT --------------
IntAttribute::IntAttribute(OnChangeListener *listener, Type type)
 : StyleAttribute(listener, type)
{
    // default
    floatValue  = 0.0f;
    intValue    = 0;
}


// ## CHANGE VALUE ##########################
// -- SET
void IntAttribute::set(string value) 
{    
    // @TODO set intAttribute by percentage string
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->intValue;
    sstream >> this->floatValue;
    
    // set mode to int
    mode = UI_ATTR__MODE_VALUE;
    
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}

void IntAttribute::set(int value) 
{    
    // set floatValue intValue
    this->intValue      = value;
    this->floatValue    = (float)value;

    // set mode to int
    mode = UI_ATTR__MODE_VALUE;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}

void IntAttribute::setPercent(float value) 
{
    percentValue = value;
    
    // set mode to percentage
    mode = UI_ATTR__MODE_PERCENT;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}

void IntAttribute::setAuto(UI_ATTR_AUTO autoMode)
{
    StyleAttribute::setAuto(autoMode);
    mode = UI_ATTR__MODE_VALUE;
}

StyleRule &IntAttribute::operator()(string value)
{
    set(value);
    return getRule();
}

StyleRule &IntAttribute::operator()(int value)
{
    set(value);
    return getRule();
}


// -- GET
string IntAttribute::get()
{
    ostringstream stream;
    stream << intValue;
    return stream.str();
}

string IntAttribute::getPercent() 
{
    ostringstream stream;
    stream << percentValue;
    return stream.str();
}


/*
int IntValue::get::operator()()
{
    //return IntValue;
}
*/



// ###########################################
// -- DESTROY OBJEKT -----------
IntAttribute::~IntAttribute() {
}

