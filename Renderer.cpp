/* 
 * File:   Renderer.cpp
 * Author: Joshua Johannson | Pancake
 *
 * ---------------------------------------
 * RENDERER CLASS
 * render VIEW
 * ---------------------------------------
 */


#include <string.h>
//#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Style.h"
#include "Box.h"
#include "BoxRenderer.h"
#include "Ui.h"
#include "GL.h"
#include "Touch.h"



// ###########################################
// -- CREATE OBJEKT ------------------
Renderer::Renderer()
{
    // set pointer var
    //renderAttributesPtr = &renderAttributes;
    //layoutAttributesPtr = &layoutAttributes;
}

Renderer::Renderer(View *view) 
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

// -- CALC LAYOUT POSITION
void Renderer::calcLayoutPosition() 
{
    // out
    //cout << "[RN of '" << view->id << "'] " << "calcLayoutPosition()" << endl;
    
    // check if have parrent
    if (view->parent == NULL)
    {
         //cout << "[RN of '" << view->id << "'] " << "has no parrent => is root view" << endl;
        // is root view
        // => position manuall
        // => to 0,0
        renderAttributes.positionX = 0.0f;
        renderAttributes.positionY = 0.0f;
    }
}


// -- CALC LAYOUT SIZE
void Renderer::calcLayoutSize() 
{    
    // cout << "[RN of '" << view->id << "'] " << "clac size width: " << renderAttributes.width << ", height:" << renderAttributes.height << endl;
    float heightHalf = renderAttributes.height / 2;
    float widthHalf  = renderAttributes.width  / 2;
    
    // -- clac vertices
    GLfloat vertices[12] = {  
                                    -widthHalf, -heightHalf, 0.0f, /* left  Bottom */
                                    -widthHalf,  heightHalf, 0.0f, /* left  Top */
                                     widthHalf, -heightHalf, 0.0f, /* right Bottom */
                                     widthHalf,  heightHalf, 0.0f  /* right Top */};
    
    // -- copy into vertices
    memcpy(renderAttributes.vertices, vertices, sizeof(vertices));
    
    // -> need position update
    // calcLayoutPosition();
}

// -- CALC ALL
void Renderer::calcLayout() 
{
    // out
    //cout << endl << "[RN of '" << view->id << "'] " << " calcLayout()" << endl;
    
    calcLayoutSize();
}

// -- ADD CALC TASK
void Renderer::addCalcTask(int type)
{
    // check if calc task fits
    switch (type)
    {
        case UI_CALCTASK_LAYOUT_SIZE:
            // add calc task
             calcTasks[type] = true;
            break;
    }        
}


// -- EXE CALC TASKS
int  Renderer::exeCaclTasks() 
{
    int i;
    
    // for each calc task
    for (i =0; i <= UI_CALCTASK_LAYOUT_SIZE; i++)
    {
        // if true 
        if (calcTasks[i])
        {
            // -> execute
            switch (i)
            {
                case UI_CALCTASK_LAYOUT_SIZE:
                    // calcLayoutSize();
                    // calcTasks[i] = false;
                    // ! not necessary:
                    // !- already called by parent 
                    // !--- by calcLayoutChildSize()
                    break;
            }
        }
    }
    
    return i;
}




// == RENDER =======================================
void Renderer::render() 
{   
    // bind view background shader
    glUseProgram(GL::SHADER_VIEW_BACKGROUND);
    
    // draw a rectangle ----------------------------------------------
    // activate for client (to draw, not calculate) vertex Array
    // => tell gl we want to draw a something with a vertex Array -> to vertexPos attribute of vertexShader
    // => kind  of glBeginn()
    glEnableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);
    
    // set attributes / uniformes  
    // set transform and projection matix
    // remove cursor -> to own position
    glm::mat4 transform = glm::translate(glm::vec3( renderAttributes.positionX /* X */,
                                                    renderAttributes.positionY /* Y */,
                                                    0.0f                       /* Z */ ))
                                                    * GL::transfomMatix;
    glm::mat4 modelMatrix     = GL::projectionMatix * transform;
    
    glUniformMatrix4fv(GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &modelMatrix[0][0]);
    
    GL::transfomMatix = transform;
    
    
    // set shader color
    glUniform4f(GL::SHADER_VIEW_BACKGROUND_UNIF_COLOR,
                renderAttributes.background_color->r,
                renderAttributes.background_color->g,
                renderAttributes.background_color->b,
                renderAttributes.background_color->alpha);
    
    // give gl the Vertices via array
    glVertexAttribPointer(
        GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS      /* pass vertices to vertex Pos attribute of vertexShader */,
        3                                               /* 3 Aruments: x,y,z */,
        GL_FLOAT                                        /* Format: float     */,
        GL_FALSE                                        /* take values as-is */,   
        0                                               /* Entry lenght ?    */,
        renderAttributes.vertices                       /* vertices Array    */ );
    
    // draw with the vertices form the given Array
    // make two connected triangle to create a rectangle
    glDrawArrays(
        GL_TRIANGLE_STRIP,
        0 /* Array start pos   */,
        4 /* how much vertices */);
    
    
    // deactivate vertex Array mode
    // => end of operation
    glDisableVertexAttribArray(GL::SHADER_VIEW_BACKGROUND_ATTR_VERTEX_POS);
    
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
                                                     * GL::transfomMatix;
    glm::mat4 model     = GL::projectionMatix * transform;
    
    glUniformMatrix4fv(GL::SHADER_VIEW_BACKGROUND_UNIF_TRANSFORM_MATIX,
                       1                /* amount of matrix */,
                       GL_FALSE         /* convert format -> NO */,
                       &model[0][0]);
    
    GL::transfomMatix = transform;
    
    /* OLD: OpenGl ES 1.x
    glTranslatef(
            -renderAttributes.positionX /* x *-/,
            -renderAttributes.positionY /* Y *-/,
             0.0f                        /* Z = 0 *-/);
    */
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
        int doseRuleFit = checkRuleBinding(*iter);
        if (doseRuleFit >= 1)
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
    bindRule(view->style);
     
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
            
        case StyleAttribute::POSITION: 
            layoutAttributes.position = dynamic_cast<IntAttribute*>(attribute);
            break;
            
            
        case StyleAttribute::BACKGROUND_COLOR: 
            renderAttributes.background_color = dynamic_cast<ColorAttribute*>(attribute);
            break;
            
        case StyleAttribute::OPACITY: 
            renderAttributes.opacity = dynamic_cast<FloatAttribute*>(attribute);
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
            
        default: cout << "[RN of '" << view->id << "'] error: bindAttribute (attribute->type '" << attribute->type << "' does not fit)" << endl;
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
Renderer::~Renderer() {
}

