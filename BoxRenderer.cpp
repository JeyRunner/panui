/* 
 * File:   BoxRenderer.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * BOXRENDERER CLASS
 * render BOXVIEW
 * extends RENDERER
 * ---------------------------------------
 */


#include "BoxRenderer.h"
#include "Box.h"
#include "View.h"



// ###########################################
// -- CREATE OBJEKT ------------------
BoxRenderer::BoxRenderer(Box *view) : Renderer(view)
{
    //this->view = view;
}


// -- CALC LAYOUT OF CHILDREN -----------------------
// -- CLAC POS
void BoxRenderer::calcLayoutChildrenPos() 
{
    //cout << "[RN of '" << view->id << "'] " << " calcLayoutChildrenPos()" << endl;
    
    // reset hightest hight
    chCur.hightesHight = 0;
    
    // set cursor to left top corner of self
    chCur.x = -(layoutAttributes.width->floatValue  /2);  // left border
    chCur.y = +(layoutAttributes.height->floatValue /2);  // top  border
    
    //cout << "[RN of '" << view->id << "'] " << "  => set cursor" << endl;
    
    // position each child
    for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)this->view)->children.end();                        /* end if iterator at last pos */
         iter++)
    { 
        // get position, width, height attributes
        IntAttribute *posAttr    = (*iter)->renderer->layoutAttributes.position;      
        IntAttribute *widthAttr  = (*iter)->renderer->layoutAttributes.width;
        IntAttribute *heightAttr = (*iter)->renderer->layoutAttributes.height;
        
        // calc child size
        if ((*iter)->renderer->calcTasks[UI_CALCTASK_LAYOUT_SIZE] /* if size attr has changed */)
            calcLayoutChildSize(*iter);
        
        // check for position type
        switch (posAttr->intValue)
        {
            case UI_ATTR_POSITION_ABSOLUTE:
                calcLayoutChildAbsolute(*iter);
                break;
                
            case UI_ATTR_POSITION_RELATIVE:
                calcLayoutChildRelative(*iter);
                break;
        }
    }
}



// -- CALC CHILD SIZE
void BoxRenderer::calcLayoutChildSize(View* v)
{
    // get var
    IntAttribute *widthAttr   = v->renderer->layoutAttributes.width;
    IntAttribute *heightAttr  = v->renderer->layoutAttributes.height;
    
    GLfloat       *widthFinal  = &(v->renderer->renderAttributes.width);
    GLfloat       *heightFinal = &(v->renderer->renderAttributes.height);

    
    // set width
    switch (widthAttr->mode)
    {
        // absolute
        case UI_ATTR__MODE_VALUE:
            *widthFinal = widthAttr->floatValue;
            break;
       
        // percentage
        case UI_ATTR__MODE_PERCENT:
            *widthFinal = widthAttr->percentValue * (layoutAttributes.width->floatValue / 100.0f);
            break;
    }
    
    
    // set height
    switch (heightAttr->mode)
    {
        // absolute
        case UI_ATTR__MODE_VALUE:
            *heightFinal = heightAttr->floatValue;
            break;
       
        // percentage
        case UI_ATTR__MODE_PERCENT:
            *heightFinal = heightAttr->percentValue * (layoutAttributes.height->floatValue / 100.0f);
            break;
    }
    
    // view calc own vertices
    v->renderer->calcLayoutSize();
    
    // done
    v->renderer->calcTasks[UI_CALCTASK_LAYOUT_SIZE] = false;
}



// -- CALC CHILD POS RELATIVE
void BoxRenderer::calcLayoutChildRelative(View* v) 
{
    // cout << "[RN of '" << view->id << "'] " << "  => pos child relative ['" << v->id << "']" << endl;
    // get render
    Renderer *ren = v->renderer;
    
    // check if open next row
    // cout << "[RN of '" << view->id << "'] " << "  child Cursor X: " << chCur.x << "     Self widht: " << layoutAttributes.width->floatValue << endl;
    if ((chCur.x +
            /* next View */ (ren->layoutAttributes.left->floatValue /* margin */      +ren->renderAttributes.width /* to right */) ) 
            >= (renderAttributes.width/2))
    {
        // if next view would positioned out of own border
        // => open nex row
        //cout << "[RN of '" << view->id << "'] " << " --------- open new row" << endl;
        chCur.Y(-chCur.hightesHight);
        chCur.x = -(renderAttributes.width  /2);  // left border
        //cout << "[RN of '" << view->id << "'] " << " --------- child Cursor Y: " << chCur.y << endl;
        
        // reset hightes hight
        chCur.hightesHight = 0;
    }
    
    

    
    // position
    // cursor X to center of view
    chCur.X(
        +ren->layoutAttributes.left->floatValue     /* margin */
        +ren->renderAttributes.width /2 /* to center */);
        
    // set pos X
    ren->renderAttributes.positionX = +chCur.x;
    
    // set pos Y
    ren->renderAttributes.positionY = +chCur.y 
                                      -ren->layoutAttributes.top->floatValue        /* margin */
                                      -ren->renderAttributes.height /2  /* to center */;
    
    // cursor to end of view
    chCur.X(
        +ren->layoutAttributes.right->floatValue     /* margin */
        +ren->renderAttributes.width /2 /* to right */);
    
    
    // calc hole hight width top, bottom
    int holeHight = ren->renderAttributes.height + ren->layoutAttributes.top->floatValue + ren->layoutAttributes.bottom->floatValue;
    
    // -- update hightes hight
    if (chCur.hightesHight < holeHight)
    {
        // => update
        chCur.hightesHight = holeHight;
    }
}


