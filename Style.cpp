/* 
 * File:   Style.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLE CLASS
 * owns all STYLERULES
 * 
 * ---------------------------------------
 */



#include "Style.h"
//#include "StyleRule.h"

// -- init static var
/*const*/ list<StyleRule*> Style::rules;
/*const*/ list<StyleRule*>::iterator Style::rulesIter;


// ############################################
// -- CREATE OBJEKT --------------
// Style::Style() {}



// -- MANGAGE STYLE RULES ------------------------------
// -- ADD -----
void Style::addRule(StyleRule* rule) 
{
    rules.push_back(rule);
}

// -- REMOVE --
void Style::removeRule(StyleRule* rule) 
{
    rules.remove(rule);
}

// -- GET -----
StyleRule* Style::getRule(string selector) 
{
    // -- search for rule
    for (rulesIter = rules.begin(); rulesIter != rules.end(); rulesIter++)
    {
        // get item
        StyleRule *item = *rulesIter;
        
        // -- is found
        if (item->selector == selector)
        {
            // return
            return item;
        } 
    }
    
    // -- nothing found
    return NULL; 
}

