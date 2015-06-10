/* 
 * File:   Text.h
 * Author: Joshua Johannson | Pancake
 *
 * 
 *  ---------------------------------------
 * TEXT(VIEW) CLASS
 * extends VIEW
 * render text
 *  ---------------------------------------
 */


#ifndef TEXT_H
#define	TEXT_H

#include "View.h"
//#include "GLES/gl.h"
#include "GL.h"
class TextRenderer;

#include <string>
#include <list>
#include <iostream>
using namespace std;



class Text : public View
{
    public:
        Text ();
        Text(string id, string class_);;
        ~Text();
        
        
        string text(string text); // set and get text 
        string text();            // set and get text 
       
        string text_str;          // read only ! 
    private:
};

#endif	/* TEXT_H */

