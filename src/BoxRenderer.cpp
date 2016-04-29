/* 
 * File:   BoxRenderer.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * BOXRENDERER CLASS
 * render BOX(VIEW)
 * extends RENDERER
 *
 *
 * layout calculation:
 * [if self height = AUTO]
 * 1. self width                    => UI_CALCTASK_LAYOUT_SIZE
 * 2. children (for each child)
 * 2.1 width, height                => child->exeCalcTasks()
 * 2.2 position                     => UI_CALCTASK_LAYOUT_CHIDREN_POSITION
 * 3. self height                   => UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND
 * 4. own vertices                  => UI_CALCTASK_LAYOUT_SIZE_VERTICES
 *
 * layout calculation:
 * * [if self height = VALUE, PERCENT]
 * 1. self width, height            => UI_CALCTASK_LAYOUT_SIZE
 * 2. children (for each child)
 * 2.1 width, height                => child->exeCalcTasks()
 * 2.2 position                     => UI_CALCTASK_LAYOUT_CHIDREN_POSITION
 * 3. own vertices                  => UI_CALCTASK_LAYOUT_SIZE_VERTICES
 *
 *
 * if use height = AUTO
 * * own height only dependent on
 *   children with non percent height
 * ---------------------------------------
 */


#include "BoxRenderer.h"
#include "Box.h"
#include "View.h"
#include "Touch.h"
using namespace ui;


// ###########################################
// -- CREATE OBJEKT ------------------
BoxRenderer::BoxRenderer(Box *view) : Renderer(view)
{
    setLogName("RBOX");
    //this->view = view;
}


// -- CALC LAYOUT OF CHILDREN -----------------------
// -- CLAC POS
void BoxRenderer::calcLayoutChildrenPos() 
{
    NumericAttribute *padLeftAttr   = layoutAttributes.paddingLeft;
    NumericAttribute *padRightAttr  = layoutAttributes.paddingRight;
    NumericAttribute *padTopAttr    = layoutAttributes.paddingTop;
    NumericAttribute *padBottonAttr = layoutAttributes.paddingBottom;

    // reset hightest hight
    chCur.hightesHight  = 0;
    chCur.contendHeight = 0;

    // set cursor to left top corner of self
    chCur.x = -(renderAttributes.width  /2) + padLeftAttr->getFloat();  // left border
    chCur.y = +(renderAttributes.height /2) - padTopAttr->getFloat();   // top  border

    
    // position each child
    for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)this->view)->children.end();                        /* end if iterator at last pos */
         iter++)
    { 
        // get position, width, height attributes
        SelectAttribute<UI_ATTR_POSITION_ > *posAttr    = (*iter)->renderer->layoutAttributes.position;
      

        // check for position type
        switch (posAttr->value)
        {
            case UI_ATTR_POSITION_ABSOLUTE:
                calcLayoutChildAbsolute(*iter);
                break;
                
            case UI_ATTR_POSITION_RELATIVE:
                calcLayoutChildRelative(*iter);
                break;
        }
    }
    
    // add last row to contend height
    chCur.contendHeight += chCur.hightesHight;
        
    // set own contendSize
    renderAttributes.contendHeight = chCur.contendHeight + padTopAttr->getFloat() + padBottonAttr->getFloat() /* padding */;
    renderAttributes.contendWidth  = renderAttributes.width + padLeftAttr->getFloat() + padRightAttr->getFloat();
    
    // done
    // cout << "[DONE] calcLayoutChildrenPos of '"<< view->id << ", " << view->class_ <<"'" << endl;
    calcTasks[UI_CALCTASK_LAYOUT_CHILDREN_POSITION] = false;
}



// -- CALC CHILD POS RELATIVE
void BoxRenderer::calcLayoutChildRelative(View* v) 
{
    // get render
    Renderer *ren = v->renderer;
    NumericAttribute *padLeft   = layoutAttributes.paddingLeft;
    NumericAttribute *padRight  = layoutAttributes.paddingRight;
    
    // check if open next row
    if ((chCur.x +
        /* next View */ (ren->layoutAttributes.left->getFloat() /* margin */      +ren->renderAttributes.width /* to right */) )
            >= (renderAttributes.width/2 - padLeft->getFloat() - padRight->getFloat()))
    {
        // if next view would positioned out of own border
        // => open nex row
        chCur.Y(-chCur.hightesHight);
        chCur.x = -(renderAttributes.width  /2) + padLeft->getFloat()  // left border
                  +(layoutAttributes.scrollX->getFloat()); // scroll x
        
        // calc contend height
        chCur.contendHeight+= chCur.hightesHight;
        
        // reset hightes hight
        chCur.hightesHight = 0;
    }
    
    

    
    // position
    // cursor X to center of view
    chCur.X(
        +ren->layoutAttributes.left->getFloat()     /* margin */
        +ren->renderAttributes.width /2 /* to center */);
        
    // set pos X
    ren->renderAttributes.positionX = +chCur.x
                                      + (renderAttributes.scrollX); // scroll x
    
    // set pos Y
    ren->renderAttributes.positionY = +chCur.y 
                                      -ren->layoutAttributes.top->getFloat()        /* margin */
                                      -ren->renderAttributes.height /2  /* to center */
                                      - (renderAttributes.scrollY); // scroll y
    
    // cursor to end of view
    chCur.X(
        +ren->layoutAttributes.right->getFloat()     /* margin */
        +ren->renderAttributes.width /2 /* to right */);
    

    // only effect hightes height if no percent height  while parent has no AUTO height
    if (!(   (ren->layoutAttributes.height->getMode() == UI_ATTR__MODE_PERCENT)
          && (v->parent->renderer->layoutAttributes.height->getAuto() == UI_ATTR_AUTO_AUTO)))
    {
        // calc whole hight width top, bottom
        int wholeHight = ren->renderAttributes.height + ren->layoutAttributes.top->getFloat() +
                                                        ren->layoutAttributes.bottom->getFloat();
    
        // -- update hightes hight
        if (chCur.hightesHight < wholeHight)
        {
            // => update
            chCur.hightesHight = wholeHight;
        }
    }
}


