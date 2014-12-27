/* 
 * File:   StringValue.cpp
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STRINGVALUE CLASS
 * saves String style value
 * set, onchange
 * ---------------------------------------
 */


#include "StringValue.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
StringValue::StringValue() 
{
    // default
    stringValue = "";
}


// ## CHANGE VALUE ##########################
// -- SET
void StringValue::set(string value) 
{    
    stringValue = value;
    
    // call onchange callback
    onChangeListener->onValueChange();
}



// -- GET

string StringValue::get()
{
    return stringValue;
}


// ###########################################
// -- DESTROY OBJEKT -----------
StringValue::~StringValue() {
}

