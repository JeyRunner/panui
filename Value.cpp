/* 
 * File:   Value.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * VALUE CLASS
 * saves style value
 * set, onchange
 * ---------------------------------------
 */


#include "Value.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
Value::Value() 
{
}


// -- ONCHNAGE -------------------
void Value::onChange(OnChangeListener *listener)
{
    this->onChangeListener = listener;
}

// -- VIRUTAL ------------
string Value::get() {}
void Value::set(string value) {}


// ###########################################
// -- DESTROY OBJEKT -----------
Value::~Value() {
}

