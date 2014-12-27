/* 
 * File:   ColorValue.cpp
 * Author: Pancake
 * 
 * Created on 19. August 2014, 13:45
 */

#include "ColorValue.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
ColorValue::ColorValue() 
{
    // default
    r = 1.0f;
    b = 1.0f;
    g = 1.0f;
    alpha = 1.0f;
}


// ## CHANGE VALUE ##########################
// -- SET -- BY HEX
void ColorValue::set(string value) 
{
    // if string is to short for r g b ---
    if (value.length() < 6)
    { return; }
    
    
    // else ---------------------    
    // convert to rgb
    char str1[2] = {value.at(0), value.at(1)};
    char str2[2] = {value.at(2), value.at(3)};
    char str3[2] = {value.at(4), value.at(5)};
    
    r = (float)(strtol(str1, NULL, 16)) / 255;
    g = (float)(strtol(str2, NULL, 16)) / 255;
    b = (float)(strtol(str3, NULL, 16)) / 255;
    
    
    // if there is alpha too --------
    if (value.length() >= 8)
    {
        // convert alpha
        char str1[2] = {value.at(6), value.at(7)};
        alpha = (float)(strtol(str1, NULL, 16)) / 255;
    }
    
    // call onchange callback
    onChangeListener->onValueChange();
}

// -- SET SINGEL VALUES --
void ColorValue::setR(float red)        { r = red;              onChangeListener->onValueChange(); }
void ColorValue::setG(float green)      { g = green;            onChangeListener->onValueChange(); }
void ColorValue::setB(float blue)       { b = blue;             onChangeListener->onValueChange(); }
void ColorValue::setAlpha(float alpha)  { this->alpha = alpha;  onChangeListener->onValueChange(); }


// -- GET
string ColorValue::get()
{
    // convert R G B to string hex
    ostringstream stream;
    stream << hex << (int)(r*255)
           << hex << (int)(g*255)
           << hex << (int)(b*255)
           << hex << (int)(alpha*255);
    
    return stream.str();
}




// ###########################################
// -- DESTROY OBJEKT -----------
ColorValue::~ColorValue() {
}

