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
    // start pos
    int offset = 0;

    // set auto to none
    autoMode = UI_ATTR_AUTO_NONE; 
    
    // if string is to short for r g b ---
    if (value.length() < 6)
    { return; }

    // remove '#'
    if (value.substr(0,1) == "#")
    { offset = 1;}

    
    // else ---------------------    
    // convert to rgb
    r = (float)(strtol(value.substr(offset + 0,2).c_str(), NULL, 16)) / 255;
    g = (float)(strtol(value.substr(offset + 2,2).c_str(), NULL, 16)) / 255;
    b = (float)(strtol(value.substr(offset + 4,2).c_str(), NULL, 16)) / 255;


    // if there is alpha too --------
    if (value.length() >= 8)
    {
        // convert alpha
        alpha = (float)(strtol(value.substr(offset + 6,2).c_str(), NULL, 16)) / 255;
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

string ColorAttribute::getR()        {ostringstream stream; stream << r; return stream.str();}
string ColorAttribute::getG()        {ostringstream stream; stream << g; return stream.str();}
string ColorAttribute::getB()        {ostringstream stream; stream << b; return stream.str();}
string ColorAttribute::getAlpha()    {ostringstream stream; stream << alpha; return stream.str();}


// ###########################################
// -- DESTROY OBJEKT -----------
ColorAttribute::~ColorAttribute() {
}

