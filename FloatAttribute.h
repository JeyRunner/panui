/* 
 * File:   FloatAttribute.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * FLOATATTRIBUTE CLASS
 * saves Float style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef FLOATATTRIBUTE_H
#define	FLOATATTRIBUTE_H

#include <sstream>
#include "Value.h"
#include "StyleAttribute.h"
#include <string>
#include <stdlib.h>
using namespace std;


class FloatAttribute : public StyleAttribute
{
    public:
      FloatAttribute (OnChangeListener *listener, Type type, initializer_list<int> causeCalc);
      ~FloatAttribute ();
      
       // set value
      void set(string value);
      void set(float value);
      
      // get value
      string get();
      // float  getFloat();
      
      
      // ! READ ONLY !
      float     floatValue;
      
      
    private:
};

#endif	/* FLOATVALUE_H */

