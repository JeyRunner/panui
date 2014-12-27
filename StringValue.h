/* 
 * File:   StringValue.h
* Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STRINGVALUE CLASS
 * saves String style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef STRINGVALUE_H
#define	STRINGVALUE_H

#include <sstream>
#include "Value.h"
#include <string>
#include <stdlib.h>
using namespace std;


class StringValue : public Value
{
    public:
      StringValue ();
      ~StringValue ();
      
      // set value
      void set(string value);
      
      // get value
      string get();
      // float  getFloat();
      
      
      // ! READ ONLY !
      string stringValue;
};

#endif	/* STRINGVALUE_H */

