/* 
 * File:   Renderer.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * RENDERER CLASS
 * render VIEW
 *
 * * layout calculation:
 * 1. self width                    => UI_CALCTASK_LAYOUT_SIZE
 * 2. self height                   => UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND
 * 3. own vertices                  => UI_CALCTASK_LAYOUT_SIZE_VERTICES
 *
 * * connects StyleRules, StyleAttributes
 * ---------------------------------------
 */

#ifdef pl_andr
#include <SDL.h>
#endif

#include <string.h>
#include <sstream>
//#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Style.h"
#include "Box.h"
#include "BoxRenderer.h"
#include "Ui.h"
#include "GL.h"
#include "Touch.h"
using namespace ui;


// ###########################################
// -- CREATE OBJEKT ------------------
Renderer::Renderer()
{
    setLogName(" RN ");

    // set pointer var
    //renderAttributesPtr = &renderAttributes;
    //layoutAttributesPtr = &layoutAttributes;
    renderAttributes.stencilIndex = 0;
    
    // default
    renderAttributes.positionX          = 0;
    renderAttributes.positionY          = 0;
    renderAttributes.contendHeight      = 0;
    renderAttributes.contendWidth       = 0;
    touchAttributes.isOver              = false;
    touchAttributes.drag                = false;
    touchAttributes.childNeedIsOver     = false;
    touchAttributes.enter               = false;
    touchAttributes.leave               = false;
    touchAttributes.relativeParent      = {0,0};
    touchAttributes.relativeSelf        = {0,0};
    touchAttributes.relativeSelfDrag    = {0,0};

    // need to calc all
    for (int i = 0; i <= UI_CALCTASK__SIZE; ++i)
    {
        calcTasks[i] = true;
    }
}

Renderer::Renderer(View *view) : Renderer::Renderer()
{

    // set view
    this->view = view;

    // set pointer var
    //renderAttributesPtr = &renderAttributes;
    //layoutAttributesPtr = &layoutAttributes;
    
    // -- bind rules and attributes
    // bindRuleAutomatic();
    // bindAttributeAutomatic();
}


// == CALCULATE LAYOUT ==================================
// -- CALC LAYOUT SIZE
void Renderer::calcLayoutSize() 
{
    // if is root view
    if (view->parent == NULL)
    {
        // done
        calcTasks[UI_CALCTASK_LAYOUT_SIZE] = false;
        return;
    }


    // get var
    IntAttribute *widthAttr     = view->renderer->layoutAttributes.width;
    IntAttribute *heightAttr    = view->renderer->layoutAttributes.height;
    IntAttribute *topAttr       = view->renderer->layoutAttributes.top;
    IntAttribute *bottomAttr    = view->renderer->layoutAttributes.bottom;
    IntAttribute *leftAttr      = view->renderer->layoutAttributes.left;
    IntAttribute *rightAttr     = view->renderer->layoutAttributes.right;

    IntAttribute *parentPadLeft     = view->parent->renderer->layoutAttributes.paddingLeft;
    IntAttribute *parentPadRight    = view->parent->renderer->layoutAttributes.paddingRight;
    IntAttribute *parentPadTop      = view->parent->renderer->layoutAttributes.paddingTop;
    IntAttribute *parentPadBotton   = view->parent->renderer->layoutAttributes.paddingBottom;
    GLfloat      *parentWidth       = &(view->parent->renderer->renderAttributes.width);
    GLfloat      *parentHeight      = &(view->parent->renderer->renderAttributes.height);

    GLfloat       *widthFinal   = &(view->renderer->renderAttributes.width);
    GLfloat       *heightFinal  = &(view->renderer->renderAttributes.height);


    // set width
    // if not in auto mode
    if (widthAttr->autoMode == UI_ATTR_AUTO_NONE)
    {
        switch (widthAttr->mode)
        {
            // absolute
            case UI_ATTR__MODE_VALUE:
                *widthFinal = widthAttr->floatValue;
                break;

                // percentage
            case UI_ATTR__MODE_PERCENT:
            {

                *widthFinal = widthAttr->percentValue * (*parentWidth / 100.0f) - leftAttr->floatValue - rightAttr->floatValue - parentPadLeft->floatValue - parentPadRight->floatValue;
            }
                break;
        }
    }
        // in auto mode
    else if (widthAttr->autoMode == UI_ATTR_AUTO_AUTO)
    {
        *widthFinal = *parentWidth - leftAttr->floatValue - rightAttr->floatValue - parentPadLeft->floatValue - parentPadRight->floatValue;
    }


    // set height
    // if not in auto mode
    if (heightAttr->autoMode == UI_ATTR_AUTO_NONE)
    {
        switch (heightAttr->mode)
        {
            // absolute
            case UI_ATTR__MODE_VALUE:
                *heightFinal = heightAttr->floatValue;
                break;

                // percentage
            case UI_ATTR__MODE_PERCENT:
                // only set height if parent heigt depends not on contend
                if (view->parent->renderer->layoutAttributes.height->autoMode != UI_ATTR_AUTO_AUTO)
                    *heightFinal = heightAttr->percentValue * (*parentHeight / 100.0f) - topAttr->floatValue - bottomAttr->floatValue - parentPadTop->floatValue - parentPadBotton->floatValue;
                else
                    *heightFinal = 0.0;
                break;
        }
    }
        // in auto mode
    else if (heightAttr->autoMode == UI_ATTR_AUTO_AUTO)
    {
        // calculated later in calcLayoutSizeAutoContend()
    }


    // set self scroll
    if (abs(renderAttributes.contendHeight - renderAttributes.height) < -renderAttributes.scrollY)
    {
        renderAttributes.scrollY = -(renderAttributes.contendHeight - renderAttributes.height);
    }
    //renderAttributes.scrollX = layoutAttributes.scrollX->floatValue;
    //renderAttributes.scrollY = layoutAttributes.scrollY->floatValue;

    // done
    // cout << "[DONE] calcLayoutSize of '"<< view->id << ", " << view->class_ <<"'" << endl;
    calcTasks[UI_CALCTASK_LAYOUT_SIZE] = false;

    // later -> calcLayoutSizeVertices()
}

