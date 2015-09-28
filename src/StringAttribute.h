/* 
 * File:   StringAttribute.h
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STRINGATTRIBUTE CLASS
 * saves String style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef STRINGATTRIBUTE_H
#define	STRINGATTRIBUTE_H

#include <sstream>
#include "Value.h"
#include "StyleAttribute.h"
#include <string>
#include <stdlib.h>
using namespace std;


class StringAttribute : public StyleAttribute
{
    public:
      StringAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc);
      ~StringAttribute();
      
      // set value
      void set(string value);
      
      // get value
      string get();
      // float  getFloat();
      
      
      // ! READ ONLY !
      string stringValue;
};

#endif	/* STRINGVALUE_H */

