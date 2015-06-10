/* 
 * File:   IntAttribute.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * INTATTRIBUTE CLASS
 * saves Integer style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef INTATTRIBUTE_H
#define	INTATTRIBUTE_H

#include <sstream>
#include "Value.h"
#include "StyleAttribute.h"
#include <string>
#include <stdlib.h>
using namespace std;


class IntAttribute : public StyleAttribute
{
    public:
      IntAttribute (OnChangeListener *listener, Type type, initializer_list<int> causeCalc);
      ~IntAttribute ();
      
       // set value
      void set(string value);
      void set(int value);
      void setPercent(float value);
      virtual void setAuto(UI_ATTR_AUTO autoMode);
      
      // get value
      string get();
      string getPercent();
      //class get { int operator()(); };
      
      
      float     floatValue;     // ! READ ONLY !
      float     percentValue;   // ! READ ONLY !
      int       intValue;        // ! READ ONLY !
      
      // mode: MODE_INT or MODE_PERCENT
      bool mode;
      
    private:
};

#endif	/* INTVALUE_H */