// -- CALC LAYOUT SIZE HEIGHT DEPENDT ON CONTEND
bool Renderer::calcLayoutSizeAutoContend()
{
    // get var
    IntAttribute *heightAttr    = view->renderer->layoutAttributes.height;
    GLfloat      *heightFinal   = &(view->renderer->renderAttributes.height);
    GLfloat       heightOld     = (view->renderer->renderAttributes.height);

    // set height
    // in auto mode
    bool Auto = (heightAttr->autoMode == UI_ATTR_AUTO_AUTO);
    if (Auto)
    {
        // set new height
        *heightFinal = view->renderer->renderAttributes.contendHeight;

        // if height has changed
        // => parent need to rePos children
        if (heightOld != *heightFinal)
        {
            if ((view->parent) != NULL)
            {
                view->parent->renderer->addCalcTask(UI_CALCTASK_LAYOUT_CHILDREN_POSITION);
            }
        }
    }

    // done
    // cout << "[DONE] calcLayoutSizeAutoContend of '"<< view->id << ", " << view->class_ <<"'" << endl;
    calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND] = false;
    return Auto;
}

// -- CALC LAYOUT VERTICES WITH HEIGHT AND WIDTH VALUES
void Renderer::calcLayoutSizeVertices()
{
    // if vertex buffer not generated
    if (renderAttributes.vertexBuffer <= 0)
    {
        // create vertex buffer
        glGenBuffers(1, &(renderAttributes.vertexBuffer));
        glBindBuffer(GL_ARRAY_BUFFER,renderAttributes.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*4, NULL, GL_DYNAMIC_DRAW /* often changed and used for rendering */ );
    }
    else
        glBindBuffer(GL_ARRAY_BUFFER,renderAttributes.vertexBuffer);


    float heightHalf = renderAttributes.height / 2;
    float widthHalf  = renderAttributes.width  / 2;

    // -- clac vertices
    GLfloat vertices[12] = {
            -widthHalf, -heightHalf, 0.0f, /* left  Bottom */
            -widthHalf,  heightHalf, 0.0f, /* left  Top */
            widthHalf, -heightHalf, 0.0f, /* right Bottom */
            widthHalf,  heightHalf, 0.0f  /* right Top */};

    // put data in vertex buffer

    // insert data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
    //debug("buffer "+str(sizeof(vertices))+"  bytes");

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // -- copy into vertices
    // memcpy(renderAttributes.vertices, vertices, sizeof(vertices));

    // done
    calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES] = false;
}


