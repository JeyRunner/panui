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
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->floatValue;
    
    // call onchange callback
    onValueChange();
}

void FloatAttribute::set(float value) 
{
    // set floatValue
    this->floatValue = value;
    
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
/*
float FloatValue::get()
{
    return floatValue;
}*/



// ###########################################
// -- DESTROY OBJEKT -----------
FloatAttribute::~FloatAttribute() {
}

