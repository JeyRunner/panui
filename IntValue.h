/* 
 * File:   IntValue.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * VALUE CLASS
 * saves Integer style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef INTVALUE_H
#define	INTVALUE_H

#include <sstream>
#include "Value.h"
#include <string>
#include <stdlib.h>
using namespace std;


class IntValue : Value
{
    public:
      IntValue ();
      ~IntValue ();
      
       // set value
      void set(string value);
      void set(int value);
      
      // get value
      string get();
      //class get { int operator()(); };
      
      
      float     floatValue;     // ! READ ONLY !
      int       intValue;       // ! READ ONLY !
      
      
    private:
};

#endif	/* INTVALUE_H */

