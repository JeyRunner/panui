/* 
 * File:   IntValue.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * INTVALUE CLASS
 * saves Integer style value
 * set, get, onchange
 * ---------------------------------------
 */


#include "IntValue.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
IntValue::IntValue() 
{
    // default
    floatValue  = 0.0f;
    intValue    = 0;
}


// ## CHANGE VALUE ##########################
// -- SET
void IntValue::set(string value) 
{    
    // convert string to int and float
    stringstream sstream(value);
    sstream >> this->intValue;
    sstream >> this->floatValue;
    
    // call onchange callback
   onChangeListener->onValueChange();
}

void IntValue::set(int value) 
{
    // set floatValue intValue
    this->intValue      = value;
    this->floatValue    = (float)value;

    
    // call onchange callback
    onChangeListener->onValueChange();
}


// -- GET
string IntValue::get()
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
IntValue::~IntValue() {
}