// -- ADD CALC TASK
void Renderer::addCalcTask(int type)
{
    // check if calc task fits
    switch (type)
    {
        case UI_CALCTASK_LAYOUT_SIZE:
            // add calc task
            calcTasks[UI_CALCTASK_LAYOUT_SIZE] = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND] = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES] = true;
            break;

        case UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND:
            // add calc task
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND] = true;
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES] = true;
            break;

        case UI_CALCTASK_LAYOUT_SIZE_VERTICES:
            // add calc task
            calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES] = true;
            break;
    }        
}


// -- EXE CALC TASKS
int  Renderer::exeCalcTasks()
{
    if(calcTasks[UI_CALCTASK_LAYOUT_SIZE])
        calcLayoutSize();

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_AUTO_CONTEND])
    calcLayoutSizeAutoContend();

    if (calcTasks[UI_CALCTASK_LAYOUT_SIZE_VERTICES])
        calcLayoutSizeVertices();
    
    return 0;
}




// == RENDER =======================================
void Renderer::render()
{
    int stencilIndexParent = 0;

    // set amount of parents
    // if root
    if (view->parent == NULL)
    {
        renderAttributes.stencilIndex = 1;
    }
    else
    {
        if (renderAttributes.overflow->value == UI_ATTR_OVERFLOW_HIDDEN) {
            renderAttributes.stencilIndex = ((view->parent->renderer->renderAttributes.stencilIndex) + 1);
        }
        else {
            renderAttributes.stencilIndex = (view->parent->renderer->renderAttributes.stencilIndex);
        }
        stencilIndexParent = (view->parent->renderer->renderAttributes.stencilIndex);

        // stencil buffer size = 8bit =>> only 255 layer
        if (renderAttributes.stencilIndex > 255)
            err("stencilIndex is higher than stencil buffers available max value of 255, there will be problems with cut overflow [means view has more than 255 parents, to solve reduce parents amount]");
    }


    // bind view background shader
    glUseProgram(GL::SHADER_VIEW_BACKGROUND);

    // give gl the Vertices via buffer
    glBindBuffer(GL_ARRAY_BUFFER, renderAttributes.vertexBuffer);

    // draw a rectangle ----------------------------------------------
    // activate for client (to draw, not calculate) vertex Array
    // set varable for vertex to vertexPos attribute of vertexShader
    // => kind  of glBeginn()
    glEnableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);

    // set attributes / uniformes  
    // set transform and projection matix
    // remove cursor -> to own position
    glm::mat4 transform = glm::translate(glm::vec3(renderAttributes.positionX /* X */,
                                                   renderAttributes.positionY /* Y */,
                                                   0.0f                       /* Z */ ))
                          * GL::transformMatrix;
    glm::mat4 modelMatrix = GL::projectionMatrix * transform;
    
    glUniformMatrix4fv(GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &modelMatrix[0][0]);
    
    GL::transformMatrix = transform;
    
    
    // set shader color
    glUniform4f(GL::SHADER_VIEW_BACKGROUND_UNIF_COLOR,
                renderAttributes.background_color->r,
                renderAttributes.background_color->g,
                renderAttributes.background_color->b,
                renderAttributes.background_color->alpha);

    // give gl the Vertices format
    glVertexAttribPointer(
            GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS      /* pass vertices to vertex Pos attribute of vertexShader */,
            3                                               /* 3 Aruments: x,y,z */,
            GL_FLOAT                                        /* Format: float     */,
            GL_FALSE                                        /* take values as-is */,
            0                                               /* Entry lenght ?    */,
            NULL                                            /* offset / vertices Array    */ );




    if (renderAttributes.overflow->value == UI_ATTR_OVERFLOW_HIDDEN)
    {
        // -- draw into stencil buffer
        // stencil set criteria
        glStencilFunc(GL_EQUAL /* set if there parent allows to paint */,
                      stencilIndexParent /* set value */,
                      0xFF      /* both values AND mask 0xFF */);

        // enable writing to stencil buffer, but not to screen
        glStencilOp(GL_KEEP /* stencil func fail */,
                    GL_INCR /* stencil func ok, but deep fail =>> set to value */,
                    GL_INCR /* both ok                        =>> set to value*/);
        //glStencilMask(0xFF);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // draw onw area in stencil buffer
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // -- draw by stencil criteria
    // disable writing to stencil buffer, but to screen
    //glStencilMask(0x00);
    glStencilOp(GL_KEEP /* stencil func fail */,
                GL_KEEP /* stencil func ok, but deep fail =>> set to value */,
                GL_KEEP /* both ok                        =>> set to value*/);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // stencil set criteria
    glStencilFunc(GL_LESS  /* set if stencil val less then ref */,
                  renderAttributes.stencilIndex -1 /* ref */,
                  0xFF      /* both values AND mask 0xFF */);


    // draw with the vertices form the given Array
    // make two connected triangle to create a rectangle
    glDrawArrays(
        GL_TRIANGLE_STRIP,
        0 /* Array start pos   */,
        4 /* how much vertices */);


    // deactivate vertex Array mode
    // => end of operation
    glDisableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);

    // unbind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind shader
    glUseProgram(0);


}


