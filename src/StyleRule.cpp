/* 
 * File:   StyleRule.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLERULE CLASS
 * owns multible STYLEATTRIBUTES
 * 
 * ---------------------------------------
 */


#include <string.h>
#include "StyleAttribute.h"

#include "StyleRule.h"
#include "View.h"
#include "Renderer.h"
#include "Style.h"
using namespace ui;


// ############################################
// -- CREATE OBJEKT --------------
StyleRule::StyleRule(string selector) :
// -- create StyleAttribute objects
height             (this,   StyleAttribute::HEIGHT),
width              (this,   StyleAttribute::WIDTH),
left               (this,   StyleAttribute::LEFT),
top                (this,   StyleAttribute::TOP),
right              (this,   StyleAttribute::RIGHT),
bottom             (this,   StyleAttribute::BOTTOM),
position           (this,   StyleAttribute::POSITION),
scrollX            (this,   StyleAttribute::SCROLL_X),
scrollY            (this,   StyleAttribute::SCROLL_Y),

paddingLeft        (this,   StyleAttribute::PADDING_LEFT),
paddingRight       (this,   StyleAttribute::PADDING_RIGHT),
paddingTop         (this,   StyleAttribute::PADDING_TOP),
paddingBottom      (this,   StyleAttribute::PADDING_BOTTOM),

backgroundColor    (this,   StyleAttribute::BACKGROUND_COLOR),
opacity            (this,   StyleAttribute::OPACITY),
overflow           (this,   StyleAttribute::OVERFLOW_CUT),

textSize           (this,   StyleAttribute::TEXT_SIZE),
textColor          (this,   StyleAttribute::TEXT_COLOR),
textFamily         (this,   StyleAttribute::TEXT_FAMILY)


{
    setLogName("RULE");
    
    // -- set var
    this->selector = selector;
    
    // -- get Selector, Type
    setSelectorType(selector);
    setImportance(this->selectorType);


    // add self to style list
    Style::addRule(this);
}


// -- SET SELECTOR TYPE BY SELECTOR
void StyleRule::setSelectorType(string selector)
{
    // have to 
    char selectorTypeStr = selector[0]; 
    // cout << "get selectorType:   selectorTypeStr = " << selectorTypeStr << endl
    //      << "       equals to . = " << (selectorTypeStr == '.') << endl;
    
    // -- switch
    if      (selectorTypeStr == '~') { selectorType = OWN_SELECTOR; }
    else if (selectorTypeStr == '#') { selectorType = ID_SELECTOR; }
    else if (selectorTypeStr == '.') { selectorType = CLASS_SELECTOR; }
    else if (selectorTypeStr == '*') { selectorType = GLOBAL_SELECTOR; }
    else                             { selectorType = VIEW_SELECTOR; }
}


// -- SET IMPORTANCE BY SELECTOR TYPE
void StyleRule::setImportance(SelectorType selectorType)
{
    switch (selectorType)
    {
        case OWN_SELECTOR:    importance = 200; break;
        case ID_SELECTOR:     importance = 100; break;
        case CLASS_SELECTOR:  importance = 20;  break;
        case VIEW_SELECTOR:   importance = 10;  break;
        case GLOBAL_SELECTOR: importance = 1;   break;
        default:              importance = 0;   break;
    }
    trace("add rule '" + str(selector) + "', has importance " + str(importance));
}


// -- GET STYLE ATTRIBUTE ----------
StyleAttribute* StyleRule::getAttribute(StyleAttribute::Type type)
{
    switch (type)
    {
        case StyleAttribute::HEIGHT:             return &height;
        case StyleAttribute::WIDTH:              return &width;
        case StyleAttribute::LEFT:               return &left;
        case StyleAttribute::TOP:                return &top;
        case StyleAttribute::RIGHT:              return &right;
        case StyleAttribute::BOTTOM:             return &bottom;
        case StyleAttribute::SCROLL_X:           return &scrollX;
        case StyleAttribute::SCROLL_Y:           return &scrollY;
        case StyleAttribute::PADDING_LEFT:       return &paddingLeft;
        case StyleAttribute::PADDING_TOP:        return &paddingTop;
        case StyleAttribute::PADDING_RIGHT:      return &paddingRight;
        case StyleAttribute::PADDING_BOTTOM:     return &paddingBottom;
        case StyleAttribute::POSITION:           return &position;
        case StyleAttribute::BACKGROUND_COLOR:   return &backgroundColor;
        case StyleAttribute::OPACITY:            return &opacity;
        case StyleAttribute::OVERFLOW_CUT:       return &overflow;
        case StyleAttribute::TEXT_SIZE:          return &textSize;
        case StyleAttribute::TEXT_COLOR:         return &textColor;
        case StyleAttribute::TEXT_FAMILY:        return &textFamily;
        
        default:                                 warn("getAttribute [type does not fit] ->default"); return NULL;
    }    
}



// -- BOUNDED VIEWS -------------------------------------------------------------------
// -- ADD
void StyleRule::addBoundedView(View *view)
{
    // add view to end of list
    boundedViews.push_back(view);
}

// -- REMOVE
void StyleRule::removeBoundedView(View *view)
{
    // -- search for View in list 
    for (list<View*>::iterator it = boundedViews.begin(); /* iterator to start pos */
         it != boundedViews.end();  /* end if iterator at last pos */
         it++)
    {
        if (*it == view)
        {
            //  found View
            //  => remove
            boundedViews.erase(it);
            return;
     }}
}




// -- COMPARE TWO STYLE RULES 
bool StyleRule::operator   <(StyleRule *otherStyleRule)
{ return (this->importance > otherStyleRule->importance); }

bool StyleRule::operator   >(StyleRule *otherStyleRule)
{ return (this->importance < otherStyleRule->importance);
}

bool StyleRule::compareRules(StyleRule* a, StyleRule* b) 
{
    return (a->importance > b->importance);
}



// -- IF STYLEATTRIBUTE AKTIVE_STATE CHANGES ---
void StyleRule::onAktiveStateChange(StyleAttribute *styleAttribute) 
{
    // bounded views -> each
    for (list<View*>::iterator it = boundedViews.begin(); /* iterator to start pos */
         it != boundedViews.end();  /* end if iterator at last pos */
         it++)
    {
        // @TODO unbind views of unused Attributes form unused Attributes @FOR_BETTER_PERFORMANCE 
         
        // get Attribute that fits to view
        StyleAttribute *calculatedAttribute = (*it)->renderer->checkAttributeBinding(styleAttribute->type);
        
        // bind new fitting Attribute
       (*it)->renderer->bindAttribute(calculatedAttribute);
       
       trace("reset Attribute type '" + str(styleAttribute->type) + "'  ...");
    }
}




// ###########################################
// -- DESTROY OBJEKT -----------
StyleRule::~StyleRule()
{
    
}