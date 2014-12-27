/* 
 * File:   FloatValue.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * FLOATVALUE CLASS
 * saves Float style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef FLOATVALUE_H
#define	FLOATVALUE_H

#include <sstream>
#include "Value.h"
#include <string>
#include <stdlib.h>
using namespace std;


class FloatValue : Value
{
    public:
      FloatValue ();
      ~FloatValue ();
      
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