// -- RENDER IN GL 1 CONTEXT  ----------------------
void Renderer::renderGl_1() 
{
     // debug out
    /*
    cout << "[RN of '" << view->id << "'] " << "Renderer->render()   view[id: " << view->id << ", class: " << view->class_ << "]" << endl;
    cout << "                                        position: " << renderAttributes.positionX << "," << renderAttributes.positionY << endl; 
    cout << "                                        vertices: ";
    int c=0;
    for (float &i : renderAttributes.vertices)
    {
        if (c >= 3)
        {
            cout << "   ";
            c = 0;
        }
        cout << i << ",";
        c++;
    }
    cout << endl;
    cout << "                                        color: r=" << renderAttributes.background_color->r << " g:" << renderAttributes.background_color->g 
                                                                << " b:" << renderAttributes.background_color->b << " alpha:" << renderAttributes.background_color->alpha << endl;
    */ 
//#ifndef pl_andr
#ifdef unused
    // remove cursor -> to own position
    glTranslatef(
            renderAttributes.positionX /* x */,
            renderAttributes.positionY /* Y */,
            0.0f                       /* Z = 0 */);
    
    // draw a rectangle ----------------------------------------------
    // activate for client (to draw, not calculate) vertex Array
    // => tell gl we want to draw a something with a vertex Array
    // => kind  of glBeginn()
    glEnableClientState(GL_VERTEX_ARRAY);
    
    // give gl the Vertices via array
    glVertexPointer(
        3                            /* 3 Aruments: x,y,z */,
        GL_FLOAT                     /* Format: float     */,
        0                            /* Entry lenght ?    */,
        renderAttributes.vertices    /* vertices Array    */ );
    
    // set the color of vertices
    glColor4f(
        renderAttributes.background_color->r     /* Red   */,
        renderAttributes.background_color->g     /* Green */,
        renderAttributes.background_color->b     /* Blue  */,
        renderAttributes.background_color->alpha /* Alpha */);
    
    // draw with the vertices form the given Array
    // make two connected triangle to create a rectangle
    glDrawArrays(
        GL_TRIANGLE_STRIP,
        0 /* Array start pos   */,
        4 /* how much vertices */);
    
    // deactivate vertex Array mode
    // => end of operation
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
}



// -- RESET CURSOR ---------------------------------
void Renderer::resetCursor() 
{
    // remove cursor -> position before
    // => render next view
    // remove cursor -> to own position
    glm::mat4 transform = glm::translate(glm::vec3( -renderAttributes.positionX /* X */,
                                                     -renderAttributes.positionY /* Y */,
                                                     -0.0f                       /* Z */ ))
                                                     * GL::transformMatrix;
    glm::mat4 model     = GL::projectionMatrix * transform;
    
    glUniformMatrix4fv(GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &model[0][0]);
    
    GL::transformMatrix = transform;
    
    /* OLD: OpenGl ES 1.x
    glTranslatef(
            -renderAttributes.positionX /* x *-/,
            -renderAttributes.positionY /* Y *-/,
             0.0f                        /* Z = 0 *-/);
    */
}


