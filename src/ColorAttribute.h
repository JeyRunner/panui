/* 
 * File:   ColorAttribute.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * COLORATTRIBUTE CLASS
 * saves color style value
 * set, get, onchange
 * ---------------------------------------
 */


#ifndef COLORATTRIBUTE_H
#define	COLORATTRIBUTE_H

// include
#include "Value.h"
#include "StyleAttribute.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
namespace ui
{

/* ColorAttribute Class
 */
class ColorAttribute : public StyleAttribute
{
    public:
      ColorAttribute (OnChangeListener *listener, Type type);
      ~ColorAttribute ();
      
      
      void set(string value);   // set by hex: R G B Alpha
      void setR(float red);     // set red
      void setG(float green);   // set green
      void setB(float blue);    // set blue
      void setAlpha(float alpha); // set alpha
      
      // get value
      string get();             // get as hex: R G B Alpha
      string getR();             // get red
      string getG();             // get green
      string getB();             // get blue
      string getAlpha();         // get alpha
      
      
      float r;          // ! READ ONLY !
      float g;          // ! READ ONLY !
      float b;          // ! READ ONLY !
      float alpha;      // ! READ ONLY !
      
      
    private:
      // string    stringValue;
};


};     /* END NAMESPACE */
#endif	/* COLORVALUE_H */

