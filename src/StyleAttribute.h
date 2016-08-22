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

// include
#include "const.h"
#include "Log.h"
#include <list>
#include <string>
#include <iostream>
#include <stdlib.h>
namespace ui
{
class View;
class StyleRule;

using namespace std;


/* StyleAttribute Class
 */
class StyleAttribute : protected Log
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
            SCROLL_X, SCROLL_Y,
            PADDING_LEFT, PADDING_TOP, PADDING_RIGHT, PADDING_BOTTOM,
            POSITION,
            BACKGROUND_COLOR, OPACITY, OVERFLOW_CUT,
            TEXT_SIZE, TEXT_COLOR, TEXT_FAMILY,
            _LAST_TYPE_AFTER  // used for loops
          } type;
          
      // listen for aktive change
      class OnChangeListener 
      { public: virtual void onAktiveStateChange(StyleAttribute *styleAttribute) = 0; };
      
      StyleAttribute(OnChangeListener *listener, Type type);
      

      // cause calc on bounded views
      struct CauseCalcElement {
        CauseCalcElement(Type _type, int _calc_self, int _calc_parent)
        {type = _type; causeCalc[0] = _calc_self;  causeCalc[1] = _calc_parent;}
        Type type;
        int causeCalc[2];
      };
      CauseCalcElement *getCauseCalcForType(Type type);
      static CauseCalcElement *causeCalcList[_LAST_TYPE_AFTER];
      CauseCalcElement *causeCalc; // 1[] -> self, parent


      // bounded Views -> call calcLayout at change of value
      void addBoundedView(View *view);
      void removeBoundedView(View *view);
      
      // set if attribute aktive - before operator =
      bool aktive(bool aktive);
      bool aktive();
      

      // set on Change Listener
      void onChange(OnChangeListener *listener);
      
      // get style rule
      StyleRule& getRule();

      // set value
      virtual void set(string value);// = 0;
      virtual StyleRule& operator()(string value);
      
      // set value mode: auto, noneAuto
      virtual void setAuto(UI_ATTR_AUTO autoMode);
      UI_ATTR_AUTO getAuto();
      
      // get value
      virtual string get();
      
      // -- OnChangeListener of value -----------------
      void onValueChange ();  // if value changes -> bouded views->needLayout
      

    private:
      // -- own on change listener
      OnChangeListener *onChangeListener;

      bool isAktive;
      
      // bounded Views 
      list<View*> boundedViews;


    protected:
      /* auto mode \n
         * READ ONLY */
      UI_ATTR_AUTO autoMode;
};


};     /* END NAMESPACE */
#endif	/* STYLEATTRIBUTE_H */