void Renderer::resetStencilBuffer()
{
    if (renderAttributes.overflow->value == UI_ATTR_OVERFLOW_HIDDEN)
    {
        // set amount of parents
        // if root
        int stencilIndexParent = 0;
        if (view->parent != NULL)
        {
            stencilIndexParent = (view->parent->renderer->renderAttributes.stencilIndex);
        }

        // bind view background shader
        glUseProgram(GL::SHADER_VIEW_BACKGROUND);

        // give gl the Vertices via buffer
        glBindBuffer(GL_ARRAY_BUFFER, renderAttributes.vertexBuffer);

        // draw a rectangle ----------------------------------------------
        // => vertexPos attribute of vertexShader
        // => kind  of glBeginn()
        glEnableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);


        glm::mat4 modelMatrix = GL::projectionMatrix * GL::transformMatrix;
        glUniformMatrix4fv(GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                           1                /* amount of matrix */,
                           GL_FALSE         /* convert format -> NO */,
                           &modelMatrix[0][0]);



        // set shader color
        glUniform4f(GL::SHADER_VIEW_BACKGROUND_UNIF_COLOR,
                    1,
                    1,
                    1,
                    1);

        // give gl the Vertices via array
        // give gl the Vertices via buffer
        // glBindBuffer(GL_ARRAY_BUFFER, renderAttributes.vertexBuffer);
        glVertexAttribPointer(
                GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS      /* pass vertices to vertex Pos attribute of vertexShader */,
                3                                               /* 3 Aruments: x,y,z */,
                GL_FLOAT                                        /* Format: float     */,
                GL_FALSE                                        /* take values as-is */,
                0                                               /* Entry lenght ?    */,
                NULL                                            /* vertices Array    */ );


        // -- draw into stencil buffer
        // stencil set criteria
        glStencilFunc(GL_EQUAL /* set in every case */,
                      renderAttributes.stencilIndex /* set value */,
                      0xFF      /* both values AND mask 0xFF */);

        // enable writing to stencil buffer, but not to screen
        glStencilOp(GL_KEEP /* stencil func fail */,
                    GL_DECR /* stencil func ok, but deep fail =>> set to value */,
                    GL_DECR /* both ok                        =>> set to value*/);

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        // draw onw area in stencil buffer
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        // deactivate vertex Array mode
        // => end of operation
        glDisableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);

        // unbind buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbind shader
        glUseProgram(0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
}




// == BINDINGS =====================================
// -- CHECK RULE BINDING
int Renderer::checkRuleBinding(StyleRule *rule) 
{
    /* @TODO checkRuleBinding -> multiselect   
     */ 
    
    // -- View selector
    if (rule->selectorType == StyleRule::VIEW_SELECTOR and  rule->selector == view->type )
    { return 1; }
    
    // -- Id selector
    if (rule->selectorType == StyleRule::ID_SELECTOR and  rule->selector == view->id )
    { return 2; }
    
    // -- Class selector
    if (rule->selectorType == StyleRule::CLASS_SELECTOR and  rule->selector == view->class_ )
    { return 1; }
    
    // -- Global selector
    if (rule->selectorType == StyleRule::GLOBAL_SELECTOR)
    { return 1; trace("GLOBAL SELECTOR"); }
    
    // -- nothing fits
    return 0;
}


// -- BIND RULE
void Renderer::bindRule(StyleRule *rule) 
{
    view->styles.push_back(rule);   
    view->styles.sort(StyleRule::compareRules); /* sort rules */
    rule->addBoundedView(view);
    
    // out 
    // cout << "[RN of '" << view->id << "'] bind StyleRule '" << rule->selector << "'" <<  endl;
}

void Renderer::unbindRule(StyleRule *rule) 
{ view->styles.remove(rule);   /* sort rules */ view->styles.sort(StyleRule::compareRules); rule->removeBoundedView(view);}

// -- FINDE RULES IN STYLE::RULES AND BIND -----
void Renderer::bindRuleAutomatic() 
{
    // out
    // cout << "[RN of '" << view->id << "'] bindRuleAutomatic '" <<  endl;
    
    // save if own id rule already exists
    bool hasIdRule = false;
     
    // -- for each Rule
    for (list<StyleRule*>::const_iterator iter = Style::rules.begin();
         iter != Style::rules.end();
         iter++)
    {
        // out
        // cout << "[RN of '" << view->id << "'] check StyleRule '" << (*iter)->selector << "'" <<  endl;
        
        // -- check if rule fits 
        int doesRuleFit = checkRuleBinding(*iter);
        if (doesRuleFit >= 1)
        {
            // first
            bindRule(*iter);
//            if (doseRuleFit == 2)
//            {
//                // is views own id rule
//                view->style = *iter;
//                hasIdRule   = true;
//            }
        }
    }
    
    // bind own style rule
    bindRule(&view->style);
     
     // if do not have id rule
//     if (hasIdRule == false)
//     { 
//         // out
//         cout << "[RN of '" << view->id << "'] create own id styleRule" <<  endl;
//         
//         // create one
//         StyleRule *rule = new StyleRule(view->id == "" ? "#" : view->id);
//         bindRule(rule);
//         view->style = rule;
//         //Style::addRule(rule);
//     }
}


