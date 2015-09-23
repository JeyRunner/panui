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

#include "const.h"
#include "Log.h"
#include <list>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;


// Base of styleAttribute -> down casting
//class StyleAttributeBase 
//{
//    public:  

//     
//      
//};
// -- static var
// enum StyleAttributeBase::Type;


class StyleAttribute : public Log
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
            PADDING_LEFT, PADDING_TOP, PADDING_RIGHT, PADDING_BOTTOM,
            POSITION,
            BACKGROUND_COLOR, OPACITY,
            TEXT_SIZE, TEXT_COLOR, TEXT_FAMILY,
            _LAST_TYPE_AFTER  // used for loops
          } type;
          
      // listen for aktive change
      class OnChangeListener 
      { public: virtual void onAktiveStateChange(StyleAttribute *styleAttribute) = 0; };
      
      StyleAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc);   
      
      
      // cause calc on bounded views
      int causeCalc[2]; // 1[] -> self, parrent 
      
      // value
      //ValueType *value;
      
      
      // bounded Views -> call calcLayout at change of value
      void addBoundedView(View *view);
      void removeBoundedView(View *view);
      
      // set if attribute aktive - before operator =
      bool aktive(bool aktive);
      bool aktive();
      
      
      
      // set on Change Listener
      void onChange(OnChangeListener *listener);
      
      
      
      // set value
      virtual void set(string value);// = 0;
      
      // set value mode: auto, noneAuto
      virtual void setAuto(UI_ATTR_AUTO autoMode);
      //virtual string operator =(string value);
      
      // get value
      virtual string get();
      
      // -- OnChangeListener of value -----------------
      void onValueChange ();  // if value changes -> bouded views->needLayout
      
      /* auto mode \n
       * READ ONLY */
      UI_ATTR_AUTO autoMode;
      
    private:
      // -- own on change listener
      OnChangeListener *onChangeListener;
      
      bool isAktive;
      
      // bounded Views 
      list<View*> boundedViews;
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