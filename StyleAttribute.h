/* 
 * File:   StyleAttribute.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLEATTRIBUTE CLASS
 * owns value
 * style attribute: left, top, background-color, ...
 * ---------------------------------------
 */


#ifndef STYLEATTRIBUTE_H
#define	STYLEATTRIBUTE_H


//#include "View.h" // => problems with circular including
class View;

#include "Value.h"
#include "IntValue.h"
#include "FloatValue.h"
#include "ColorValue.h"
#include "const.h"

#include <list>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;


// Base of styleAttribute -> down casting
class StyleAttributeBase 
{
    public:

      // -- CONSTAND
      static const bool AKTIVE = true;
      static const bool NON_AKTIVE = false;

      
      /* static */ enum Type 
          {
            _FIRST_TYPE_BEFORE, // used for loops
            HEIGHT, WIDTH,
            LEFT, TOP, RIGHT, BOTTOM,
            BACKGROUND_COLOR, OPACITY,
            TEXT_SIZE, TEXT_COLOR, TEXT_FAMILY,
            _LAST_TYPE_AFTER  // used for loops
          } type;


      // bounded Views -> call calcLayout at change of value
      virtual void addBoundedView(View *view);
      virtual void removeBoundedView(View *view);
          
      // listen for aktive change
      class OnChangeListener 
      { public: virtual void onAktiveStateChange(StyleAttributeBase *styleAttribute) = 0; };
      
      // set if attribute aktive
      virtual bool aktive(bool aktive); // = 0
      virtual bool aktive(); // = 0
      
};
// -- static var
// enum StyleAttributeBase::Type;


template <typename ValueType> class StyleAttribute : public StyleAttributeBase, public Value::OnChangeListener
{
    public:
      StyleAttribute(StyleAttributeBase::OnChangeListener *listener, Type type, initializer_list<int> causeCalc);   
      
      
      // cause calc on bounded views
      int causeCalc[2]; // 1[] -> self, parrent 
      
      // value
      ValueType *value;
      
      
      // bounded Views -> call calcLayout at change of value
      void addBoundedView(View *view);
      void removeBoundedView(View *view);
      
      // set if attribute aktive - before operator =
      bool aktive(bool aktive);
      bool aktive();
      
      
      // set on Change Listener
      void onChange(StyleAttributeBase::OnChangeListener *listener);
      
      
    private:
      // -- OnChangeListener of value -----------------
      void onValueChange ();  // if value changes -> bouded views->needLayout

      
      // -- own on change listener
      StyleAttributeBase::OnChangeListener *onChangeListener;
      
      bool isAktive;
      
      // bounded Views 
      list<View*> boundedViews;
      list<View*>::iterator iterator;


};




#endif	/* STYLEATTRIBUTE_H */





// ############# OLD ########################################
      /*
      // different types
      enum Type 
      {
        left, right, top, bottom,
        height, widht,
        background_color, alpha,         
      };
      
      // information struct for each type
      class TypeDataItem 
      {
      public:
        Type    type;
        string  typeName;
        Value::Type   valueType;
        
        TypeDataItem();
        TypeDataItem(Type type, string typeName, Value::Type valueType);
      };
      
      // Additional information for each type
      static TypeDataItem *typeData[8];
      
      // seach for type
      TypeDataItem* getTypeDataItem(string type);
      TypeDataItem* getTypeDataItem(Type type);
      */