// -- CHECK ATTRIBUTE BINDING
StyleAttribute* Renderer::checkAttributeBinding(StyleAttribute::Type type) 
{
    // -- for each Style Rule
    for (list<StyleRule*>::iterator iter = view->styles.begin(); /* iterator to start pos */
         iter != view->styles.end();  /* end if iterator at last pos */
         iter++)
    {
        // -- get attribute of rule
        StyleAttribute *attr = (*iter)->getAttribute(type);
        
        // out
        // cout << "[RN of '" << view->id << "'] check Attribute '" << attr->type << "' of Rule '" << (*iter)->selector << "'" <<  endl; 
        
        // check if isSet
        if (attr->aktive())
        {
            // if is aktive
            // -> found
            return attr;
        }
    }
}


// -- BIND ATTRIBUTE
void Renderer::bindAttribute(StyleAttribute *attribute) 
{
    switch(attribute->type)
    {
        case StyleAttribute::HEIGHT: 
            layoutAttributes.height = (dynamic_cast<IntAttribute*>(attribute));
            break;
            
        case StyleAttribute::WIDTH: 
            layoutAttributes.width = dynamic_cast<IntAttribute*>(attribute);
            break;
            
            
        case StyleAttribute::TOP: 
            layoutAttributes.top = dynamic_cast<IntAttribute*>(attribute);
            break;
            
        case StyleAttribute::BOTTOM: 
            layoutAttributes.bottom = dynamic_cast<IntAttribute*>(attribute);
            break;
            
        case StyleAttribute::LEFT: 
            layoutAttributes.left = dynamic_cast<IntAttribute*>(attribute);
            break;
            
        case StyleAttribute::RIGHT: 
            layoutAttributes.right = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::SCROLL_X:
            layoutAttributes.scrollX = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::SCROLL_Y:
            layoutAttributes.scrollY = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::PADDING_LEFT:
            layoutAttributes.paddingLeft = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::PADDING_RIGHT:
            layoutAttributes.paddingRight = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::PADDING_TOP:
            layoutAttributes.paddingTop = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::PADDING_BOTTOM:
            layoutAttributes.paddingBottom = dynamic_cast<IntAttribute*>(attribute);
            break;

        case StyleAttribute::POSITION: 
            layoutAttributes.position = dynamic_cast<SelectAttribute<UI_ATTR_POSITION_>*>(attribute);
            break;
            
            
        case StyleAttribute::BACKGROUND_COLOR: 
            renderAttributes.background_color = dynamic_cast<ColorAttribute*>(attribute);
            break;
            
        case StyleAttribute::OPACITY: 
            renderAttributes.opacity = dynamic_cast<FloatAttribute*>(attribute);
            break;

        case StyleAttribute::OVERFLOW_CUT:
            renderAttributes.overflow = dynamic_cast<SelectAttribute<UI_ATTR_OVERFLOW_>*>(attribute);
            break;

        case StyleAttribute::TEXT_SIZE:
            renderAttributes.text_size = dynamic_cast<IntAttribute*>(attribute);
            break;     
            
        case StyleAttribute::TEXT_COLOR: 
            renderAttributes.text_color = dynamic_cast<ColorAttribute*>(attribute);
            break; 
            
         case StyleAttribute::TEXT_FAMILY: 
            // cout << "[RN of '" << view->id << "'] at StyleAttribute textFamily '" << attribute->type << "'" <<  endl;
            renderAttributes.text_family = dynamic_cast<StringAttribute*>(attribute);
            break; 
            
        default: warn("of '" + view->id + "' bindAttribute: attribute->type '" + str(attribute->type) + "' does not fit");
    }
    attribute->addBoundedView(view);
    
    // -- out
    // cout << "[RN of '" << view->id << "'] bind StyleAttribute '" << attribute->type << "'" <<  endl;
}

// -- UNBIND ATTRIBUTE 
void Renderer::unbindAttribute(StyleAttribute* attribute) 
{
    attribute->removeBoundedView(view);
}


