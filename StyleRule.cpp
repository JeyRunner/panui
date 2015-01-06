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


// ############################################
// -- CREATE OBJEKT --------------
StyleRule::StyleRule(string selector) 
{
    // -- create StyleAttribute objectes 
    //                                                                                   CALC ON SELF                    CALC ON PARRENT
    height           = new IntAttribute(this,    StyleAttribute::HEIGHT,            {UI_CALCTASK_LAYOUT_SIZE,        UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    width            = new IntAttribute(this,    StyleAttribute::WIDTH,             {UI_CALCTASK_LAYOUT_SIZE,        UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    left             = new IntAttribute(this,    StyleAttribute::LEFT,              {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    top              = new IntAttribute(this,    StyleAttribute::TOP,               {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    right            = new IntAttribute(this,    StyleAttribute::RIGHT,             {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    bottom           = new IntAttribute(this,    StyleAttribute::BOTTOM,            {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    backgroundColor = new ColorAttribute(this,  StyleAttribute::BACKGROUND_COLOR,  {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    opacity          = new FloatAttribute(this,  StyleAttribute::OPACITY,           {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    
    // text
    textSize         = new IntAttribute   (this, StyleAttribute::TEXT_SIZE,         {UI_CALCTASK_TEXT_SIZE,          UI_CALCTASK_NONE                        });
    textColor        = new ColorAttribute (this, StyleAttribute::TEXT_COLOR,        {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    textFamily       = new StringAttribute(this, StyleAttribute::TEXT_FAMILY,       {UI_CALCTASK_TEXT_FAMILY,        UI_CALCTASK_NONE                        });
    
    
    // -- set var
    this->selector = selector;
    
    // -- get Selector, Type
    setSelectorType(selector);
    setImportance(this->selectorType);
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
        default:              importance = 0;   break;
    }
    cout << "[RULE] add rule '" << selector << "' with importance " << importance << endl;
}


// -- GET STYLE ATTRIBUTE ----------
StyleAttribute* StyleRule::getAttribute(StyleAttribute::Type type)
{
    switch (type)
    {
        case StyleAttribute::HEIGHT:             return height;
        case StyleAttribute::WIDTH:              return width;       
        case StyleAttribute::LEFT:               return left;   
        case StyleAttribute::TOP:                return top;       
        case StyleAttribute::RIGHT:              return right;        
        case StyleAttribute::BOTTOM:             return bottom; 
        case StyleAttribute::BACKGROUND_COLOR:   return backgroundColor;   
        case StyleAttribute::OPACITY:            return opacity;
        case StyleAttribute::TEXT_SIZE:          return textSize;
        case StyleAttribute::TEXT_COLOR:         return textColor;
        case StyleAttribute::TEXT_FAMILY:        return textFamily;
        
        default:                                    cout << "[RULE] getAttribute [type does not fit] ->default" << endl; return NULL;
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
    for (iterator = boundedViews.begin(); /* iterator to start pos */
         iterator != boundedViews.end();  /* end if iterator at last pos */
         iterator++)
    {
        if (*iterator == view)
        {
            //  found View
            //  => remove
            boundedViews.erase(iterator);
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
    for (iterator = boundedViews.begin(); /* iterator to start pos */
         iterator != boundedViews.end();  /* end if iterator at last pos */
         iterator++)
    {
        // @TODO unbind views of unused Attributes form unused Attributes @FOR_BETTER_PERFORMANCE 
         
        // get Attribute that fits to view
        StyleAttribute *calculatedAttribute = (*iterator)->renderer->checkAttributeBinding(styleAttribute->type);
        
        // bind new fitting Attribute
       (*iterator)->renderer->bindAttribute(calculatedAttribute);
       
       cout << "[STRULE] reset Attribute (type '" << styleAttribute->type << "') (in progress...)" << endl;
    }
}




// ###########################################
// -- DESTROY OBJEKT -----------
StyleRule::~StyleRule()
{
    
}