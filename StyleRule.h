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
#include "IntAttribute.h"
#include "ColorAttribute.h"
#include "FloatAttribute.h"
#include "StringAttribute.h"

#include <stdlib.h>
#include <list>
#include <string>
#include <iostream>
using namespace std;


class StyleRule : public StyleAttribute::OnChangeListener
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
      IntAttribute    *height;
      IntAttribute    *width;
      
      IntAttribute    *left;
      IntAttribute    *top;
      IntAttribute    *right;
      IntAttribute    *bottom;
      
      IntAttribute    *position;
      
      ColorAttribute  *backgroundColor;
      FloatAttribute  *opacity;
      
      IntAttribute    *textSize;
      ColorAttribute  *textColor;
      StringAttribute *textFamily;
      

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
      list<View*>::iterator iterator;

};

#endif	/* STYLERULE_H */