// -- CALC CHILD POS ABSOLUTE
void BoxRenderer::calcLayoutChildAbsolute(View* v) 
{
    v->renderer->renderAttributes.positionX = + (v->renderer->renderAttributes.width / 2) 
                                                  +  v->renderer->layoutAttributes.left->floatValue
                                                  - (renderAttributes.width  /2);  // left border
    
    v->renderer->renderAttributes.positionY = - (v->renderer->renderAttributes.height / 2) 
                                                  -  v->renderer->layoutAttributes.top->floatValue
                                                  + (renderAttributes.height /2);  // top  border
}
 


// -- POSITION ITSELF  ------------------------------
void BoxRenderer::calcLayoutSize() 
{
    // calc self
    Renderer::calcLayoutSize();
    
    // percent sized children need calc size
    for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)this->view)->children.end();                  /* end if iterator at last pos */
         iter++)
    { 
        // attributes
        IntAttribute *widthAttr  = (*iter)->renderer->layoutAttributes.width;
        IntAttribute *heightAttr = (*iter)->renderer->layoutAttributes.height;
        
        if (widthAttr->mode  == UI_ATTR__MODE_PERCENT  /* if percent value  */
         || heightAttr->mode == UI_ATTR__MODE_PERCENT) /* if percent value  */
            (*iter)->renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);
    }
        
    
    // update children pos
    calcLayoutChildrenPos();
}

// -- ADD CALC TASK
void BoxRenderer::addCalcTask(int type) 
{
    Renderer::addCalcTask(type);
    
    // check for own possibel task types
    switch (type)
    {
        case UI_CALCTASK_LAYOUT_CHIDREN_POSITION:
            calcTasks[type] = true;
    }
}


// -- EXE CALC TAKKS
int  BoxRenderer::exeCaclTasks() 
{
    // children calc tasks
    // for each child
     for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)view)->children.end();                          /* end if iterator at last pos */
         iter++)
    {
        // render child
        (*iter)->renderer->exeCaclTasks();
    }
    
     
    // own calc tasks
    int lastTask = Renderer::exeCaclTasks();
    int i;
    
    // for each calc task
    for (i =lastTask; i <= UI_CALCTASK_LAYOUT_CHIDREN_POSITION; i++)
    {
        // if true 
        if (calcTasks[i])
        {
            // -> execute
            switch (i)
            {
                case UI_CALCTASK_LAYOUT_CHIDREN_POSITION:
                    calcLayoutChildrenPos();
                    calcTasks[i] = false;
                    break;
            }
        }
    }
    
    return i;
}



// -- RENDER ----------------------------------------
void BoxRenderer::render() 
{
    // render self
    Renderer::render();
    
    // -- render children -------------
    // for each child
    for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)view)->children.end();                          /* end if iterator at last pos */
         iter++)
    {
        // render child
        (*iter)->renderer->render();
        
        // reset cursor to render next
        (*iter)->renderer->resetCursor();
    }
}





// -- HELP STUFF ------------------------------------
// -- CHIDREN CURSOR --------------------
void BoxRenderer::ChildrenCursor::X(float x) 
{ this->x = this->x + x;
}

void BoxRenderer::ChildrenCursor::Y(float y) 
{ this->y = this->y + y;
}

void BoxRenderer::ChildrenCursor::XY(float x, float y) 
{ X(x); Y(y); }




// ###########################################
// -- DESTROY OBJEKT -----------
BoxRenderer::~BoxRenderer() {
}

