/* 
 * File:   SelectAttribute.cpp
 * Author: Joshua Johannson
 *
 * 
 * --------------------------------------
 * SelectAttribute CLASS
 * saves Select style value
 * -> select between different options
 * set, onchange
 * --------------------------------------
 */


#ifndef PANUI_SELECTATTRIBUTE_H
#define PANUI_SELECTATTRIBUTE_H

// include
#include <iostream>
#include <string>
#include <stdlib.h>
#include "Value.h"
#include "StyleAttribute.h"
using namespace std;
namespace ui
{

/* SelectAttribute class
 */
template <typename _type, typename = void>
class SelectAttribute{};

template <typename _type>
class SelectAttribute<_type, typename enable_if<is_enum<_type>::value>::type>  : public StyleAttribute
{
    public:
        SelectAttribute (OnChangeListener *listener, Type type)
                : StyleAttribute(listener, type) {}


        // set value
        void set(_type value)
        {
            this->value = value;
            onValueChange();
        }

        StyleRule& operator()(_type value)
        {
            return getRule();
        }


        // get value
        string get()
        {
            return to_string(value);
        }


        _type value;
};


};     /* END NAMESPACE */
#endif /* PANUI_SELECTATTRIBUTE_H */
