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
using namespace ui;

// -- init static var
/*const*/ list<StyleRule*> Style::rules;
/*const*/ list<StyleRule*>::iterator Style::rulesIter;


// ############################################
// -- INIT ---------------------------------------------
void Style::init() 
{
    // default styleRules
    StyleRule *rBase = new StyleRule("*");
    
    rBase->top->set(0);
    rBase->right->set(0);
    rBase->left->set(0);
    rBase->bottom->set(0);
    rBase->position->set(UI_ATTR_POSITION_RELATIVE);

    rBase->scrollX->set(0);
    rBase->scrollY->set(0);

    rBase->paddingTop->set(0);
    rBase->paddingBottom->set(0);
    rBase->paddingLeft->set(0);
    rBase->paddingRight->set(0);
    
    rBase->height->setAuto(UI_ATTR_AUTO_AUTO);
    rBase->width->setAuto(UI_ATTR_AUTO_AUTO);
    rBase->opacity->set(0.0f);    
    
    rBase->backgroundColor->setR(1);
    rBase->backgroundColor->setG(1);
    rBase->backgroundColor->setB(1);
    rBase->backgroundColor->setAlpha(0);

    rBase->overflow->set(UI_ATTR_OVERFLOW_HIDDEN);
    
    rBase->textSize->set(25);
    rBase->textColor->set("000000FF");
    rBase->textFamily->set("/usr/share/fonts/truetype/freefont/FreeSans.ttf");
    
    addRule(rBase);
}



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

