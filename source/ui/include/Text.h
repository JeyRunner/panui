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

// include
#include "View.h"
#include "GL.h"
#include <string>
#include <list>
#include <iostream>
namespace ui
{
class TextRenderer;

using namespace std;


/* Text Class
 */
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


};     /* END NAMESPACE */
#endif	/* TEXT_H */

