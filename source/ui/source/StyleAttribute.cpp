/* 
 * File:   StyleAttribute.cpp
 * Author: Joshua Johannson | Pancake
 *
 *
 * ---------------------------------------
 * STYLEATTRIBUTE CLASS
 * owns value
 * style attribute: left, top, background-color, ...
 * ---------------------------------------
 */


#include "StyleAttribute.h"
#include "View.h"
#include "Ui.h"
#include "const.h"
#include "Renderer.h"
#include "Box.h"
using namespace ui;

// cause calc list
StyleAttribute::CauseCalcElement* StyleAttribute::causeCalcList[_LAST_TYPE_AFTER] = {
//                                                               CALC ON SELF                       CALC ON PARENT
        new CauseCalcElement(StyleAttribute::HEIGHT,            UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::WIDTH,             UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::LEFT,              UI_CALCTASK_NONE,                  UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::TOP,               UI_CALCTASK_NONE,                  UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::RIGHT,             UI_CALCTASK_NONE,                  UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::BOTTOM,            UI_CALCTASK_NONE,                  UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::POSITION,          UI_CALCTASK_NONE,                  UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::SCROLL_X,          UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_NONE                    ),
        new CauseCalcElement(StyleAttribute::SCROLL_Y,          UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_NONE                    ),

        new CauseCalcElement(StyleAttribute::PADDING_LEFT,      UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::PADDING_RIGHT,     UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::PADDING_TOP,       UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),
        new CauseCalcElement(StyleAttribute::PADDING_BOTTOM,    UI_CALCTASK_LAYOUT_SIZE,           UI_CALCTASK_LAYOUT_CHILDREN_POSITION),

        new CauseCalcElement(StyleAttribute::BACKGROUND_COLOR,  UI_CALCTASK_NONE,                  UI_CALCTASK_NONE                    ),
        new CauseCalcElement(StyleAttribute::OPACITY,           UI_CALCTASK_NONE,                  UI_CALCTASK_NONE                    ),
        new CauseCalcElement(StyleAttribute::OVERFLOW_CUT,      UI_CALCTASK_NONE,                  UI_CALCTASK_NONE                    ),

        new CauseCalcElement(StyleAttribute::TEXT_SIZE,         UI_CALCTASK_TEXT_SIZE,             UI_CALCTASK_NONE                    ),
        new CauseCalcElement(StyleAttribute::TEXT_COLOR,        UI_CALCTASK_NONE,                  UI_CALCTASK_NONE                    ),
        new CauseCalcElement(StyleAttribute::TEXT_FAMILY,       UI_CALCTASK_TEXT_FAMILY,           UI_CALCTASK_NONE                    )
};


// ############################################
// -- CREATE OBJEKT --------------
StyleAttribute::StyleAttribute(OnChangeListener *listener, Type type)
{
    setLogName("ATTR");

    // default
    autoMode = UI_ATTR_AUTO_AUTO;
    
    // set type
    this->type = type; 
    
    // set onChangeListener
    onChange(listener);

    // set causeLayout
    causeCalc = getCauseCalcForType(type);

    // deaktivate self
    isAktive = NON_AKTIVE;
}


StyleAttribute::CauseCalcElement *StyleAttribute::getCauseCalcForType(Type type)
{
    for (auto el : causeCalcList)
    {
        if (el->type == type)
            return el;
    }
    err("cause calc settings for type " + to_string(type) + " not fonund");
    return nullptr;
}

// -- BOUNDED VIEWS -------------------------------------------------------------------
// -- ADD
void StyleAttribute::addBoundedView(View *view)
{
    // add view to end of list
    boundedViews.push_back(view);
    
    // add calc tasks
//    if (view)
//        view->renderer->addCalcTask(causeCalcList[UI_VIEW_SELF]);
//    if (view->parent)
//        view->parent->renderer->addCalcTask(causeCalcList[UI_VIEW_PARENT]);
}

// -- REMOVE
void StyleAttribute::removeBoundedView(View *view)
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




// -- ON VALUE CHANGE -----------
void StyleAttribute::onValueChange()
{
    if (causeCalc->causeCalc[UI_VIEW_SELF] != UI_CALCTASK_NONE || causeCalc->causeCalc[UI_VIEW_PARENT] != UI_CALCTASK_NONE )
    {
        // for self, parent
        for (int i = 0; i < 2; i++) 
        {
            
            //cout << "[STATTR] boundedViews need Layout" << endl;
            // update each view -> add it to task list
            for (list<View*>::iterator it = boundedViews.begin(); it != boundedViews.end(); it++)
            {
                //cout << "[STATTR] add view to calculationTasks" << endl << endl;
                // to parrent or self
                if (i == UI_VIEW_SELF)
                {
                    // add calc task to view self
                    (*it)->renderer->addCalcTask(causeCalc->causeCalc[i]);
                }
                else if (i == UI_VIEW_PARENT)
                {
                    // add calc task to view parrent
                    View *parent = (View*)(*it)->parent;
                    if (parent != NULL)
                    {
                        parent->renderer->addCalcTask(causeCalc->causeCalc[i]);
                    }
                }


            }
        }
        
    }
    
    // ??????????????????? ----
    // -- aktivate auto -> not backwards
    if (!aktive()) { aktive(true); }
}



// -- SET ON CHANGE LISTENER --
void StyleAttribute::onChange(OnChangeListener *listener) 
{
    this->onChangeListener = listener;
}


// -- SET GET VALUE ----------------------------------------
void StyleAttribute::set(string value)
{
    
}


StyleRule &StyleAttribute::operator()(string value)
{
    return getRule();
}


void StyleAttribute::setAuto(UI_ATTR_AUTO autoMode)
{
    this->autoMode = autoMode;
    onValueChange();
}

UI_ATTR_AUTO StyleAttribute::getAuto()
{
    return autoMode;
}

string StyleAttribute::get()
{
    
}

StyleRule &StyleAttribute::getRule()
{ return *dynamic_cast<StyleRule*>(onChangeListener); }



// -- SET IS AKTIVE -----------
bool StyleAttribute::aktive(bool aktive)
{
    if (this->isAktive != aktive)
    {
        this->isAktive = aktive;
        this->onChangeListener->onAktiveStateChange(this);
    }
    
    return this->isAktive;
}

bool StyleAttribute::aktive()
{
    return this->isAktive;
}