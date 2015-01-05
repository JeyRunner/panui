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
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
IntAttribute::IntAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc) 
 : StyleAttribute(listener, type, causeCalc)
{
    // default
    floatValue  = 0.0f;
    intValue    = 0;
}


// ## CHANGE VALUE ##########################
// -- SET
void IntAttribute::set(string value) 
{    
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->intValue;
    sstream >> this->floatValue;
    
    // call onchange callback
    onValueChange();
}

void IntAttribute::set(int value) 
{
    // set floatValue intValue
    this->intValue      = value;
    this->floatValue    = (float)value;

    
    // call onchange callback
    onValueChange();
}


// -- GET
string IntAttribute::get()
{
    ostringstream stream;
    stream << intValue;
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

