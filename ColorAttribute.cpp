/* 
 * File:   ColorAttribute.cpp
 * Author: Pancake
 * 
 * Created on 19. August 2014, 13:45
 */

#include "ColorAttribute.h"
using namespace std;


// ############################################
// -- CREATE OBJEKT --------------
ColorAttribute::ColorAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc) 
 : StyleAttribute(listener, type, causeCalc)
{
    // default
    r = 1.0f;
    b = 1.0f;
    g = 1.0f;
    alpha = 1.0f;
}


// ## CHANGE VALUE ##########################
// -- SET -- BY HEX
void ColorAttribute::set(string value) 
{
        
    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE; 
    
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
    onValueChange();
}

// -- SET SINGEL VALUES --
void ColorAttribute::setR(float red)        { r = red;              onValueChange(); }
void ColorAttribute::setG(float green)      { g = green;            onValueChange(); }
void ColorAttribute::setB(float blue)       { b = blue;             onValueChange(); }
void ColorAttribute::setAlpha(float alpha)  { this->alpha = alpha; onValueChange(); }


// -- GET
string ColorAttribute::get()
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
ColorAttribute::~ColorAttribute() {
}

