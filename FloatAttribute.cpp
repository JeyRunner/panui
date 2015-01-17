/* 
 * File:   FloatAttribute.cpp
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * FLOATATTRIBUTE CLASS
 * saves Integer style value
 * set, get, onchange
 * ---------------------------------------
 */


#include "FloatAttribute.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
FloatAttribute::FloatAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc) 
 : StyleAttribute(listener, type, causeCalc)
{
    // default
    floatValue = 0.0f;
}


// ## CHANGE VALUE ##########################
// -- SET
void FloatAttribute::set(string value) 
{    
    // @TODO set floatAttribute by percentage string
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->floatValue;
    
    // set mode to float
    mode = UI_ATTR__MODE_VALUE;
    
    // call onchange callback
    onValueChange();
}

void FloatAttribute::set(float value) 
{
    // set floatValue
    this->floatValue = value;
    
    // set mode to float
    mode = UI_ATTR__MODE_VALUE;
    
    // call onchange callback
    onValueChange();
}

void FloatAttribute::setPercent(float value) 
{
    percentValue = value;
    
    // set mode to percentage
    mode = UI_ATTR__MODE_PERCENT;
    
    // call onchange callback
    onValueChange();
}



// -- GET

string FloatAttribute::get()
{
    ostringstream stream;
    stream << floatValue;
    return stream.str();
}

string FloatAttribute::getPercent() 
{
    ostringstream stream;
    stream << percentValue;
    return stream.str();
}

/*
float FloatValue::get()
{
    return floatValue;
}*/



// ###########################################
// -- DESTROY OBJEKT -----------
FloatAttribute::~FloatAttribute() {
}

