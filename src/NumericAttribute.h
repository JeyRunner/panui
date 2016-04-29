/* 
 * File:   FloatAttribute.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * NUMERICATTRIBUTE CLASS
 * saves Numeric style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef NUMERICATTRIBUTE_H
#define	NUMERICATTRIBUTE_H

// include
#include <sstream>
#include "Value.h"
#include "StyleAttribute.h"
#include <string>
#include <stdlib.h>

using namespace std;
namespace ui
{

/* NumericAttribute Class
 */
class NumericAttribute : public StyleAttribute
{
    public:
      NumericAttribute(OnChangeListener *listener, Type type);
      
       // set value
      void set(string value);
      void set(float  value);
      void set(int    value);
      void set(double value);
      void setPercent(float value);
      void setMode(bool mode); // mode: MODE_INT or MODE_PERCENT
      virtual void setAuto(UI_ATTR_AUTO autoMode);// use relative-percent or absolute value | set with UI_ATTR__MODE_
      StyleRule& operator()(string value);
      StyleRule& operator()(float value);
      StyleRule& operator()(double value);
      StyleRule& operator()(int value);

      // get value
      string get();
      float  getFloat();
      int    getInt();
      double getDouble();
      string getPercent();
      float  getPercentFloat();
      bool   getMode(); // mode: MODE_INT or MODE_PERCENT
      
    private:
      float     floatValue;
      float     percentValue;
      bool      mode;   // mode: MODE_INT or MODE_PERCENT
};


};     /* END NAMESPACE */
#endif	/* NUMERICATTRIBUTE_H */