// -- CALC CHILD POS ABSOLUTE
void BoxRenderer::calcLayoutChildAbsolute(View* v) 
{
    v->renderer->renderAttributes.positionX = + (v->renderer->renderAttributes.width / 2) 
                                                  +  v->renderer->layoutAttributes.left->getFloat()
                                                  - (renderAttributes.width  /2)            // left border
                                                  + (layoutAttributes.scrollX->getFloat()); // scroll x
    
    v->renderer->renderAttributes.positionY = - (v->renderer->renderAttributes.height / 2) 
                                                  -  v->renderer->layoutAttributes.top->getFloat()
                                                  + (renderAttributes.height /2);  // top  border
                                                  - (layoutAttributes.scrollY->getFloat()); // scroll y
}


// -- CALC LAYOUT SIZE HEIGHT DEPENDT ON CONTEND
bool BoxRenderer::calcLayoutSizeAutoContend()
{
    // get var
    GLfloat heightOld  = renderAttributes.height;
    GLfloat heightNew  = renderAttributes.contendHeight;

    // if height is auto
    if (Renderer::calcLayoutSizeAutoContend())
    {
        // correct vertical position of all children
        // calc shift amount
        GLfloat shift = (heightNew - heightOld) / 2.0;

        // shift each child
        for (auto child : ((Box*)view)->children)
        {
            child->renderer->renderAttributes.positionY+= shift;
        }
    }
}


// -- ADD CALC TASK
void BoxRenderer::addCalcTask(int type) 
{
    Renderer::addCalcTask(type);
    
    // check for own possibel task types
    switch (type)
    {
        case UI_CALCTASK_LAYOUT_CHILDREN_POSITION:
            calcTasks[UI_CALCTASK_LAYOUT_CHILDREN_POSITION]  = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND] = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES]     = true;
            break;

        case UI_CALCTASK_LAYOUT_SIZE:
            calcTasks[UI_CALCTASK_LAYOUT_SIZE]              = true;
            calcTasks[UI_CALCTASK_LAYOUT_CHILDREN_POSITION]  = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND] = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES]     = true;
    }
}


// -- EXE CALC TAKKS
int  BoxRenderer::exeCalcTasks()
{
    // remember calc size
    bool needCalcSize = calcTasks[UI_CALCTASK_LAYOUT_SIZE];

    /* calc width, (height if not depends on contend)
     * [!] do befor children calc size (may depend on parent width or height)
     */
    if(calcTasks[UI_CALCTASK_LAYOUT_SIZE])
        calcLayoutSize();


    // children calc tasks
    // for each child
     for (list<View*>::iterator iter = ((Box*)view)->children.begin(); /* iterator to start pos */
         iter != ((Box*)view)->children.end();                          /* end if iterator at last pos */
         iter++)
    {
        // if own size changed
        // => percent sized children need calc size
        if(needCalcSize)
        {
            // attributes
            NumericAttribute *widthAttr = (*iter)->renderer->layoutAttributes.width;
            NumericAttribute *heightAttr = (*iter)->renderer->layoutAttributes.height;

            if (widthAttr->getMode() == UI_ATTR__MODE_PERCENT     /* if percent value  */
                || heightAttr->getMode() == UI_ATTR__MODE_PERCENT /* if percent value  */
                || widthAttr->getAuto() == UI_ATTR_AUTO_AUTO  /* if auto width */)
            {
                (*iter)->renderer->addCalcTask(UI_CALCTASK_LAYOUT_SIZE);
            }
        }


        // calc child
        (*iter)->renderer->exeCalcTasks();
    }
    
     
    // own calc tasks
    if (calcTasks[UI_CALCTASK_LAYOUT_CHILDREN_POSITION]) {
        calcLayoutChildrenPos();
        Touch::needReCheck = true;
    }

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND])
        calcLayoutSizeAutoContend();

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES])
        calcLayoutSizeVertices();

    return 0;
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

        // reset stencil buffer to render next
        (*iter)->renderer->resetStencilBuffer();

        // reset cursor to render next
        (*iter)->renderer->resetCursor();
    }
}


// -- CHECK TOUCH POINT IS OVER ------------------------------
View* BoxRenderer::isOver(float x, float y) 
{
    // var
    View *viewChild;

    if (Renderer::isOver(x,y) || touchAttributes.childNeedIsOver)
    {
        // var
        View *v;

        // make x,y relative to self
        x-= renderAttributes.positionX;
        y-= renderAttributes.positionY;

        // check all children - backwards
        for (list<View*>::reverse_iterator iter = ((Box*)view)->children.rbegin(); /* iterator to start pos */
             iter != ((Box*)view)->children.rend();                        /* end if iterator at last pos */
             ++iter)
        {
            viewChild = (*iter)->renderer->isOver(x,y);

            if (viewChild)
            {
                v = viewChild;
                //if (!touchAttributes.childNeedIsOver)
                    return v;
            }

        }

        // is not over child
        return this->view;
    }
    
    return NULL;
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