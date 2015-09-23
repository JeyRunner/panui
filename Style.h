/* 
 * File:   Style.h
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLE CLASS
 * owns all STYLERULES
 * 
 * ---------------------------------------
 */


#ifndef STYLE_H
#define	STYLE_H

//class StyleRule;
#include "StyleRule.h"
#include <list>
#include <stdlib.h>
using namespace std;


class Style : public Log
{
    public:
      /* init 
       * @description creates default styleRules*/
      static void init();
        
      StyleRule* operator()(string selector);
      StyleRule* operator()(StyleRule *rule);
        
      static void addRule(StyleRule* rule);             // add Rule to list
      static void removeRule(StyleRule* rule);          // remove Rule from list
      static StyleRule* getRule(string selector);       // search for a view in list
      
      // rules list
      static /*const*/ list<StyleRule*> rules; 
      static /*const*/ list<StyleRule*>::iterator rulesIter;
      
    private:
      

};

// -- init static var
// /*const*/ list<StyleRule*> Style::rules;
// /*const*/ list<StyleRule*>::iterator Style::rulesIter;


#endif	/* STYLE_H */

