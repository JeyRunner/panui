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
    //                                                                                                  CALC ON SELF                    CALC ON PARRENT
    heigh               = new StyleAttribute<IntValue>(this,    StyleAttributeBase::HEIGHT,            {UI_CALCTASK_LAYOUT_SIZE,        UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    width               = new StyleAttribute<IntValue>(this,    StyleAttributeBase::WIDTH,             {UI_CALCTASK_LAYOUT_SIZE,        UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    left                = new StyleAttribute<IntValue>(this,    StyleAttributeBase::LEFT,              {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    top                 = new StyleAttribute<IntValue>(this,    StyleAttributeBase::TOP,               {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    right               = new StyleAttribute<IntValue>(this,    StyleAttributeBase::RIGHT,             {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    bottom              = new StyleAttribute<IntValue>(this,    StyleAttributeBase::BOTTOM,            {UI_CALCTASK_NONE,               UI_CALCTASK_LAYOUT_CHIDREN_POSITION     });
    backgroundColor     = new StyleAttribute<ColorValue>(this,  StyleAttributeBase::BACKGROUND_COLOR,  {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    opacity             = new StyleAttribute<FloatValue>(this,  StyleAttributeBase::OPACITY,           {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    
    // text
    textSize            = new StyleAttribute<IntValue>   (this, StyleAttributeBase::TEXT_SIZE,         {UI_CALCTASK_TEXT_SIZE,          UI_CALCTASK_NONE                        });
    textColor           = new StyleAttribute<ColorValue> (this, StyleAttributeBase::TEXT_COLOR,        {UI_CALCTASK_NONE,               UI_CALCTASK_NONE                        });
    textFamily          = new StyleAttribute<StringValue>(this, StyleAttributeBase::TEXT_FAMILY,       {UI_CALCTASK_TEXT_FAMILY,        UI_CALCTASK_NONE                        });
    
    
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
    if      (selectorTypeStr == '#') { selectorType = ID_SELECTOR; }
    else if (selectorTypeStr == '.') { selectorType = CLASS_SELECTOR; }
    else                             { selectorType = VIEW_SELECTOR; }
}


// -- SET IMPORTANCE BY SELECTOR TYPE
void StyleRule::setImportance(SelectorType selectorType)
{
    switch (selectorType)
    {
        case ID_SELECTOR:     importance = 100; break;
        case CLASS_SELECTOR:  importance = 20;  break;
        case VIEW_SELECTOR:   importance = 10;  break;
    }
    cout << "[RULE] add rule '" << selector << "' with importance " << importance << endl;
}


// -- GET STYLE ATTRIBUTE ----------
StyleAttributeBase* StyleRule::getAttribute(StyleAttributeBase::Type type)
{
    switch (type)
    {
        case StyleAttributeBase::HEIGHT:             return heigh;
        case StyleAttributeBase::WIDTH:              return width;       
        case StyleAttributeBase::LEFT:               return left;   
        case StyleAttributeBase::TOP:                return top;       
        case StyleAttributeBase::RIGHT:              return right;        
        case StyleAttributeBase::BOTTOM:             return bottom; 
        case StyleAttributeBase::BACKGROUND_COLOR:   return backgroundColor;   
        case StyleAttributeBase::OPACITY:            return opacity;
        case StyleAttributeBase::TEXT_SIZE:          return textSize;
        case StyleAttributeBase::TEXT_COLOR:         return textColor;
        case StyleAttributeBase::TEXT_FAMILY:        return textFamily;
        
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
void StyleRule::onAktiveStateChange(StyleAttributeBase *styleAttributeBase) 
{
    // bounded views -> each
    for (iterator = boundedViews.begin(); /* iterator to start pos */
         iterator != boundedViews.end();  /* end if iterator at last pos */
         iterator++)
    {
        // @TODO unbind views of unused Attributes form unused Attributes @FOR_BETTER_PERFORMANCE 
         
        // get Attribute that fits to view
        StyleAttributeBase *calculatedAttribute = (*iterator)->renderer->checkAttributeBinding(styleAttributeBase->type);
        
        // bind new fitting Attribute
       (*iterator)->renderer->bindAttribute(calculatedAttribute);
       
       cout << "[STRULE] reset Attribute (type '" << styleAttributeBase->type << "') (in progress...)" << endl;
    }
}




// ###########################################
// -- DESTROY OBJEKT -----------
StyleRule::~StyleRule()
{
    
}