// -- FINDE FITTING ATTRIBUTE AND BIND -----------
void Renderer::bindAttributeAutomatic() 
{
    // -- for each attribute type
    for (int i = StyleAttribute::_FIRST_TYPE_BEFORE+1;
         !(i >= StyleAttribute::_LAST_TYPE_AFTER/*-1*/);
         i++)
    {
        // out
        // cout << "[RN of '" << view->id << "'] " << "search for attribte binding: " << i << endl;
        
        // -- check each bind rule
        for (list<StyleRule*>::iterator iter = view->styles.begin();
             iter != view->styles.end();
             iter++)
        {
            // out
            // cout << "[RN of '" << view->id << "'] " << "-- check rule: " << (*iter)->selector << endl;
                       
            // -- get attribute of rule
            StyleAttribute *attr = (*iter)->getAttribute(static_cast<StyleAttribute::Type>(i));
            
            // cout << "--- >> aktive = " << attr->aktive() << endl;
            // -- check if attribute is set
            if (attr->aktive())
            {
                // bind
                bindAttribute(attr);
                
                // cancel loop -> next attribute
                // iter = view->styles.end();
                break;
            }
        }
        
//        cout << endl;
    }
     
    // register self as need calc
    for (int i=0; i <= UI_CALCTASK__SIZE; i++)
        addCalcTask(i);
}


// -- CHECK TOUCH POINT IS OVER ------------------------------
View* Renderer::isOver(float x, float y) 
{
//    cout << "--------------------" << endl;
//    cout << "[ RN ] '"<< view->id <<"' isOverX  "<< renderAttributes.positionX - renderAttributes.width/2 <<" < " << x << " < " << renderAttributes.positionX + renderAttributes.width/2 << endl;
//    cout << "[ RN ] '"<< view->id <<"' isOverY  "<< renderAttributes.positionY - renderAttributes.height/2 <<" < " << y << " < " << renderAttributes.positionY + renderAttributes.height/2 << endl;
     
    // isOver count
    Touch::isOverCount++;
    
    // child need is over
    if (touchAttributes.childNeedIsOver)
    {
         // set relative to parent
        if (view->parent) 
        {
            touchAttributes.relativeParent.x =  x + view->parent->renderer->renderAttributes.width/2;
            touchAttributes.relativeParent.y = -y + view->parent->renderer->renderAttributes.height/2;
        }
        
        // set relative to self
        touchAttributes.relativeSelf.x =  x + renderAttributes.width/2;
        touchAttributes.relativeSelf.y = -y + renderAttributes.height/2;
    }
    
    // is over
    if (    (x > renderAttributes.positionX - renderAttributes.width/2)
         && (x < renderAttributes.positionX + renderAttributes.width/2)   
         
         && (y < renderAttributes.positionY + renderAttributes.height/2)
         && (y > renderAttributes.positionY - renderAttributes.height/2))  
    {
        // cout << "[ RN ] '"<< view->id <<"' isOver: true" << endl;

        // set relative to parent
        if (view->parent) 
        {
            touchAttributes.relativeParent.x =  x + view->parent->renderer->renderAttributes.width/2;
            touchAttributes.relativeParent.y = -y + view->parent->renderer->renderAttributes.height/2;
        }
        
        
        // make x,y relative to self
        x-= renderAttributes.positionX;
        y-= renderAttributes.positionY;
        
        // enter
        if (!touchAttributes.isOver)
        {
            // cout << view->id << ": enter" << endl;
            touchAttributes.enter = true;
            touchAttributes.isOver = true;
        }
        else
        {
            //touchAttributes.enter = false;
        }
        
        // set relative to self
        touchAttributes.relativeSelf.x =  x + renderAttributes.width/2;
        touchAttributes.relativeSelf.y = -y + renderAttributes.height/2;
        
        return this->view;
    } 
    else
    {
        // leave
        if (touchAttributes.isOver)
        {
            // cout << view->id << ": leave" << endl;
            touchAttributes.leave = true;
            touchAttributes.isOver = false;
        }
        else
        {
            //touchAttributes.leave = false;
        }
        
        return NULL;
    }
}





// ###########################################
// -- DESTROY OBJEKT -----------
Renderer::~Renderer()
{
    // delete vertex buffer
    glDeleteBuffers(1, &renderAttributes.vertexBuffer);
}

