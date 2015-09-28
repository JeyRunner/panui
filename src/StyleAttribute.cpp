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


// ############################################
// -- CREATE OBJEKT --------------
StyleAttribute::StyleAttribute(OnChangeListener *listener, Type type, initializer_list<int> causeCalc)
{
    // create value object , set onChangeListener
//    value = new ValueType();
//    ((Value*)value)->onChange(this);
    // default
    autoMode = UI_ATTR_AUTO_AUTO;
    
    // set type
    this->type = type; 
    
    // set onChangeListener
    onChange(listener);
    
    // set causeLayout
    copy(causeCalc.begin(), causeCalc.end(), this->causeCalc);
    
    // deaktivate self
    isAktive = NON_AKTIVE;
}




// -- BOUNDED VIEWS -------------------------------------------------------------------
// -- ADD
void StyleAttribute::addBoundedView(View *view)
{
    // add view to end of list
    boundedViews.push_back(view);
    
    // add calc tasks
//    if (view)
//        view->renderer->addCalcTask(causeCalc[UI_VIEW_SELF]);
//    if (view->parent)
//        view->parent->renderer->addCalcTask(causeCalc[UI_VIEW_PARRENT]);
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
    if (causeCalc[UI_VIEW_SELF] != UI_CALCTASK_NONE || causeCalc[UI_VIEW_PARRENT] != UI_CALCTASK_NONE )
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
                    (*it)->renderer->addCalcTask(causeCalc[i]);
                }
                else if (i == UI_VIEW_PARRENT)
                {
                    // add calc task to view parrent
                    View *parent = (View*)(*it)->parent;
                    if (parent != NULL)
                    {
                        parent->renderer->addCalcTask(causeCalc[i]);
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

void StyleAttribute::setAuto(UI_ATTR_AUTO autoMode) 
{
    this->autoMode = autoMode;
    onValueChange();
}



string StyleAttribute::get()
{
    
}




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




// -- for compiler --------------------------------
//template class StyleAttribute<Value>;
//template class StyleAttribute<IntValue>;
//template class StyleAttribute<FloatValue>;
//template class StyleAttribute<ColorValue>;
//template class StyleAttribute<StringValue>;







// ########### OLD  ########################################
/*
StyleAttribute::StyleAttribute(Type type) 
{
    // inset into typeData
    typeData[0] = new TypeDataItem(top,                 "top",          Value::Int);
    typeData[1] = new TypeDataItem(left,                "left",         Value::Int);
    typeData[2] = new TypeDataItem(right,               "right",        Value::Int);
    typeData[3] = new TypeDataItem(bottom,              "bottom",       Value::Int);
    typeData[4] = new TypeDataItem(height,              "height",       Value::Int);
    typeData[5] = new TypeDataItem(widht,               "widht",        Value::Int);
    typeData[6] = new TypeDataItem(background_color, "background-color",Value::Color);
    typeData[7] = new TypeDataItem(alpha,               "alpha",        Value::Float);
    
    // set own type
    getTypeDataItem(type)->;
    
}


// ## TypeDataItem ###############################
StyleAttribute::TypeDataItem::TypeDataItem() {}
StyleAttribute::TypeDataItem::TypeDataItem(Type type, string typeName, Value::Type valueType)
{
    this->type      = type;
    this->typeName  = typeName;
    this->valueType = valueType;
}

// -- GET TYPE DATA ITEM --
StyleAttribute::TypeDataItem* StyleAttribute::getTypeDataItem(string type)
{
    // seach loop
    TypeDataItem* (*pointer) = &typeData[0];
    
    while(true) {
        if (pointer >= &typeData[sizeof(typeData)-1] )
        {
            break;
        } 
        else if((&pointer)->typeName == type)
        {
            return &pointer;
            break;
        }
        
        pointer++;
    }
    return NULL;
}

StyleAttribute::TypeDataItem* StyleAttribute::getTypeDataItem(Type type)
{
    // seach loop
    TypeDataItem* (*pointer) = &typeData[0];
    
    while(true) {
        if (pointer >= &typeData[sizeof(typeData)-1] )
        {
            break;
        } 
        else if((&pointer)->type == type)
        {
            return &pointer;
            break;
        }
        
        pointer++;
    }
    return NULL;
}
*/
