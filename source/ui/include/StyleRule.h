/* 
 * File:   StyleRule.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLERULE CLASS
 * owns multiple STYLEATTRIBUTES
 * 
 * ---------------------------------------
 */


#ifndef STYLERULE_H
#define	STYLERULE_H

// include
#include "StyleAttribute.h"
#include "ColorAttribute.h"
#include "NumericAttribute.h"
#include "StringAttribute.h"
#include "SelectAttribute.h"
#include <stdlib.h>
#include <list>
#include <string>
#include <iostream>

using namespace std;
namespace ui
{

/* StyleRule Class
 */
class StyleRule : public StyleAttribute::OnChangeListener, protected Log
{
    public:
      StyleRule (string selector);
      ~StyleRule ();

      
      // -----------------------------------------
      string       selector;   // selector    
      
      // selector type - view, id, ...
      enum SelectorType
      { OWN_SELECTOR, ID_SELECTOR, CLASS_SELECTOR, VIEW_SELECTOR, GLOBAL_SELECTOR } selectorType;  
      
      /* how important is styleRule compared to others
       * has OWN_SELCTOR     => 100 
       * has ID_SELECTOR     => 100 
       * has CLASS_SELECTOR  => 20
       * has VIEW_SELECTOR   => 10  
       * has GLOBAL_SELECTOR => 1
       */
      int  importance;        
      

      
      // -- Style Attributes ----------------------
      NumericAttribute    height;
      NumericAttribute    width;

      NumericAttribute    left;
      NumericAttribute    top;
      NumericAttribute    right;
      NumericAttribute    bottom;

      NumericAttribute    scrollX;
      NumericAttribute    scrollY;

      NumericAttribute    paddingLeft;
      NumericAttribute    paddingTop;
      NumericAttribute    paddingRight;
      NumericAttribute    paddingBottom;
      
      SelectAttribute<UI_ATTR_POSITION_> position;

      ColorAttribute      backgroundColor;
      NumericAttribute    opacity;
      SelectAttribute<UI_ATTR_OVERFLOW_> overflow;

      NumericAttribute    textSize;
      ColorAttribute      textColor;
      StringAttribute     textFamily;
      

      // get Attribute by StyleAttributeType
      StyleAttribute* getAttribute(StyleAttribute::Type type);
      
      
      // bounded Views -> call calcLayout at change of value
      void addBoundedView(View *view);
      void removeBoundedView(View *view);
      
      
      // compare two StyleRules -> by importance
      bool operator <(StyleRule *otherStyleRule);
      bool operator >(StyleRule *otherStyleRule);
      static bool compareRules(StyleRule *a, StyleRule *b);
      
      
    private:
      // -- on change listener of attributes --
      virtual void onAktiveStateChange (StyleAttribute *styleAttribute);
      
      
      void setImportance(SelectorType selectorType);
      void setSelectorType(string selector);
      
      // bounded Views 
      list<View*> boundedViews;
};


};     /* END NAMESPACE */
#endif	/* STYLERULE_H */

