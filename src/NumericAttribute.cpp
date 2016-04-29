/* 
 * File:   FloatAttribute.cpp
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * NUMERIC ATTRIBUTE CLASS
 * saves Numeric style value
 * set, get, onchange
 * ---------------------------------------
 */


#include "NumericAttribute.h"
using namespace std;
using namespace ui;


// ############################################
// -- CREATE OBJEKT --------------
NumericAttribute::NumericAttribute(OnChangeListener *listener, Type type)
 : StyleAttribute(listener, type)
{
    // default
    floatValue = 0.0f;
}


// ## CHANGE VALUE ##########################
// -- SET
void NumericAttribute::set(string value)
{    
    // @TODO set floatAttribute by percentage string
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->floatValue;
    
    // set mode to float
    mode = UI_ATTR__MODE_VALUE;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}

void NumericAttribute::set(float value)
{
    // set floatValue
    this->floatValue = value;
    
    // set mode to float
    mode = UI_ATTR__MODE_VALUE;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}


void NumericAttribute::set(int value)
{
    // set floatValue
    this->floatValue = (float)value;

    // set mode to float
    mode = UI_ATTR__MODE_VALUE;

    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;

    // call onchange callback
    onValueChange();
}

void NumericAttribute::set(double value)
{
    // set floatValue
    this->floatValue = (double)value;

    // set mode to float
    mode = UI_ATTR__MODE_VALUE;

    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;

    // call onchange callback
    onValueChange();
}

void NumericAttribute::setPercent(float value)
{
    percentValue = value;
    
    // set mode to percentage
    mode = UI_ATTR__MODE_PERCENT;
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE;
    
    // call onchange callback
    onValueChange();
}

void NumericAttribute::setMode(bool mode)
{
    this->mode = mode;
}

void NumericAttribute::setAuto(UI_ATTR_AUTO autoMode)
{
    StyleAttribute::setAuto(autoMode);
    mode = UI_ATTR__MODE_VALUE;
}


StyleRule &NumericAttribute::operator()(string value)
{
    set(value);
    return getRule();
}

StyleRule &NumericAttribute::operator()(float value)
{
    set(value);
    return getRule();
}

StyleRule &NumericAttribute::operator()(int value)
{
    set(value);
    return getRule();
}

StyleRule &NumericAttribute::operator()(double value)
{
    set(value);
    return getRule();
}


// -- GET
string NumericAttribute::get()
{
    ostringstream stream;
    stream << floatValue;
    return stream.str();
}

string NumericAttribute::getPercent()
{
    ostringstream stream;
    stream << percentValue;
    return stream.str();
}

float NumericAttribute::getFloat()
{
    return floatValue;
}

int NumericAttribute::getInt()
{
    return (int)floatValue;
}

double NumericAttribute::getDouble()
{
    return (double)floatValue;
}

float NumericAttribute::getPercentFloat()
{
    return percentValue;
}

bool NumericAttribute::getMode()
{
    return mode;
}
