/* 
 * File:   FloatValue.cpp
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * FLOATVALUE CLASS
 * saves Integer style value
 * set, get, onchange
 * ---------------------------------------
 */


#include "FloatValue.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
FloatValue::FloatValue() 
{
    // default
    floatValue = 0.0f;
}


// ## CHANGE VALUE ##########################
// -- SET
void FloatValue::set(string value) 
{    
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->floatValue;
    
    // call onchange callback
    onChangeListener->onValueChange();
}

void FloatValue::set(float value) 
{
    // set floatValue
    this->floatValue = value;
    
    // call onchange callback
    onChangeListener->onValueChange();
}



// -- GET

string FloatValue::get()
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
FloatValue::~FloatValue() {
}

