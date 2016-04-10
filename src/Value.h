/* 
 * File:   Value.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * VALUE CLASS
 * saves style value
 * set, onchange
 * ---------------------------------------
 */


#ifndef VALUE_H
#define	VALUE_H

// include
#include <string>
#include <stdlib.h>
#include "Log.h"

using namespace std;
namespace ui
{

/* Ui Class
 */
class Value : public Log
{
    public:
      Value ();
      ~Value ();

      
      // set value
      virtual void set(string value);// = 0;
      //virtual string operator =(string value);
      
      // get value
      virtual string get();// = 0;
      
      
      // onChange callback interface
      class OnChangeListener 
      { public: virtual void onValueChange() = 0; };
      
      // set onchange callback
      void onChange(OnChangeListener *listener);
      
      
      // type
      enum Type 
      { Int, Float, Color };
      
      
    private:
      
    protected:      
      // on change listener -> call boundedViews->needLayout
      OnChangeListener *onChangeListener;
      

};


};     /* END NAMESPACE */
#endif	/* VALUE_H */
