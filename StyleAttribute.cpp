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


// ############################################
// -- CREATE OBJEKT --------------
template<typename ValueType> StyleAttribute<ValueType>::StyleAttribute(StyleAttributeBase::OnChangeListener *listener, Type type, initializer_list<int> causeCalc)
{
    // create value object , set onChangeListener
    value = new ValueType();
    ((Value*)value)->onChange(this);
    
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

void StyleAttributeBase::addBoundedView(View* view) {}
template<typename ValueType> void StyleAttribute<ValueType>::addBoundedView(View *view)
{
    // add view to end of list
    boundedViews.push_back(view);
}

// -- REMOVE

void StyleAttributeBase::removeBoundedView(View* view) {}
template<typename ValueType> void StyleAttribute<ValueType>::removeBoundedView(View *view)
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




// -- ON VALUE CHANGE -----------
template<typename ValueType> void StyleAttribute<ValueType>::onValueChange()
{    
    if (causeCalc[UI_VIEW_SELF] != UI_CALCTASK_NONE || causeCalc[UI_VIEW_PARRENT] != UI_CALCTASK_NONE )
    {   
        // for self, parent
        for (int i = 0; i < 2; i++) 
        {
            
            //cout << "[STATTR] boundedViews need Layout" << endl;
            // update each view -> add it to task list
            for (iterator = boundedViews.begin(); iterator != boundedViews.end(); iterator++)
            {
                //cout << "[STATTR] add view to calculationTasks" << endl << endl;
                // to parrent or self
                if (i == UI_VIEW_SELF)
                {
                    // add calc task to view self
                    (*iterator)->renderer->addCalcTask(causeCalc[i]);
                }
                else if (i == UI_VIEW_PARRENT)
                {
                    // add calc task to view parrent
                    View *parrent = (View*)(*iterator)->parent;
                    if (parrent != NULL)
                    {
                        parrent->renderer->addCalcTask(causeCalc[i]);
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

template<typename ValueType>
void StyleAttribute<ValueType>::onChange(StyleAttributeBase::OnChangeListener *listener) 
{
    this->onChangeListener = listener;
}


// -- SET IS AKTIVE -----------
bool StyleAttributeBase::aktive(bool aktive) {}; // for override
template<typename ValueType> bool StyleAttribute<ValueType>::aktive(bool aktive)
{
    if (this->isAktive != aktive)
    {
        this->isAktive = aktive;
        this->onChangeListener->onAktiveStateChange(this);
    }
    
    return this->isAktive;
}

bool StyleAttributeBase::aktive() {};   // for override
template<typename ValueType> bool StyleAttribute<ValueType>::aktive()
{
    return this->isAktive;
}




// -- for compiler --------------------------------
template class StyleAttribute<Value>;
template class StyleAttribute<IntValue>;
template class StyleAttribute<FloatValue>;
template class StyleAttribute<ColorValue>;
template class StyleAttribute<StringValue>;







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
