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
#include "Box.h"
//#include "StyleRule.h"

// -- init static var
/*const*/ list<StyleRule*> Style::rules;
/*const*/ list<StyleRule*>::iterator Style::rulesIter;


// ############################################
// -- CREATE OBJEKT --------------
// Style::Style() {}


// -- FUNCTION OBJECT ----------------------------------    
StyleRule* Style::operator()(string selector) 
{
    StyleRule *rule = getRule(selector);
    
    // new rule
    if (rule == NULL)
        rule = new StyleRule(selector);
    
    return rule;
}

StyleRule* Style::operator()(StyleRule *rule)
{
    // add rule
    addRule(rule);
}

          
          
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

