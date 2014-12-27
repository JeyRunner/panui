/* 
 * File:   StyleRule.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLERULE CLASS
 * owns multible STYLEATTRIBUTES
 * 
 * ---------------------------------------
 */


#ifndef STYLERULE_H
#define	STYLERULE_H


#include "StyleAttribute.h"
#include "IntValue.h"
#include "ColorValue.h"
#include "FloatValue.h"
#include "StringValue.h"

#include <stdlib.h>
#include <list>
#include <string>
#include <iostream>
using namespace std;


class StyleRule : public StyleAttributeBase::OnChangeListener
{
    public:
      StyleRule (string selector);
      ~StyleRule ();

      
      // -----------------------------------------
      string       selector;   // selector    
      
      // selector type - view, id, ...
      enum SelectorType
      { ID_SELECTOR, CLASS_SELECTOR, VIEW_SELECTOR } selectorType;  
      
      /* how important is styleRule compared to others
       * has ID_SELECTOR     => 100 
       * has CLASS_SELECTOR  => 20
       * has VIEW_SELECTOR   => 10  
       */
      int  importance;        
      
      
      
      // -- Style Attributes ----------------------
      StyleAttribute<IntValue>    *heigh;
      StyleAttribute<IntValue>    *width;
      
      StyleAttribute<IntValue>    *left;
      StyleAttribute<IntValue>    *top;
      StyleAttribute<IntValue>    *right;
      StyleAttribute<IntValue>    *bottom;
      
      StyleAttribute<ColorValue>  *backgroundColor;
      StyleAttribute<FloatValue>  *opacity;
      
      StyleAttribute<IntValue>    *textSize;
      StyleAttribute<ColorValue>  *textColor;
      StyleAttribute<StringValue> *textFamily;
      

      // get Attribute by StyleAttributeType
      StyleAttributeBase* getAttribute(StyleAttributeBase::Type type);
      
      
      // bounded Views -> call calcLayout at change of value
      void addBoundedView(View *view);
      void removeBoundedView(View *view);
      
      
      // compare two StyleRules -> by importance
      bool operator <(StyleRule *otherStyleRule);
      bool operator >(StyleRule *otherStyleRule);
      static bool compareRules(StyleRule *a, StyleRule *b);
      
      
    private:
      // -- on change listener of attributes --
      virtual void onAktiveStateChange (StyleAttributeBase *styleAttributeBase);
      
      
      void setImportance(SelectorType selectorType);
      void setSelectorType(string selector);
      
      // bounded Views 
      list<View*> boundedViews;
      list<View*>::iterator iterator;

};

#endif	/* STYLERULE_H */

